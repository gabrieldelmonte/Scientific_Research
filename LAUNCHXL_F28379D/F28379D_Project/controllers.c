/**
 * @file controllers.c
 * @brief Implementation of unified controller interface
 * @author Gabriel Del Monte
 * @date 2025
 */

#include "controllers.h"

// Global controller instances
NeuralNetwork neural_network;
PIController pi_controller;
uint8_t current_controller_type = PI_CONTROLLER;

/**
 * @brief Initialize the selected controller
 * @param controller_type Type of controller to initialize
 * @return void
 */
void controller_init(uint8_t controller_type) {
    current_controller_type = controller_type;

    if (controller_type == NNA_CONTROLLER)
        neural_network_init();
    else if (controller_type == PI_CONTROLLER)
        pi_controller_init();

    return;
}

/**
 * @brief Compute controller output using the selected controller
 * @param setpoint Desired setpoint value
 * @param measured_voltage Measured voltage value
 * @param measured_current Measured current value
 * @return Computed controller output
 */
float controller_compute(float setpoint, float measured_voltage, float measured_current) {
    if (current_controller_type == NNA_CONTROLLER)
        return neural_network_compute(setpoint, measured_voltage, measured_current);
    else if (current_controller_type == PI_CONTROLLER)
        return pi_controller_compute(setpoint, measured_voltage);
    else
        return 0.0f;
}

/**
 * @brief Reset the current controller state
 * @return void
 */
void controller_reset(void) {
    if (current_controller_type == NNA_CONTROLLER)
        neural_network_reset();
    else
        pi_controller_reset();

    return;
}

// PI Controller implementation

/**
 * @brief Initialize PI controller with tuned parameters
 * @return void
 */
void pi_controller_init(void) {
    pi_controller.error         = 0.0f;
    pi_controller.error_old     = 0.0f;
    pi_controller.output        = 0.0f;
    pi_controller.output_old    = 0.0f;

    // Tuned parameters for buck converter
    pi_controller.b0 = 0.063288f;
    pi_controller.b1 = -0.060934f;
    pi_controller.a1 = -1.0f;

    return;
}

/**
 * @brief Compute PI controller output
 * @param setpoint Desired setpoint value
 * @param measured_voltage Measured voltage value
 * @return Computed controller output
 */
float pi_controller_compute(float setpoint, float measured_voltage) {
    pi_controller.error = (setpoint - measured_voltage);

    pi_controller.output =
        (pi_controller.error * pi_controller.b0) +
        (pi_controller.error_old * pi_controller.b1) -
        (pi_controller.output_old * pi_controller.a1);

    pi_controller.error_old = pi_controller.error;
    pi_controller.output_old = pi_controller.output;

    // Saturate output
    if (pi_controller.output >= 1.0f)
        return 1.0f;
    if (pi_controller.output <= 0.0f)
        return 0.0f;

    return pi_controller.output;
}

/**
 * @brief Reset PI controller state
 * @return void
 */
void pi_controller_reset(void) {
    pi_controller.error         = 0.0f;
    pi_controller.error_old     = 0.0f;
    pi_controller.output        = 0.0f;
    pi_controller.output_old    = 0.0f;

    return;
}

// Neural Network implementation

/**
 * @brief Initialize neural network with random weights
 * @return void
 */
void neural_network_init(void) {
    int x, y;
    float sqrt_layers;

    // Initialize weights using He initialization
    sqrt_layers = custom_sqrt(2.0f / INPUT_SIZE);
    for (x = 0; x < INPUT_SIZE; x++)
        for (y = 0; y < HIDDEN1_SIZE; y++)
            neural_network.weights_h1[x][y] = (random_float() * 2.0f - 1.0f) * sqrt_layers;

    sqrt_layers = custom_sqrt(2.0f / HIDDEN1_SIZE);
    for (x = 0; x < HIDDEN1_SIZE; x++) {
        neural_network.bias_h1[x] = 0.01f;

        for (y = 0; y < HIDDEN2_SIZE; y++)
            neural_network.weights_h2[x][y] = (random_float() * 2.0f - 1.0f) * sqrt_layers;
    }

    sqrt_layers = custom_sqrt(2.0f / HIDDEN2_SIZE);
    for (x = 0; x < HIDDEN2_SIZE; x++) {
        neural_network.bias_h2[x] = 0.01f;
        neural_network.weights_output[x] = (random_float() * 2.0f - 1.0f) * sqrt_layers;
    }

    neural_network.bias_output = 0.01f;

    return;
}

/**
 * @brief Compute neural network output with real-time training
 * @param setpoint Desired setpoint value
 * @param measured_voltage Measured voltage value
 * @param measured_current Measured current value
 * @return Computed controller output
 */
float neural_network_compute(float setpoint, float measured_voltage, float measured_current) {
    float inputs[INPUT_SIZE];
    float output_network;
    float error, error_norm;

    // Prepare inputs
    inputs[0] = BIAS;
    inputs[1] = (2.0f * measured_voltage / MAX_VOLTAGE) - 1.0f;
    inputs[2] = (measured_current / MAX_CURRENT_mA) * 2.0f - 1.0f;

    if (inputs[2] < -1.0f)
        inputs[2] = -1.0f;
    if (inputs[2] > 1.0f)
        inputs[2] = 1.0f;

    // Forward pass
    output_network = neural_network_forward(inputs);

    // Clamp output
    if (output_network > 0.975f)
        output_network = 0.975f;
    if (output_network < 0.025f)
        output_network = 0.025f;

    // Training
    error = setpoint - measured_voltage;
    error_norm = error / MAX_VOLTAGE;
    neural_network_backpropagate(inputs, setpoint, error_norm);

    return output_network;
}

/**
 * @brief Reset neural network
 * @return void
 */
void neural_network_reset(void) {
    neural_network_init();

    return;
}

// Utility functions

/**
 * @brief Compute the square root of a value
 * @param value The value to compute the square root of
 * @return float The computed square root
 */
float custom_sqrt(float value) {
    if (value <= 0)
        return 0;

    float guess = value;
    int i;

    for (i = 0; i < 10; i++)
        guess = 0.5f * (guess + value / guess);

    return guess;
}

/**
 * @brief Generate a random float value
 * @return float A random float value
 */
float random_float(void) {
    static unsigned long int seed = 12345;
    const unsigned long int a = 12072001;
    const unsigned long int c = 2406202212;

    seed = (a * seed + c) % 0xFFFFFFFF;

    return (float)seed / (float)0xFFFFFFFF;
}

/**
 * @brief Compute the rectified linear unit (ReLU) activation
 * @param value The input value
 * @return float The output value
 */
float relu(float value) {
    return (value > 0.0f) ? value : 0.0f;
}

/**
 * @brief Compute the leaky ReLU activation
 * @param value The input value
 * @return float The output value
 */
float leaky_relu(float value) {
    return (value > 0.0f) ? value : 0.01f * value;
}

/**
 * @brief Compute the rectified linear unit (ReLU) activation
 * @param value The input value
 * @return float The output value
 */
float relu_clipped(float value) {
    value = leaky_relu(value);

    return (value < 1.0f) ? value : 1.0f;
}

/**
 * @brief Compute the sigmoid activation
 * @param output The input value
 * @return float The output value
 */
float sigmoid(float output) {
    float x = ALPHA * output;
    if (x > 10.0f)
        x = 10.0f;
    if (x < -10.0f)
        x = -10.0f;

    return 0.5f + x / (2.0f * (0.5f + (x < 0 ? -x : x)));
}

/**
 * @brief Neural network forward pass
 * @param inputs The input values
 * @return float The output value
 */
float neural_network_forward(float inputs[INPUT_SIZE]) {
    float h1[HIDDEN1_SIZE];
    float h2[HIDDEN2_SIZE];
    float output;
    int x, y;

    // Hidden layer 1
    for (x = 0; x < HIDDEN1_SIZE; x++) {
        h1[x] = neural_network.bias_h1[x];

        for (y = 0; y < INPUT_SIZE; y++)
            h1[x] += inputs[y] * neural_network.weights_h1[y][x];

        h1[x] = relu(h1[x]);
    }

    // Hidden layer 2
    for (x = 0; x < HIDDEN2_SIZE; x++) {
        h2[x] = neural_network.bias_h2[x];

        for (y = 0; y < HIDDEN1_SIZE; y++)
            h2[x] += h1[y] * neural_network.weights_h2[y][x];

        h2[x] = relu(h2[x]);
    }

    // Output layer
    output = neural_network.bias_output;
    for (x = 0; x < HIDDEN2_SIZE; x++)
        output += h2[x] * neural_network.weights_output[x];

    return relu_clipped(output);
}

/**
 * @brief Neural network backpropagation
 * @param inputs The input values
 * @param target The target output value
 * @param error The error value
 * @return void
 */
void neural_network_backpropagate(float inputs[INPUT_SIZE], float target, float error) {
    float h1[HIDDEN1_SIZE];
    float h2[HIDDEN2_SIZE];
    float delta_h2[HIDDEN2_SIZE];
    float delta_h1, delta_output;
    float output, grad_h1, grad_h2, sum;
    int x, y;

    // Forward pass to get intermediate values
    for (x = 0; x < HIDDEN1_SIZE; x++) {
        h1[x] = neural_network.bias_h1[x];

        for (y = 0; y < INPUT_SIZE; y++)
            h1[x] += inputs[y] * neural_network.weights_h1[y][x];
        h1[x] = relu(h1[x]);
    }

    for (x = 0; x < HIDDEN2_SIZE; x++) {
        h2[x] = neural_network.bias_h2[x];

        for (y = 0; y < HIDDEN1_SIZE; y++)
            h2[x] += h1[y] * neural_network.weights_h2[y][x];

        h2[x] = relu(h2[x]);
    }

    output = neural_network.bias_output;
    for (x = 0; x < HIDDEN2_SIZE; x++)
        output += h2[x] * neural_network.weights_output[x];

    output = relu(output);

    // Backpropagation
    delta_output = error * (output > 0.0f ? 1.0f : 0.0f);

    // Update output layer
    for (x = 0; x < HIDDEN2_SIZE; x++)
        neural_network.weights_output[x] += ETA * delta_output * h2[x];

    neural_network.bias_output += ETA * delta_output;

    // Update hidden layer 2
    for (x = 0; x < HIDDEN2_SIZE; x++) {
        grad_h2 = (h2[x] > 0.0f ? 1.0f : 0.0f);
        delta_h2[x] = delta_output * neural_network.weights_output[x] * grad_h2;

        for (y = 0; y < HIDDEN1_SIZE; y++)
            neural_network.weights_h2[y][x] += ETA * delta_h2[x] * h1[y];

        neural_network.bias_h2[x] += ETA * delta_h2[x];
    }

    // Update hidden layer 1
    for (y = 0; y < HIDDEN1_SIZE; y++) {
        sum = 0.0f;

        for (x = 0; x < HIDDEN2_SIZE; x++)
            sum += delta_h2[x] * neural_network.weights_h2[y][x];

        grad_h1 = (h1[y] > 0.0f ? 1.0f : 0.0f);
        delta_h1 = sum * grad_h1;

        for (x = 0; x < INPUT_SIZE; x++)
            neural_network.weights_h1[x][y] += ETA * delta_h1 * inputs[x];

        neural_network.bias_h1[y] += ETA * delta_h1;
    }

    return;
}

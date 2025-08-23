/**
 * @file controllers.h
 * @brief Unified controller interface for PI and Neural Network controllers
 * @author Gabriel Del Monte
 * @date 2025
 */

#ifndef CONTROLLERS_H
#define CONTROLLERS_H

    #include "peripheral_Setup.h"

    // Controller types
    #define PI_CONTROLLER   0
    #define NNA_CONTROLLER  1

    // Neural Network parameters
    #define ALPHA           0.4f
    #define BIAS            1.0f
    #define ETA             1.0f / (1.0f * 100.0f)

    #define INPUT_SIZE      3
    #define HIDDEN1_SIZE    3
    #define HIDDEN2_SIZE    2

    /**
     * @brief Neural Network structure (3-3-2-1 architecture)
     */
    typedef struct {
        float weights_h1[INPUT_SIZE][HIDDEN1_SIZE];
        float bias_h1[HIDDEN1_SIZE];

        float weights_h2[HIDDEN1_SIZE][HIDDEN2_SIZE];
        float bias_h2[HIDDEN2_SIZE];

        float weights_output[HIDDEN2_SIZE];
        float bias_output;
    } NeuralNetwork;

    /**
     * @brief PI Controller structure
     */
    typedef struct {
        float error;
        float error_old;

        float output;
        float output_old;

        float b0, b1, a1;  // PI parameters
    } PIController;

    // Global controller instances
    extern NeuralNetwork neural_network;
    extern PIController pi_controller;
    extern uint8_t current_controller_type;

    // Controller interface functions
    void controller_init(uint8_t controller_type);
    float controller_compute(float setpoint, float measured_voltage, float measured_current);
    void controller_reset(void);

    // PI Controller functions
    void pi_controller_init(void);
    float pi_controller_compute(float setpoint, float measured_voltage);
    void pi_controller_reset(void);

    // Neural Network functions
    void neural_network_init(void);
    float neural_network_compute(float setpoint, float measured_voltage, float measured_current);
    void neural_network_reset(void);

    // Neural Network utility functions
    float custom_sqrt(float value);
    float random_float(void);
    float relu(float value);
    float leaky_relu(float value);
    float relu_clipped(float value);
    float sigmoid(float output);
    float neural_network_forward(float inputs[INPUT_SIZE]);
    void neural_network_backpropagate(float inputs[INPUT_SIZE], float target, float error);

#endif /* CONTROLLERS_H */

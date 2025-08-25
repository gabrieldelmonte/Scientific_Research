# F28379D Buck Converter Control System

This project implements a unified control system for a buck converter using the TI F28379D LaunchPad. The system supports two different control algorithms:
- **PI Controller** (traditional control approach)
- **Neural Network Approximator (NNA)** with real-time training

## Features

- **Dual Controller Architecture**: Switch between PI and Neural Network controllers
- **Real-time Control**: FreeRTOS-based task scheduling for precise timing
- **ADC Monitoring**: Voltage and current sensing with rolling average filtering
- **PWM Generation**: 20kHz switching frequency control
- **UART Communication**: Real-time data transmission at 9600 baud
- **RTC Integration**: DS3231 real-time clock for timestamping via I2C
- **Watchdog Protection**: System reliability monitoring
- **Button Interface**: System ON/OFF control via GPIO buttons
- **LED Status Indicators**: Visual feedback for system state

## Hardware Requirements

- **TI LAUNCHXL-F28379D** LaunchPad
- **Buck Converter Circuit** with:
  - Voltage sensing circuit (0-10V range, ADC channels B2, B3)
  - Current sensing circuit (0-1A range, ADC channel C3)
  - Setpoint potentiometer (ADC channel A2)
  - Input voltage monitoring (ADC channel B2)
  - PWM-controlled MOSFET driver (EPWM1A output)
- **DS3231 RTC Module** (I2C interface on GPIO40/41)
- **UART Interface** (GPIO56/139, 9600 baud) for data communication
- **Control Buttons**: Start (GPIO67) and Stop (GPIO111)
- **Status LEDs**: GPIO31 and GPIO34

## Project Structure

```
F28379D_Project/
├── main.c                  # Main application entry point
├── controllers.c/h         # Unified controller implementation
├── peripheral_Setup.c/h    # Hardware peripheral configuration
├── freeRTOS_Tasks.c/h      # Real-time task definitions
├── Libraries/              # TI driver libraries and FreeRTOS
├── Peripheral/             # Custom peripheral drivers
└── Debug/                  # Build output directory
```

## Configuration

### Controller Selection

Edit `main.c` to select the desired controller:

```c
/**
 * @brief Controller selection define
 *        Set to 0 for PI Controller
 *        Set to 1 for Neural Network Approximator (NNA)
 */
#define CONTROLLER 1  // Change to 0 for PI, 1 for NNA
```

### System Parameters

Key parameters are defined in `peripheral_Setup.h`:

```c
#define MAX_VOLTAGE                 10.0f    // Maximum voltage (V)
#define MAX_CURRENT                 1.00f    // Maximum current (A)
#define VOLTAGE_CONVERSION_FACTOR   0.0060f  // ADC to voltage conversion
#define CURRENT_CONVERSION_FACTOR   0.6300f  // ADC to current conversion
```

## Controller Configuration

### PI Controller

The PI controller uses a discrete-time implementation with pre-tuned parameters in the structure:

```c
typedef struct {
    float error;        // Current error
    float error_old;    // Previous error
    float output;       // Current output
    float output_old;   // Previous output
    float b0, b1, a1;   // PI coefficients
} PIController;

// Default PI parameters (in controllers.c)
pi_controller.b0 = 0.063288f;   // Proportional gain coefficient
pi_controller.b1 = -0.060934f;  // Integral gain coefficient  
pi_controller.a1 = -1.0f;       // Feedback coefficient
```

The PI control law is implemented as:
```c
output = (error * b0) + (error_old * b1) - (output_old * a1)
```

**IMPORTANT: PI Controller Tuning**
> If you are going to use the PI Controller, you **MUST** recalculate the `b1`, `b0`, and `a1` values according to your specific system parameters (inductance, capacitance, load resistance, switching frequency, etc.). The current values are tuned for a specific buck converter design and may not work optimally with your hardware.

**PI Tuning Steps:**
1. Identify your system's transfer function
2. Design the continuous-time PI controller
3. Convert to discrete-time using appropriate method (Tustin, ZOH, etc.)
4. Update the coefficients in `pi_controller_init()`

### Neural Network Approximator (NNA)

The NNA uses a 3-3-2-1 feedforward architecture with ReLU activation functions:

**Network Structure:**
- **Input Layer**: 3 neurons (bias=1.0, normalized voltage, normalized current)
- **Hidden Layer 1**: 3 neurons with ReLU activation
- **Hidden Layer 2**: 2 neurons with ReLU activation  
- **Output Layer**: 1 neuron with clipped ReLU activation

```c
typedef struct {
    float weights_h1[INPUT_SIZE][HIDDEN1_SIZE];     // Input to Hidden1 weights
    float bias_h1[HIDDEN1_SIZE];                    // Hidden1 biases
    float weights_h2[HIDDEN1_SIZE][HIDDEN2_SIZE];   // Hidden1 to Hidden2 weights
    float bias_h2[HIDDEN2_SIZE];                    // Hidden2 biases
    float weights_output[HIDDEN2_SIZE];             // Hidden2 to Output weights
    float bias_output;                              // Output bias
} NeuralNetwork;

// Neural Network parameters (in controllers.h)
#define ALPHA           0.4f                    // Sigmoid activation parameter
#define BIAS            1.0f                    // Bias input value
#define ETA             1.0f / (1.0f * 100.0f)  // Learning rate (0.01)
#define INPUT_SIZE      3                       // Inputs: bias, voltage, current
#define HIDDEN1_SIZE    3                       // First hidden layer neurons
#define HIDDEN2_SIZE    2                       // Second hidden layer neurons
```

**Input Normalization:**
- `inputs[0] = BIAS` (always 1.0)
- `inputs[1] = (2.0 * measured_voltage / MAX_VOLTAGE) - 1.0` (range: -1 to +1)
- `inputs[2] = (measured_current / MAX_CURRENT_mA) * 2.0 - 1.0` (range: -1 to +1)

**Weight Initialization:** He initialization for ReLU networks
**Training:** Real-time backpropagation with normalized error

**IMPORTANT: NNA Learning Rate**
> If you are going to use the NNA Controller, you should **check if the learning rate (`ETA`) isn't too high for your project**. A learning rate that's too high can cause:
> - Unstable learning behavior
> - Oscillations in control output
> - Poor convergence or divergence

**Learning Rate Guidelines:**
- Current value: `ETA = 0.01` (1.0 / (1.0 * 100.0))
- Start with a conservative value (e.g., 0.001-0.01)
- Monitor control performance during initial operation
- Reduce if you observe oscillations or instability
- Increase gradually if learning is too slow

## Build Instructions

### Using Code Composer Studio (CCS)

1. **Import Project**:
   - Open Code Composer Studio
   - File → Import → Existing CCS/CCE Eclipse Projects
   - Browse to the F28379D_Project folder
   - Select the project and click Finish

2. **Configure Build**:
   - Right-click project → Properties
   - Verify target device: TMS320F28379D
   - Check include paths and library dependencies

3. **Build**:
   - Project → Build Project
   - Or use Ctrl+B

4. **Debug/Flash**:
   - Connect LaunchPad via USB
   - Run → Debug (F5)
   - Or Run → Load → Load Program

## System Operation

### Task Architecture

The system uses three FreeRTOS tasks running on static allocation:

1. **Control Task** (1ms period, priority 3):
   - Reads ADC values via ISR (voltage, current, setpoint)
   - Applies safety checks and filtering
   - Executes selected controller algorithm
   - Updates PWM duty cycle (EPWM1_Modulante_CMPA)
   - Implements output saturation (0.025-0.975 range)

2. **Communication Task** (4000ms period, priority 1):
   - Transmits system data via UART (9600 baud)
   - Formats: `setpoint.x,voltage.x,current.x,HH:MM:SS`
   - Sends "OFF" when system is disabled
   - Controls LED indicators (GPIO31, GPIO34)

3. **Time Update Task** (1ms period, priority 1):
   - Manages RTC communication via I2C
   - Converts BCD time format to decimal
   - Provides system timestamps
   - Handles I2C timeout and error recovery

### Interrupt Service Routine (Timer0 - 50ms)

The main control loop runs in Timer0 ISR with multiple functions:
- **Button Monitoring**: GPIO67 (START), GPIO111 (STOP)
- **ADC Data Processing**: Reads all 4 ADC channels
- **Setpoint Filtering**: 10-sample rolling average filter
- **Input Voltage Monitoring**: Safety check for overvoltage
- **Safety Limiting**: Prevents setpoint > 95% of input voltage


### System States

- **OFF State**: PWM disabled, controller reset, LEDs off
- **ON State**: Controllers active, PWM enabled, data logging
- **Safety Mode**: Automatic shutdown on overvoltage/button press

## Monitoring and Debugging

### UART Output Format

The system transmits data in the following CSV format every 4 seconds:
```
SETPOINT.DECIMAL,VOLTAGE.DECIMAL,CURRENT.DECIMAL,HH:MM:SS
```

**Active System Examples:**
```
5.2,4.98,0.75,00:01:23
6.0,5.95,0.82,00:01:27
3.5,3.48,0.45,00:01:31
```

**System Off Example:**
```
OFF,00:01:35
```

**Data Fields:**
- **Setpoint**: Target voltage from potentiometer (filtered)
- **Voltage**: Measured output voltage from buck converter
- **Current**: Measured load current
- **Timestamp**: Real-time clock (HH:MM:SS format)

### Debug Features

- **Watchdog Timer**: Automatically resets system if tasks hang (serviced in each task)
- **I2C Error Handling**: Timeout and NACK error detection with recovery
- **Output Saturation**: PWM duty cycle clamped to 2.5%-97.5% range
- **Voltage Safety**: Setpoint limited to 95% of input voltage
- **ADC Validation**: Rolling average filter with 10 samples
- **Button Debouncing**: GPIO qualification with 100-sample filter
- **LED Status Indicators**: 
  - GPIO31: System active (low = ON)
  - GPIO34: Communication active (low = transmitting)

## Safety Considerations

- **Output Saturation**: PWM duty cycle is limited to 0.025-0.975 range (2.5%-97.5%)
- **Voltage Limits**: System designed for 0-10V operation with safety limiting
- **Current Limits**: Maximum current monitoring up to 1A (1000mA)
- **Input Voltage Protection**: Setpoint automatically limited to 95% of input voltage
- **Watchdog Protection**: Automatic reset on system hang (WD_PS_1 prescaler)
- **Emergency Stop**: STOP button (GPIO111) immediately disables PWM output
- **Start-up Safety**: System starts in OFF state, requires button press to activate
- **ADC Range Protection**: All ADC inputs scaled and validated (4095 max count)
- **I2C Timeout Protection**: 10000-cycle timeout prevents I2C bus lockup
- **FreeRTOS Stack Overflow Detection**: Automatic system halt on stack overflow

## Customization

### Adding New Controllers

To implement a new controller:

1. **Define Controller Structure** in `controllers.h`:
```c
typedef struct {
    // Add your controller variables here
    float custom_param1;
    float custom_param2;
    // ...
} CustomController;
```

2. **Add Controller Type**:
```c
#define CUSTOM_CONTROLLER  2  // Add after NNA_CONTROLLER
```

3. **Implement Functions** in `controllers.c`:
```c
void custom_controller_init(void);
float custom_controller_compute(float setpoint, float voltage, float current);
void custom_controller_reset(void);
```

4. **Update Unified Interface**:
   - Add cases in `controller_init()`, `controller_compute()`, and `controller_reset()`

### Modifying System Parameters

Key parameters to adjust for your hardware:

**ADC Conversion Factors** (in `peripheral_Setup.h`):
```c
#define VOLTAGE_CONVERSION_FACTOR   0.0060f // Adjust for your voltage divider
#define CURRENT_CONVERSION_FACTOR   0.6300f // Adjust for your current sensor
#define INPUT_CONVERSION_FACTOR     0.0045f // Adjust for input voltage scaling
```

**System Limits**:
```c
#define MAX_VOLTAGE                 10.0f   // Maximum voltage (V)
#define MAX_CURRENT                 1.00f   // Maximum current (A)
#define MAX_ADC                     4095.0f // 12-bit ADC resolution
```

**Task Timing** (in `freeRTOS_Tasks.h`):
```c
#define TASK1_LOOP_DELAY    4000    // Communication period (ms)
#define TASK2_LOOP_DELAY    1       // Control period (ms)
#define TASK3_LOOP_DELAY    1       // Time update period (ms)
```

**PWM Frequency** (in `peripheral_Setup.c`):
```c
ConfigEPwm_REF(EPWM1, ePWM_HSPCLKDIV_1, ePWM_CLKDIV_1, 20000);  // 20kHz
```

**Timer0 ISR Period** (in `peripheral_Setup.c`):
```c
ConfigCpuTimer(&CpuTimer0, 100, 50);  // 50ms period
```

**Rolling Average Filter Size**:
```c
#define SIZE_ADC_READINGS           10  // Number of samples to average
```

## Troubleshooting

### Common Issues

1. **No PWM Output**:
   - Check system state: Press START button (GPIO67)
   - Verify EPWM1A GPIO configuration in `peripheral_Setup.c`
   - Ensure controller output is within 0.025-0.975 range
   - Check if setpoint < 95% of input voltage

2. **Incorrect ADC Readings**:
   - Verify conversion factors: `VOLTAGE_CONVERSION_FACTOR`, `CURRENT_CONVERSION_FACTOR`
    - These conversion factors are specific for each system
   - Check ADC channel assignments: A2(setpoint), B2(input), B3(voltage), C3(current)
   - Validate input signal conditioning circuits
   - Monitor raw ADC values in debugger

3. **Controller Instability**:
   - **For PI**: Retune `b0`, `b1`, `a1` parameters for your specific system
   - **For NNA**: Reduce learning rate `ETA` (try 0.001-0.005)
   - Check for noise in feedback signals
   - Verify setpoint filtering is working (10-sample average)

4. **UART Communication Issues**:
   - Verify baud rate: 9600 (both ends)
   - Check GPIO18 (TX) and GPIO19 (RX) connections
   - Ensure proper ground reference between devices
   - Monitor communication task LED (GPIO34)

5. **I2C/RTC Problems**:
   - Check DS3231 module connections: GPIO40 (SDA), GPIO41 (SCL)
   - Verify pull-up resistors on I2C lines, if not defined on GPIO104 and GPIO105
   - Monitor `i2c_status` variable for error codes
   - Check 3.3V power supply to DS3231

6. **System Resets/Watchdog Issues**:
   - Each task calls `ServiceDog()` - check for task blocking
   - Monitor FreeRTOS stack usage in debugger
   - Check for infinite loops in ISR or tasks
   - Verify Timer0 ISR is not taking too long

7. **Button Control Not Working**:
   - Check button connections: GPIO67 (START), GPIO111 (STOP)
   - Verify pull-up resistors are enabled
   - Monitor button debouncing (QUAL3, 100 samples)
   - Check button logic in Timer0 ISR

### Debug Steps

1. **Hardware Verification**:
   - Verify all power supply voltages (3.3V, 5V, input voltage)
   - Check GPIO pin assignments and connections
   - Test individual peripherals (ADC, PWM, UART, I2C)

2. **Software Debug**:
   - Use CCS debugger to monitor variables in real-time
   - Set breakpoints in controller functions
   - Monitor UART output for system status
   - Check FreeRTOS task states and stack usage

3. **System Integration**:
   - Start with PI controller (simpler to debug)
   - Verify setpoint potentiometer and ADC readings
   - Test PWM output with multimeter/oscilloscope
   - Gradually enable advanced features (NNA, RTC)

### Error Codes

**I2C Status Codes**:
- `I2C_SUCCESS`: Operation successful
- `I2C_NACK_ERROR`: No acknowledgment from DS3231
- `I2C_TIMEOUT_ERROR`: Communication timeout
- `I2C_BUS_BUSY_ERROR`: I2C bus stuck busy
- `I2C_STP_NOT_READY_ERROR`: Stop condition not cleared

**System State Indicators**:
- `system_state = ON (1)`: System active and controlling
- `system_state = OFF (0)`: System disabled, PWM off
- GPIO31 = LOW: Control task active
- GPIO34 = LOW: Communication task transmitting

## License

This project is licensed under the terms specified in the LICENSE file.

## Author

**Gabriel Del Monte** - 2025

---

For questions or support, please refer to the project documentation or contact the development team.

/**
 * @file freeRTOS_Tasks.c
 * @brief FreeRTOS task implementation for Buck Converter Control
 * @author Gabriel Del Monte
 * @date 2025
 */

#include "freeRTOS_Tasks.h"

// Static task buffers
static StaticTask_t update_time_task_buffer;
static StaticTask_t communication_task_buffer;
static StaticTask_t control_task_buffer;
static StaticTask_t idle_task_buffer;

static StackType_t update_time_task_stack[STACK_SIZE];
static StackType_t communication_task_stack[STACK_SIZE];
static StackType_t control_task_stack[STACK_SIZE];
static StackType_t idle_task_stack[STACK_SIZE];

// freeRTOS objects
SemaphoreHandle_t communication_semaphore = NULL;
QueueHandle_t control_queue = NULL;

// External variables
extern MEDIDA medidasADC;
extern InputMonitor input_monitor;
extern SetpointFilter setpoint_filter;

extern uint16_t i2c_status;
extern uint16_t pwm_factor;

extern uint16_t hours;
extern uint16_t minutes;
extern uint16_t seconds;

extern uint16_t hours_decimal;
extern uint16_t minutes_decimal;
extern uint16_t seconds_decimal;

extern float duty_cycle;
extern char system_state;

void update_time_task(void *pvParameters) {
    vTaskDelay(TASK3_STARTUP_DELAY / portTICK_PERIOD_MS);

    while (1) {
        vTaskDelay(TASK3_LOOP_DELAY / portTICK_PERIOD_MS);
        ServiceDog();

        i2c_status = ds3231_read_time(&hours, &minutes, &seconds);

        if (i2c_status == I2C_SUCCESS) {
            hours_decimal = bcd_to_decimal(hours);
            minutes_decimal = bcd_to_decimal(minutes);
            seconds_decimal = bcd_to_decimal(seconds);
        }

        vTaskDelay(TASK3_END_DELAY / portTICK_PERIOD_MS);
    }

}

/**
 * @brief Communication task - handles UART transmission
 *        according to the defined protocol:
 *          SETPOINT.VALUE
 *          ,
 *          CIRCUIT_VOLTAGE
 *          ,
 *          CIRCUIT_CURRENT
 *          ,
 *          HOURS_DECIMAL
 *          :
 *          MINUTES_DECIMAL
 *          :
 *          SECONDS_DECIMAL
 */
void communication_task(void *pvParameters) {
    vTaskDelay(TASK1_STARTUP_DELAY / portTICK_PERIOD_MS);

    while (1) {
        vTaskDelay(TASK1_LOOP_DELAY / portTICK_PERIOD_MS);
        ServiceDog();

        if (system_state) {
            GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1;

            int decimal_part;

            uart_send_int(setpoint_filter.setpoint);
            decimal_part = (setpoint_filter.setpoint - (int)setpoint_filter.setpoint) * 10;
            uart_send_char('.');
            uart_send_int(decimal_part);

            uart_send_char(',');

            uart_send_int(medidasADC.valor_real[Tensao_DC]);
            decimal_part = (medidasADC.valor_real[Tensao_DC] - (int)medidasADC.valor_real[Tensao_DC]) * 10;
            uart_send_char('.');
            uart_send_int(decimal_part);

            uart_send_char(',');

            uart_send_int(medidasADC.valor_real[Corrente_carga]);
            decimal_part = (medidasADC.valor_real[Corrente_carga] - (int)medidasADC.valor_real[Corrente_carga]) * 10;
            uart_send_char('.');
            if (decimal_part > 0)
                uart_send_int(decimal_part);
            else
                uart_send_int(0);
        }
        else
            uart_send_string("OFF");

        uart_send_char(',');

        if (hours_decimal < 10)
            uart_send_char('0');
        uart_send_int(hours_decimal);

        uart_send_char(':');

        if (minutes_decimal < 10)
            uart_send_char('0');
        uart_send_int(minutes_decimal);

        uart_send_char(':');

        if (seconds_decimal < 10)
            uart_send_char('0');
        uart_send_int(seconds_decimal);

        vTaskDelay(TASK1_END_DELAY / portTICK_PERIOD_MS);
    }
}

/**
 * @brief Control task - executes main control loop
 */
void control_task(void *pvParameters) {
    float controller_output = 0.0f;
    char reset_flag = 1;

    vTaskDelay(TASK2_STARTUP_DELAY / portTICK_PERIOD_MS);

    while (1) {
        vTaskDelay(TASK2_LOOP_DELAY / portTICK_PERIOD_MS);
        ServiceDog();

        if (system_state) {
            GpioDataRegs.GPACLEAR.bit.GPIO31 = 1;

            reset_flag = 1;

            if (setpoint_filter.setpoint < (0.975f * input_monitor.voltage)) {
                controller_output = controller_compute(
                    setpoint_filter.setpoint,
                    medidasADC.valor_real[Tensao_DC],
                    medidasADC.valor_real[Corrente_carga]
                );

                if (controller_output > 0.975f)
                    controller_output = 0.975f;
                if (controller_output < 0.025f)
                    controller_output = 0.025f;

                EPWM1_Modulante_CMPA = controller_output * pwm_factor;
                duty_cycle = controller_output;
            }
        }
        else {
            if (reset_flag) {
                controller_reset();
                reset_flag = 0;
            }

            duty_cycle = 0.0f;
        }

        vTaskDelay(TASK2_END_DELAY / portTICK_PERIOD_MS);
    }
}

/**
 * @brief Initialize FreeRTOS system and start scheduler
 */
void freeRTOS_Setup(void) {
    static StaticSemaphore_t semaphore_buffer;
    static uint8_t queue_storage[1 * sizeof(float)];
    static StaticQueue_t static_queue;

    communication_semaphore = xSemaphoreCreateMutexStatic(&semaphore_buffer);
    xSemaphoreGive(communication_semaphore);

    control_queue = xQueueCreateStatic(1, sizeof(float), queue_storage, &static_queue);

    xTaskCreateStatic(
        update_time_task,
        "UpdateTimeTask",
        STACK_SIZE, 
        (void *)NULL,
        tskIDLE_PRIORITY + 1,
        update_time_task_stack,
        &update_time_task_buffer
    );

    xTaskCreateStatic(
        communication_task,
        "CommTask",
        STACK_SIZE, 
        (void *)NULL,
        tskIDLE_PRIORITY + 1,
        communication_task_stack,
        &communication_task_buffer
    );

    xTaskCreateStatic(
        control_task,
        "ControlTask",
        STACK_SIZE, 
        (void *)NULL,
        tskIDLE_PRIORITY + 4,
        control_task_stack,
        &control_task_buffer
    );

    vTaskStartScheduler();
}

// ========== FREERTOS CALLBACK FUNCTIONS ==========

/**
 * @brief Provide memory for idle task
 *        Required callback for static allocation mode
 */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, 
                                  StackType_t **ppxIdleTaskStackBuffer, 
                                  uint32_t *pulIdleTaskStackSize) {
    *ppxIdleTaskTCBBuffer = &idle_task_buffer;
    *ppxIdleTaskStackBuffer = idle_task_stack;
    *pulIdleTaskStackSize = STACK_SIZE;
}

/**
 * @brief Handle stack overflow detection
 *        Called when FreeRTOS detects a stack overflow
 * @param xTask Handle of overflowed task
 * @param pcTaskName Name of overflowed task
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    // Infinite loop to halt system - allows debugging
    while (1) {
        // Could add additional diagnostic code here...
    }
}

/**
 * @brief Handle assertion failures
 *        Called when driverlib ASSERT macro fails
 * @param filename Source file where assertion failed
 * @param line Line number where assertion failed
 */
void __error__(const char *filename, uint32_t line) {
    // Emergency stop - halt CPU
    ESTOP0;
}

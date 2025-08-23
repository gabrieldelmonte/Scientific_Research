/**
 * @file freeRTOS_Tasks.h
 * @brief FreeRTOS task definitions for Buck Converter Control
 * @author Gabriel Del Monte
 * @date 2025
 */

#ifndef FREERTOS_TASKS_H_
#define FREERTOS_TASKS_H_

    #include "peripheral_Setup.h"

    #include "controllers.h"

    #include "Libraries/freeRTOS/FreeRTOS.h"
    #include "Libraries/freeRTOS/task.h"
    #include "Libraries/freeRTOS/semphr.h"

    #include "Libraries/Common/F28x_Project.h"

    #include "Peripheral/includes_FirmWare.h"

    #include "sys/_stdint.h"

    // Task configuration
    #define STACK_SIZE          256

    // Task timing (milliseconds)
    #define TASK1_STARTUP_DELAY 10
    #define TASK1_LOOP_DELAY    4000
    #define TASK1_END_DELAY     1

    #define TASK2_STARTUP_DELAY 10
    #define TASK2_LOOP_DELAY    1
    #define TASK2_END_DELAY     1

    #define TASK3_STARTUP_DELAY 10
    #define TASK3_LOOP_DELAY    1
    #define TASK3_END_DELAY     1

    // Global variables
    extern SemaphoreHandle_t communication_semaphore;
    extern QueueHandle_t control_queue;

    // Function prototypes
    void update_time_task(void *pvParameters);
    void communication_task(void *pvParameters);
    void control_task(void *pvParameters);
    void freeRTOS_Setup(void);

#endif /* FREERTOS_TASKS_H_ */

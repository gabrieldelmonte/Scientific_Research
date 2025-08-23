/**
 * @file main.c
 * @brief Main application file for the F28379D Buck Converter Control System
 * 
 * This project implements a unified control system for a buck converter using either:
 * - PI Controller (traditional control approach)
 * - Neural Network Approximator (NNA) with real-time training
 * 
 * The controller type is selected using the CONTROLLER define below.
 * 
 * @author Gabriel Del Monte
 * @date 2025
 */

#include "peripheral_Setup.h"
#include "freeRTOS_Tasks.h"
#include "controllers.h"

/**
 * @brief Controller selection define
 *        Set to 0 for PI Controller
 *        Set to 1 for Neural Network Approximator (NNA)
 */
#define CONTROLLER 1

/**
 * @brief Main application entry point
 *        Initializes peripherals, configures the selected controller,
 *        and starts the FreeRTOS scheduler.
 */
void main(void) {
    // Initialize all peripheral systems
    peripheral_Setup();

    // Initialize the selected controller
    controller_init(CONTROLLER);

    // Start freeRTOS tasks
    freeRTOS_Setup();
}

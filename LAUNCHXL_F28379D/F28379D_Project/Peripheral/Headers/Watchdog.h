/*
 * Watchdog.h
 *
 *  Created on: 22 de jul de 2022
 *      Author: foste
 */

#ifndef WATCHDOG_H
#define WATCHDOG_H

#include "Peripheral/Headers/defines.h"
#include "Libraries/Common/F28x_Project.h"


    typedef enum WD_operation{
        INTERRUPT = 0,
        RESET
    }WD_operation;


    typedef enum WD_Prescaler{
        WD_PS_1 = 1,
        WD_PS_2,
        WD_PS_4,
        WD_PS_8,
        WD_PS_16,
        WD_PS_32,
        WD_PS_64
    }WD_Prescaler;


    void ConfigWatchdog(WD_operation WD_mode);
    void EnableWatchdog(WD_Prescaler prescaler);

#endif


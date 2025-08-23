/*
 * Interrupts.h
 *
 *  Created on: 22 de jul de 2022
 *      Author: foste
 */

#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "Peripheral/Headers/defines.h"
#include "Libraries/Common/F28x_Project.h"

    /* Defini��o dos grupos na tabela */
        typedef enum INT_grupo{
            grupo_1 = 1,
            grupo_2,
            grupo_3,
            grupo_4,
            grupo_5,
            grupo_6,
            grupo_7,
            grupo_8,
            grupo_9,
            grupo_10,
            grupo_11,
            grupo_12,
        }INT_grupo;

        /* Defini��o das interrup��es na tabela */
        typedef enum INT_periferico{
            interrupt_1 = 0,
            interrupt_2,
            interrupt_3,
            interrupt_4,
            interrupt_5,
            interrupt_6,
            interrupt_7,
            interrupt_8,
            interrupt_9,
            interrupt_10,
            interrupt_11,
            interrupt_12,
            interrupt_13,
            interrupt_14,
            interrupt_15,
            interrupt_16
        }INT_periferico;

    typedef struct Ints{
        INT_grupo grupo;
        INT_periferico periferico;
    }Interrupts;

    typedef struct Int_Vect{
        Interrupts vetor[MAX_INT];
    }Int_Vect;

    void ConfigInterrupt(Int_Vect interrupts);

#endif


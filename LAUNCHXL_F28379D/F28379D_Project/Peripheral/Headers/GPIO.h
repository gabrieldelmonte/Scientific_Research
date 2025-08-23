/*======================================================================================================================
                                                            GPIO                                                        |
                                                        ------------                                                    |
FILE: GPIO.h                                                                                                            |
                                                                                                                        |
    Todas as defini��es necess�rias para a configura��o das GPIOs.                                                      |
    Dependendo do DSP definido o arquivo j� faz as mudan�as necess�rias nas configura��es.                              |
________________________________________________________________________________________________________________________|
                                                                                                                        |
    Fun��es:                                                                                                            |
        GPIO_SetupPinMux:       Configura uma GPIO como um perif�rico espec�fico. [Entrada, Sa�da, PWM, XINT...]        |
        GPIO_SetupPinOptions:   Configura as caracter�sticas de um pino que ser� usado como I/O.                        |
        GPIO_SetupQualCtrl:     Configura o tempo tempo de qualifica��o de um pino usado como entrada.                  |
        ConfigGPIO:             Faz toda a configura��o de um pino que ser� usado como entrada ou sa�da.                |
                                                                                                                        |
________________________________________________________________________________________________________________________|
                                                                                                                        |
                                                                                                   JO�O GABRIEL         |                                                                                                                       |
                                                                                                      08/Julho/2022     |
=======================================================================================================================*/

#ifndef GPIO_H
#define GPIO_H

#include "Peripheral/Headers/defines.h"        //  Necess�rio pela defini��o do DSP sendo usado
#include "Libraries/Common/F28x_Project.h"

	enum{
            ENTRADA = 0,
            SAIDA
        };

        enum{
            PULLUP_DESATIVADO = 0,
            PULLUP_ATIVADO
        };

        enum{
            INV_DESATIVADO = 0,
            INV_ATIVADO
        };

        enum{
            OPENDRAIN_DESATIVADO = 0,
            OPENDRAIN_ATIVADO
        };

        enum{
            SYNC=0,
            QUAL3,
            QUAL6,
            ASYNC
        };

    /* N�o precisa prot�tipo para "GPIO_SetupPinMux" nem "GPIO_SetupPinOptions", pois nesse DSP
      estas fun��es j� s�o definidas nos headers do codeComposer*/

        void GPIO_SetupQualCtrl(Uint16 pin, unsigned char QUALPRD);
        void ConfigGPIO(unsigned char GPIO_number, unsigned char I_O,
                        unsigned char Pullup, unsigned char Inverter_Entrada,
                        unsigned char Open_Drain, unsigned char QSEL, unsigned char QUALPRD);

#endif


/*======================================================================================================================
                                             Analog to Digital Converter                                                |
                                        ------------------------------------                                            |
FILE: ADC.h                                                                                                             |
                                                                                                                        |
    Todas as defini��es necess�rias para a utilizar o perif�rico ADC.                                                   |
    Dependendo do DSP definido o arquivo j� faz as mudan�as necess�rias nas defini��es.                                 |
________________________________________________________________________________________________________________________|
                                                                                                                        |
                                                                                                    JO�O GABRIEL        |                                                                                                                       |
                                                                                                      17/Julho/2022     |
_______________________________________________________________________________________________________________________*/

#ifndef EPWM_H
#define EPWM_H

#include "Libraries/Headers/F2837xD_device.h"
#include "Peripheral/Headers/defines.h"
#include "Libraries/Common/F28x_Project.h"

        typedef enum ePWM_modulos{
            EPWM1 = 1,
            EPWM2,
            EPWM3,
            EPWM4,
            EPWM5,
            EPWM6,
            EPWM7,
            EPWM8,
            EPWM9,
            EPWM10,
            EPWM11,
            EPWM12
        }ePWM_modulos;

        typedef enum ePWM_CLKDIV{
            ePWM_CLKDIV_1 = 0X0,
            ePWM_CLKDIV_2,
            ePWM_CLKDIV_4,
            ePWM_CLKDIV_8,
            ePWM_CLKDIV_16,
            ePWM_CLKDIV_32,
            ePWM_CLKDIV_64,
            ePWM_CLKDIV_128
        }ePWM_CLKDIV;

        typedef enum ePWM_HSPCLKDIV{
            ePWM_HSPCLKDIV_1 = 0X0,
            ePWM_HSPCLKDIV_2,
            ePWM_HSPCLKDIV_4,
            ePWM_HSPCLKDIV_6,
            ePWM_HSPCLKDIV_8,
            ePWM_HSPCLKDIV_10,
            ePWM_HSPCLKDIV_12,
            ePWM_HSPCLKDIV_14
        }ePWM_HSPCLKDIV;


        //---> Defini��o de Macros para facilitar o uso do pwm
        #define EPWM1_Modulante_CMPA EPwm1Regs.CMPA.bit.CMPA
        #define EPWM1_Modulante_CMPB EPwm1Regs.CMPB.bit.CMPB
        #define EPWM2_Modulante_CMPA EPwm2Regs.CMPA.bit.CMPA
        #define EPWM2_Modulante_CMPB EPwm2Regs.CMPB.bit.CMPB
        #define EPWM3_Modulante_CMPA EPwm3Regs.CMPA.bit.CMPA
        #define EPWM3_Modulante_CMPB EPwm3Regs.CMPB.bit.CMPB
        #define EPWM4_Modulante_CMPA EPwm4Regs.CMPA.bit.CMPA
        #define EPWM4_Modulante_CMPB EPwm4Regs.CMPB.bit.CMPB
        #define EPWM5_Modulante_CMPA EPwm5Regs.CMPA.bit.CMPA
        #define EPWM5_Modulante_CMPB EPwm5Regs.CMPB.bit.CMPB
        #define EPWM6_Modulante_CMPA EPwm6Regs.CMPA.bit.CMPA
        #define EPWM6_Modulante_CMPB EPwm6Regs.CMPB.bit.CMPB
        #define EPWM7_Modulante_CMPA EPwm7Regs.CMPA.bit.CMPA
        #define EPWM7_Modulante_CMPB EPwm7Regs.CMPB.bit.CMPB
        #define EPWM8_Modulante_CMPA EPwm8Regs.CMPA.bit.CMPA
        #define EPWM8_Modulante_CMPB EPwm8Regs.CMPB.bit.CMPB
        #define EPWM9_Modulante_CMPA EPwm9Regs.CMPA.bit.CMPA
        #define EPWM9_Modulante_CMPB EPwm9Regs.CMPB.bit.CMPB
        #define EPWM10_Modulante_CMPA EPwm10Regs.CMPA.bit.CMPA
        #define EPWM10_Modulante_CMPB EPwm10Regs.CMPB.bit.CMPB
        #define EPWM11_Modulante_CMPA EPwm11Regs.CMPA.bit.CMPA
        #define EPWM11_Modulante_CMPB EPwm11Regs.CMPB.bit.CMPB
        #define EPWM12_Modulante_CMPA EPwm12Regs.CMPA.bit.CMPA
        #define EPWM12_Modulante_CMPB EPwm12Regs.CMPB.bit.CMPB

        //---Prot�tipos de fun��es
        void ConfigEPwm_REF(ePWM_modulos SAIDA_EPWM, ePWM_HSPCLKDIV CLKDIV1, ePWM_CLKDIV CLKDIV2, Uint16 freq);
        void ConfigPWM_Bipolar(ePWM_modulos EPWM_PERNA1, ePWM_modulos EPWM_PERNA2, ePWM_HSPCLKDIV CLKDIV1, ePWM_CLKDIV CLKDIV2, Uint16 freq);
        void ConfigPWM_Unipolar(ePWM_modulos EPWM_PERNA1, ePWM_modulos EPWM_PERNA2, ePWM_HSPCLKDIV CLKDIV1, ePWM_CLKDIV CLKDIV2, Uint16 freq);

        void StartEPWMConfig(void);
        void EndEPWMConfig(void);
        void ConfigPhasePWM(ePWM_modulos SAIDA_EPWM, Uint16 Fase_graus);
        void ConfigFreqPWM(ePWM_modulos SAIDA_EPWM, Uint16 freq);
        void ConfigDeadBandPWM(ePWM_modulos SAIDA_EPWM, Uint16 tred, Uint16 tfed);
        void AqctlARegConfig(ePWM_modulos SAIDA_EPWM, Uint16 AQ_CAU, Uint16 AQ_CAD, Uint16 AQ_CBU, Uint16 AQ_CBD , Uint16 AQ_ZRO, Uint16 AQ_PRD);
        void AqctlBRegConfig(ePWM_modulos SAIDA_EPWM, Uint16 AQ_CAU, Uint16 AQ_CAD, Uint16 AQ_CBU, Uint16 AQ_CBD , Uint16 AQ_ZRO, Uint16 AQ_PRD);
        void ConfigSyncPWMs(void);

        //

        Uint16 DutyCycle(ePWM_HSPCLKDIV CLKDIV1, ePWM_CLKDIV CLKDIV2, Uint16 freq);

#endif

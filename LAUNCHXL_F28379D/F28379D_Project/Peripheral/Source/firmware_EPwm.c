/*======================================================================================================================
                                                        Enhanced PWM                                                    |
                                                       --------------                                                   |
FILE: firmware_ePWM.c                                                                                                   |
                                                                                                                        |
    Fun��es usadas para configura��o do perif�rico ePWM.                                                                |

    Baseado em:
________________________________________________________________________________________________________________________|
                                                                                                                        |
                                                                                                   JO�O GABRIEL         |
                                                                                                                        |
                                                                                                    19/Julho/2022       |
=======================================================================================================================*/

#include "Libraries/Headers/F2837xD_device.h"
#include "Peripheral/Headers/EPWM.h"

/*---------------------------------------------------------------------------
 * StartEPWMConfig:
 *---------------------------------------------------------------------------
 *        Pausa o clock dos ePWM.
 *        Necess�rio antes de habilitar e sincronizar clocks dos ePWM
 */
void StartEPWMConfig(void){
    EALLOW;                                         //  Permite escrita em registradores protegidos
        CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;       //  Pausa o clock dos ePWM. Necess�rio antes de habilitar e sincronizar clocks dos ePWM
    EDIS;                                           //  Desabilita escrita em registradores protegidos
}


/*---------------------------------------------------------------------------
 * EndEPWMConfig:
 *---------------------------------------------------------------------------
 *        Inicia de forma sincronizada os clocks dos ePWM
 */
void EndEPWMConfig(void){
    EALLOW;                                         //  Permite escrita em registradores protegidos
        CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;       //  Inicia de forma sincronizada os clocks dos ePWM
    EDIS;                                           //  Desabilita escrita em registradores protegidos
}


//
// InitEPwmGpio - Initialize all EPWM modules' GPIOs
//
void InitEPwmGpio(void)
{

//    #define ATIVAR_EPWM1      //->  Conector PWM1
//    #define ATIVAR_EPWM2      //->  Conector PWM1
//    #define ATIVAR_EPWM3      //->  Conector PWM2
//    #define ATIVAR_EPWM4      //->  Conector PWM2
//    #define ATIVAR_EPWM5      //->  Conector PWM6
//    #define ATIVAR_EPWM6      //->  Conector PWM6
//    #define ATIVAR_EPWM7      //->  Conector PWM4
//    #define ATIVAR_EPWM8      //->  Conector PWM4
//    #define ATIVAR_EPWM9      //->  Conector PWM5
//    #define ATIVAR_EPWM10     //->  Conector PWM5
//    #define ATIVAR_EPWM11     //->  Conector PWM3
//    #define ATIVAR_EPWM12     //->  Conector PWM3

#ifdef ATIVAR_EPWM1
    InitEPwm1Gpio();
#endif
#ifdef ATIVAR_EPWM2
    InitEPwm2Gpio();
#endif
#ifdef ATIVAR_EPWM3
    InitEPwm3Gpio();
#endif
#ifdef ATIVAR_EPWM4
    InitEPwm4Gpio();
#endif
#ifdef ATIVAR_EPWM5
    InitEPwm5Gpio();
#endif
#ifdef ATIVAR_EPWM6
    InitEPwm6Gpio();
#endif
#ifdef ATIVAR_EPWM7
    InitEPwm7Gpio();
#endif
#ifdef ATIVAR_EPWM8
    InitEPwm8Gpio();
#endif
#ifdef ATIVAR_EPWM9
    InitEPwm9Gpio();
#endif
#ifdef ATIVAR_EPWM10
    InitEPwm10Gpio();
#endif
#ifdef ATIVAR_EPWM11
    InitEPwm11Gpio();
#endif
#ifdef ATIVAR_EPWM12
    InitEPwm12Gpio();
#endif
}

#ifdef ATIVAR_EPWM1

//
// InitEPwm1Gpio - Initialize EPWM1 GPIOs
//
void InitEPwm1Gpio(void)
{

    CpuSysRegs.PCLKCR2.bit.EPWM1 = 1;       //  Habilita o clock do EPWM1 para habilitar o perif�rico

    EALLOW;

    //
    // Disable internal pull-up for the selected output pins
    // for reduced power consumption
    // Pull-ups can be enabled or disabled by the user.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO0 = 1;    // Disable pull-up on GPIO0 (EPWM1A)
    GpioCtrlRegs.GPAPUD.bit.GPIO1 = 1;    // Disable pull-up on GPIO1 (EPWM1B)
    // GpioCtrlRegs.GPEPUD.bit.GPIO145 = 1;    // Disable pull-up on GPIO145 (EPWM1A)
    // GpioCtrlRegs.GPEPUD.bit.GPIO146 = 1;    // Disable pull-up on GPIO146 (EPWM1B)

    //
    // Configure EPWM-1 pins using GPIO regs
    // This specifies which of the possible GPIO pins will be EPWM1 functional
    // pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;   // Configure GPIO0 as EPWM1A
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;   // Configure GPIO1 as EPWM1B
    // GpioCtrlRegs.GPEMUX2.bit.GPIO145 = 1;   // Configure GPIO145 as EPWM1A
    // GpioCtrlRegs.GPEMUX2.bit.GPIO146 = 1;   // Configure GPIO0146 as EPWM1B

    EDIS;
}
#endif

#ifdef ATIVAR_EPWM2
//
// InitEPwm2Gpio - Initialize EPWM2 GPIOs
//
void InitEPwm2Gpio(void)
{

    CpuSysRegs.PCLKCR2.bit.EPWM2 = 1;       //  Habilita o clock do EPWM2 para habilitar o perif�rico

    EALLOW;

    //
    // Disable internal pull-up for the selected output pins
    // for reduced power consumption
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO2 = 1;    // Disable pull-up on GPIO2 (EPWM2A)
    GpioCtrlRegs.GPAPUD.bit.GPIO3 = 1;    // Disable pull-up on GPIO3 (EPWM2B)
    // GpioCtrlRegs.GPEPUD.bit.GPIO147 = 1;    // Disable pull-up on GPIO147 (EPWM2A)
    // GpioCtrlRegs.GPEPUD.bit.GPIO148 = 1;    // Disable pull-up on GPIO148 (EPWM2B)

    //
    // Configure EPwm-2 pins using GPIO regs
    // This specifies which of the possible GPIO pins will be EPWM2 functional pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;   // Configure GPIO2 as EPWM2A
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;   // Configure GPIO3 as EPWM2B
   // GpioCtrlRegs.GPEMUX2.bit.GPIO147 = 1;   // Configure GPIO147 as EPWM2A
   // GpioCtrlRegs.GPEMUX2.bit.GPIO148 = 1;   // Configure GPIO148 as EPWM2B

    EDIS;
}
#endif
//
#ifdef ATIVAR_EPWM3
//
// InitEPwm3Gpio - Initialize EPWM3 GPIOs
//
void InitEPwm3Gpio(void)
{

    CpuSysRegs.PCLKCR2.bit.EPWM3 = 1;       //  Habilita o clock do EPWM3 para habilitar o perif�rico

    EALLOW;

    //
    // Disable internal pull-up for the selected output pins
    //   for reduced power consumption
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO4 = 1;    // Disable pull-up on GPIO4 (EPWM3A)
    GpioCtrlRegs.GPAPUD.bit.GPIO5 = 1;    // Disable pull-up on GPIO5 (EPWM3B)
    // GpioCtrlRegs.GPEPUD.bit.GPIO149 = 1;    // Disable pull-up on GPIO149 (EPWM3A)
    // GpioCtrlRegs.GPEPUD.bit.GPIO150 = 1;    // Disable pull-up on GPIO150 (EPWM3B)

    //
    // Configure EPwm-3 pins using GPIO regs
    // This specifies which of the possible GPIO pins will be EPWM3 functional pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;   // Configure GPIO4 as EPWM3A
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;   // Configure GPIO5 as EPWM3B
    // GpioCtrlRegs.GPEMUX2.bit.GPIO149 = 1;   // Configure GPIO149 as EPWM3A
    // GpioCtrlRegs.GPEMUX2.bit.GPIO150 = 1;   // Configure GPIO150 as EPWM3B

    EDIS;
}
#endif
//
#ifdef ATIVAR_EPWM4
//
// InitEPwm4Gpio - Initialize EPWM4 GPIOs
//
void InitEPwm4Gpio(void)
{

    CpuSysRegs.PCLKCR2.bit.EPWM4 = 1;       //  Habilita o clock do EPWM4 para habilitar o perif�rico

    EALLOW;

    //
    // Disable internal pull-up for the selected output pins
    //   for reduced power consumption
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO6 = 1;    // Disable pull-up on GPIO6 (EPWM4A)
    GpioCtrlRegs.GPAPUD.bit.GPIO7 = 1;    // Disable pull-up on GPIO7 (EPWM4B)
    // GpioCtrlRegs.GPEPUD.bit.GPIO151 = 1;    // Disable pull-up on GPIO151 (EPWM4A)
    // GpioCtrlRegs.GPEPUD.bit.GPIO152 = 1;    // Disable pull-up on GPIO152 (EPWM4B)

     //
     // Configure EPWM-4 pins using GPIO regs
     // This specifies which of the possible GPIO pins will be EPWM4 functional
     // pins.
     // Comment out other unwanted lines.
     //
    GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 1;   // Configure GPIO6 as EPWM4A
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 1;   // Configure GPIO7 as EPWM4B
    // GpioCtrlRegs.GPEMUX2.bit.GPIO151 = 1;   // Configure GPIO151 as EPWM4A
    // GpioCtrlRegs.GPEMUX2.bit.GPIO152 = 1;   // Configure GPIO152 as EPWM4B

    EDIS;
}
#endif

#ifdef ATIVAR_EPWM5
//
// InitEPwm5Gpio - Initialize EPWM5 GPIOs
//
void InitEPwm5Gpio(void)
{

    CpuSysRegs.PCLKCR2.bit.EPWM5 = 1;       //  Habilita o clock do EPWM5 para habilitar o perif�rico

    EALLOW;
    //
    // Disable internal pull-up for the selected output pins
    //   for reduced power consumption
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO8 = 1;    // Disable pull-up on GPIO8 (EPWM5A)
    GpioCtrlRegs.GPAPUD.bit.GPIO9 = 1;    // Disable pull-up on GPIO9 (EPWM5B)
    // GpioCtrlRegs.GPEPUD.bit.GPIO153 = 1;    // Disable pull-up on GPIO153 (EPWM5A)
    // GpioCtrlRegs.GPEPUD.bit.GPIO154 = 1;    // Disable pull-up on GPIO154 (EPWM5B)

    //
    // Configure EPWM-5 pins using GPIO regs
    // This specifies which of the possible GPIO pins will be EPWM5 functional
    // pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 1;   // Configure GPIO8 as EPWM5A
    GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 1;   // Configure GPIO9 as EPWM5B
    // GpioCtrlRegs.GPEMUX2.bit.GPIO153 = 1;   // Configure GPIO153 as EPWM5A
    // GpioCtrlRegs.GPEMUX2.bit.GPIO154 = 1;   // Configure GPIO0154 as EPWM5B

    EDIS;
}
#endif

#ifdef ATIVAR_EPWM6
//
// InitEPwm6Gpio - Initialize EPWM6 GPIOs
//
void InitEPwm6Gpio(void)
{

    CpuSysRegs.PCLKCR2.bit.EPWM6 = 1;       //  Habilita o clock do EPWM6 para habilitar o perif�rico

    EALLOW;
    //
    // Disable internal pull-up for the selected output pins
    // for reduced power consumption
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO10 = 1;    // Disable pull-up on GPIO10 (EPWM6A)
    GpioCtrlRegs.GPAPUD.bit.GPIO11 = 1;    // Disable pull-up on GPIO11 (EPWM6B)
    // GpioCtrlRegs.GPEPUD.bit.GPIO155 = 1;    // Disable pull-up on GPIO155 (EPWM6A)
    // GpioCtrlRegs.GPEPUD.bit.GPIO156 = 1;    // Disable pull-up on GPIO156 (EPWM6B)

    //
    // Configure EPWM-6 pins using GPIO regs
    // This specifies which of the possible GPIO pins will be EPWM6 functional
    // pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 1;   // Configure GPIO10 as EPWM6A
    GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 1;   // Configure GPIO11 as EPWM6B
    // GpioCtrlRegs.GPEMUX2.bit.GPIO155 = 1;   // Configure GPIO155 as EPWM6A
    // GpioCtrlRegs.GPEMUX2.bit.GPIO156 = 1;   // Configure GPIO156 as EPWM6B

    EDIS;
}
#endif

#ifdef ATIVAR_EPWM7
//
// InitEPwm7Gpio - Initialize EPWM7 GPIOs
//
void InitEPwm7Gpio(void)
{

    CpuSysRegs.PCLKCR2.bit.EPWM7 = 1;       //  Habilita o clock do EPWM7 para habilitar o perif�rico

    EALLOW;

    //
    // Disable internal pull-up for the selected output pins
    // for reduced power consumption
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO12 = 1;    // Disable pull-up on GPIO12 (EPWM7A)
    GpioCtrlRegs.GPAPUD.bit.GPIO13 = 1;    // Disable pull-up on GPIO13 (EPWM7B)
    // GpioCtrlRegs.GPEPUD.bit.GPIO157 = 1;    // Disable pull-up on GPIO157 (EPWM7A)
    // GpioCtrlRegs.GPEPUD.bit.GPIO158 = 1;    // Disable pull-up on GPIO158 (EPWM7B)

    //
    // Configure EPWM-6 pins using GPIO regs
    // This specifies which of the possible GPIO pins will be EPWM6 functional
    // pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 1;   // Configure GPIO12 as EPWM7A
    GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 1;   // Configure GPIO13 as EPWM7B
    // GpioCtrlRegs.GPEMUX2.bit.GPIO157 = 1;   // Configure GPIO157 as EPWM7A
    // GpioCtrlRegs.GPEMUX2.bit.GPIO158 = 1;   // Configure GPIO158 as EPWM7B

    EDIS;
}
#endif

#ifdef ATIVAR_EPWM8
//
// InitEPwm8Gpio - Initialize EPWM8 GPIOs
//
void InitEPwm8Gpio(void)
{

    CpuSysRegs.PCLKCR2.bit.EPWM8 = 1;       //  Habilita o clock do EPWM8 para habilitar o perif�rico

    EALLOW;
    //
    // Disable internal pull-up for the selected output pins
    // for reduced power consumption
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO14 = 1;    // Disable pull-up on GPIO14 (EPWM8A)
    GpioCtrlRegs.GPAPUD.bit.GPIO15 = 1;    // Disable pull-up on GPIO15 (EPWM8B)
//  GpioCtrlRegs.GPEPUD.bit.GPIO159 = 1;    // Disable pull-up on GPIO159 (EPWM8A)
//  GpioCtrlRegs.GPFPUD.bit.GPIO160 = 1;    // Disable pull-up on GPIO160 (EPWM8B)

     //
     // Configure EPWM-6 pins using GPIO regs
     // This specifies which of the possible GPIO pins will be EPWM6 functional
     // pins.
     // Comment out other unwanted lines.
     //
    GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 1;   // Configure GPIO14 as EPWM8A
    GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 1;   // Configure GPIO15 as EPWM8B
    // GpioCtrlRegs.GPEMUX2.bit.GPIO159 = 1;   // Configure GPIO159 as EPWM8A
    // GpioCtrlRegs.GPFMUX1.bit.GPIO160 = 1;   // Configure GPIO160 as EPWM8B

    EDIS;
}
#endif

#ifdef ATIVAR_EPWM9
//
// InitEPwm9Gpio - Initialize EPWM9 GPIOs
//
void InitEPwm9Gpio(void)
{

    CpuSysRegs.PCLKCR2.bit.EPWM9 = 1;       //  Habilita o clock do EPWM9 para habilitar o perif�rico

    EALLOW;
    //
    // Disable internal pull-up for the selected output pins
    // for reduced power consumption
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO16 = 1;    // Disable pull-up on GPIO16 (EPWM9A)
    GpioCtrlRegs.GPAPUD.bit.GPIO17 = 1;    // Disable pull-up on GPIO17 (EPWM9B)
//    GpioCtrlRegs.GPFPUD.bit.GPIO161 = 1;    // Disable pull-up on GPIO161 (EPWM9A)
//    GpioCtrlRegs.GPFPUD.bit.GPIO162 = 1;    // Disable pull-up on GPIO162 (EPWM9B)

    //
    // Configure EPWM-9 pins using GPIO regs
    // This specifies which of the possible GPIO pins will be EPWM9 functional
    // pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAGMUX2.bit.GPIO16 = 1;   //| Configure GPIO16 as EPWM9A
    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 1;    //|
    GpioCtrlRegs.GPAGMUX2.bit.GPIO17 = 1;   //| Configure GPIO17 as EPWM9B
    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 1;    //|
//    GpioCtrlRegs.GPFMUX1.bit.GPIO161 = 1;   // Configure GPIO161 as EPWM9A
//    GpioCtrlRegs.GPFMUX1.bit.GPIO162 = 1;   // Configure GPIO162 as EPWM9B

    EDIS;
}
#endif

#ifdef ATIVAR_EPWM10
//
// InitEPwm10Gpio - Initialize EPWM10 GPIOs
//
void InitEPwm10Gpio(void)
{

    CpuSysRegs.PCLKCR2.bit.EPWM10 = 1;       //  Habilita o clock do EPWM10 para habilitar o perif�rico

    EALLOW;
    //
    // Disable internal pull-up for the selected output pins
    // for reduced power consumption
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO18 = 1;    // Disable pull-up on GPIO18 (EPWM10A)
    GpioCtrlRegs.GPAPUD.bit.GPIO19 = 1;    // Disable pull-up on GPIO19 (EPWM10B)
//    GpioCtrlRegs.GPFPUD.bit.GPIO163 = 1;    // Disable pull-up on GPIO163 (EPWM10A)
//    GpioCtrlRegs.GPFPUD.bit.GPIO164 = 1;    // Disable pull-up on GPIO164 (EPWM10B)

    //
    // Configure EPWM-10 pins using GPIO regs
    // This specifies which of the possible GPIO pins will be EPWM10 functional
    // pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAGMUX2.bit.GPIO18 = 1;   //| Configure GPIO18 as EPWM10A
    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 1;    //|
    GpioCtrlRegs.GPAGMUX2.bit.GPIO19 = 1;   //| Configure GPIO19 as EPWM10B
    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 1;    //|
//    GpioCtrlRegs.GPFMUX1.bit.GPIO163 = 1;   // Configure GPIO163 as EPWM10A
//    GpioCtrlRegs.GPFMUX1.bit.GPIO164 = 1;   // Configure GPIO164 as EPWM10B

    EDIS;
}
#endif

#ifdef ATIVAR_EPWM11
//
// InitEPwm11Gpio - Initialize EPWM11 GPIOs
//
void InitEPwm11Gpio(void)
{

    CpuSysRegs.PCLKCR2.bit.EPWM11 = 1;       //  Habilita o clock do EPWM11 para habilitar o perif�rico

    EALLOW;
    //
    // Disable internal pull-up for the selected output pins
    // for reduced power consumption
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO20 = 1;    // Disable pull-up on GPIO20 (EPWM11A)
    GpioCtrlRegs.GPAPUD.bit.GPIO21 = 1;    // Disable pull-up on GPIO21 (EPWM11B)
//    GpioCtrlRegs.GPFPUD.bit.GPIO165 = 1;    // Disable pull-up on GPIO165 (EPWM11A)
//    GpioCtrlRegs.GPFPUD.bit.GPIO166 = 1;    // Disable pull-up on GPIO166 (EPWM11B)

    //
    // Configure EPWM-11 pins using GPIO regs
    // This specifies which of the possible GPIO pins will be EPWM11 functional
    // pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAGMUX2.bit.GPIO20 = 1;   //| Configure GPIO20 as EPWM11A
    GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 1;    //|
    GpioCtrlRegs.GPAGMUX2.bit.GPIO21 = 1;   //| Configure GPIO21 as EPWM11B
    GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 1;    //|
//    GpioCtrlRegs.GPFMUX1.bit.GPIO165 = 1;   // Configure GPIO165 as EPWM11A
//    GpioCtrlRegs.GPFMUX1.bit.GPIO166 = 1;   // Configure GPIO166 as EPWM11B

    EDIS;
}
#endif

#ifdef ATIVAR_EPWM12
//
// InitEPwm12Gpio - Initialize EPWM12 GPIOs
//
void InitEPwm12Gpio(void)
{

    CpuSysRegs.PCLKCR2.bit.EPWM12 = 1;       //  Habilita o clock do EPWM12 para habilitar o perif�rico

    EALLOW;
    //
    // Disable internal pull-up for the selected output pins
    // for reduced power consumption
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO22 = 1;    // Disable pull-up on GPIO22 (EPWM12A)
    GpioCtrlRegs.GPAPUD.bit.GPIO23 = 1;    // Disable pull-up on GPIO23 (EPWM12B)
//    GpioCtrlRegs.GPFPUD.bit.GPIO167 = 1;    // Disable pull-up on GPIO167 (EPWM12A)
//    GpioCtrlRegs.GPFPUD.bit.GPIO168 = 1;    // Disable pull-up on GPIO168 (EPWM12B)

    //
    // Configure EPWM-6 pins using GPIO regs
    // This specifies which of the possible GPIO pins will be EPWM6 functional
    // pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAGMUX2.bit.GPIO22 = 1;   //| Configure GPIO22 as EPWM12A
    GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 1;    //|
    GpioCtrlRegs.GPAGMUX2.bit.GPIO23 = 1;   //| Configure GPIO23 as EPWM12B
    GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 1;    //|
//    GpioCtrlRegs.GPFMUX1.bit.GPIO167 = 1;   // Configure GPIO167 as EPWM12A
//    GpioCtrlRegs.GPFMUX1.bit.GPIO168 = 1;   // Configure GPIO168 as EPWM12B

    EDIS;
}
#endif


/*-------------------------------------------------------------------------------------------------
                                     Fun��es espec�ficas
                                    --------------------

    Fun��es criadas para facilitar a configura��o do ePWM.
-------------------------------------------------------------------------------------------------*/

volatile struct EPWM_REGS* EPWM_PTR[13] = {0x0, &EPwm1Regs, &EPwm2Regs, &EPwm3Regs, &EPwm4Regs, &EPwm5Regs, &EPwm6Regs,
                                           &EPwm7Regs, &EPwm8Regs, &EPwm9Regs, &EPwm10Regs, &EPwm11Regs, &EPwm12Regs};

/*---------------------------------------------------------------------------
 * ConfigEPwm_REF:
 *---------------------------------------------------------------------------
 *       Configura um dos PWM para atuar como uma sa�da para visualizar as refer�ncias
 *   ou outras grandezas no oscilosc�pio.
 *       Por padr�o:
 *
 *            Modo de contagem:   UP_DOWN
 *            Sincroniza��o:      Desabilitada
 *            Dead-band:          Desabilitado
 *        Par�metros:
 *          Uint16 CLKDIV1:           Valor colocado em HSPCLKDIV  [0, 1, 2, 3, 4, 5, 6, 7]   //|     Arquivo 'EPWM_regs.c' mostra o
 *          Uint16 CLKDIV2:           Valor colocado em CLKDIV     [0, 1, 2, 3, 4, 5, 6, 7]   //| que cada valor representa.
 *          Uint16 TBPRD_VALUE:       Valor que ser� colocado em TBPRD, calculado como indicado abaixo
 *
 *---------------- DEFINI��O DA FREQU�NCIA DOS M�DULOS ePWM ----------------
 *
 *      EPWMCLK = SYSCLK / 2 = 100MHz -> Inicializa��o padr�o
 *
 *      TBCLK = EPWMCLK / (HSPCLKDIV * CLKDIV) = 100MHz / (HSPCLKDIV_DEFAULT * CLKDIV_DEFAULT)
 *
 *  Para CTRMODE = TB_COUNT_UP ou CTRMODE = TB_COUNT_DOWN:
 *
 *      TBPRD = (TBCLK / fpwm) - 1
 *
 *  Para CTRMODE = TB_COUNT_UPDOWN:
 *
 *      TBPRD = (0.5) * (TBCLK / fpwm)
 */
void ConfigEPwm_REF(ePWM_modulos SAIDA_EPWM, ePWM_HSPCLKDIV CLKDIV1, ePWM_CLKDIV CLKDIV2, Uint16 freq){
    Uint16 cont_max;
    Uint32 tbclk;
    Uint16 HSPCLKDIV_TAB[8] = {1, 2, 4, 6, 8, 10, 12, 14};
    Uint16 CLKDIV_TAB[8] = {1, 2, 4, 8, 16, 32, 64, 128};

    tbclk = (Uint32) (100000000 / (HSPCLKDIV_TAB[CLKDIV1] * CLKDIV_TAB[CLKDIV2]));

    cont_max = 0.5 * (tbclk / freq);

    //**************************** TBCTL - Time Base Control Register ****************************//

    EPWM_PTR[SAIDA_EPWM]->TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;                      //  Portadora triangular

    EPWM_PTR[SAIDA_EPWM]->TBCTL.bit.PHSEN = TB_DISABLE;                             //  N�o recebe sincroniza��o externa
    EPWM_PTR[SAIDA_EPWM]->TBCTL.bit.SYNCOSEL = TB_SYNC_IN;                          //  SYNC_OUT Conectado a Sync_in para passar adiante qualquer sinal de sincronia
    EPWM_PTR[SAIDA_EPWM]->TBCTL.bit.PRDLD = TB_SHADOW;                              //  TBPRD � carregado a partir de seu registrador shadow
//    EPWM_PTR[SAIDA_EPWM]->TBCTL.bit.SWFSYNC = 0x0;                                  //  'Software Forced Sync Pulse' -> Usado para gerar sinal de sincronia quando .TBCTL.bit.SYNCOSEL==0
    EPWM_PTR[SAIDA_EPWM]->TBCTL.bit.HSPCLKDIV = CLKDIV1;                            //  Determina parte do pre-scale do clock
    EPWM_PTR[SAIDA_EPWM]->TBCTL.bit.CLKDIV = CLKDIV2;                               //  Segunda parte do pre-scale do clock
    EPWM_PTR[SAIDA_EPWM]->TBCTL.bit.PHSDIR = TB_DOWN;                               //  Determina a dire��o da contagem ap�s sinal  de sincronia, apenas para .TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN
    EPWM_PTR[SAIDA_EPWM]->TBCTL.bit.FREE_SOFT = 0x0;                                //  Durante emula��o a contagem para no pr�ximo incremento ap�s um evento



    // **************************** TBCTL2 - Time Base Control Register2 ****************************//

    EPWM_PTR[SAIDA_EPWM]->TBCTL2.bit.OSHTSYNCMODE = TB_DISABLE;         //  N�o � usado oneshot sync
//    EPWM_PTR[SAIDA_EPWM]->TBCTL2.bit.OSHTSYNC = TB_DISABLE;           //  N�o � usado oneshot sync (Escrever 0 n�o tem efeito nenhum)
//    EPWM_PTR[SAIDA_EPWM]->TBCTL2.bit.SYNCOSELX = TB_DISABLE;          //  N�o faz diferen�a pois TBCTL[SYNCOSEL] != 11
    EPWM_PTR[SAIDA_EPWM]->TBCTL2.bit.PRDLDSYNC = TB_DISABLE;            //  Valor de TBPRD � carregado do registrador shadow quando CTR=0



    //**************************** TBCTR - Time Base Counter Register ****************************//

    EPWM_PTR[SAIDA_EPWM]->TBCTR = 0x000;                                            //  Reseta o contador



    // **************************** TBSTS - Time Base Status Register ****************************//

     // Nenhum bit de TBSTS precisa ser configurado



    // **************************** CMPCTL - Counter Compare Control Register ****************************//

    EPWM_PTR[SAIDA_EPWM]->CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;               //  Carrega CMPA do registrador shadow quando CTR = 0
    EPWM_PTR[SAIDA_EPWM]->CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;               //  Carrega CMPB do registrador shadow quando CTR = 0
    EPWM_PTR[SAIDA_EPWM]->CMPCTL.bit.SHDWAMODE = CC_SHADOW;                 //  Registrador shadow ativado
    EPWM_PTR[SAIDA_EPWM]->CMPCTL.bit.SHDWBMODE = CC_SHADOW;                 //  Registrador shadow ativado
    EPWM_PTR[SAIDA_EPWM]->CMPCTL.bit.LOADASYNC = 0x00;                      //  Carregamento do registrador shadow de acordo com CMPCTL[LOADAMODE]
    EPWM_PTR[SAIDA_EPWM]->CMPCTL.bit.LOADBSYNC = 0x00;                      //  Carregamento do registrador shadow de acordo com CMPCTL[LOADBMODE]


    //*************************** DBCTL - Dead-Band Generator Control Register ***************************//

    //  Configura��o feita para atuar com uma ponte-H COM delay por hardware
    EPWM_PTR[SAIDA_EPWM]->DBCTL.bit.OUT_MODE = DB_DISABLE;                          //|  Desabilita o subm�dulo de dead-band, dessa forma a �nica
                                                                                    //| outra configura��o necess�ria � de OUTSWAP
    EPWM_PTR[SAIDA_EPWM]->DBCTL.bit.OUTSWAP = 0;                                    //  A sa�da � como definida por OUT_MODE



    //************************** AQCTL - Action Qualifier Control Register **************************//

    EPWM_PTR[SAIDA_EPWM]->AQCTL.bit.SHDWAQAMODE = 0;                        //  Modo imediato
    EPWM_PTR[SAIDA_EPWM]->AQCTL.bit.SHDWAQBMODE = 0;                        //  Modo imediato



    //***************************** AQCTLA - Action Qualifier Control Register for Output A *****************************//

    EPWM_PTR[SAIDA_EPWM]->AQCTLA.bit.ZRO = AQ_NO_ACTION;    //  Nenhuma a��o
    EPWM_PTR[SAIDA_EPWM]->AQCTLA.bit.PRD = AQ_NO_ACTION;    //  Nenhuma a��o
    EPWM_PTR[SAIDA_EPWM]->AQCTLA.bit.CAU = AQ_CLEAR;        //  Sa�da vai para LOW quando a modulante de CMPA for menor que a portadora
    EPWM_PTR[SAIDA_EPWM]->AQCTLA.bit.CAD = AQ_SET;          //  Sa�da vai para HIGH quando a modulante de CMPA for maior que a portadora
    EPWM_PTR[SAIDA_EPWM]->AQCTLA.bit.CBU = AQ_NO_ACTION;    //  Nenhuma a��o
    EPWM_PTR[SAIDA_EPWM]->AQCTLA.bit.CBD = AQ_NO_ACTION;    //  Nenhuma a��o



    //***************************** AQCTLB - Action Qualifier Control Register for Output B *****************************//

    EPWM_PTR[SAIDA_EPWM]->AQCTLB.bit.ZRO = AQ_NO_ACTION;    //  Nenhuma a��o
    EPWM_PTR[SAIDA_EPWM]->AQCTLB.bit.PRD = AQ_NO_ACTION;    //  Nenhuma a��o
    EPWM_PTR[SAIDA_EPWM]->AQCTLB.bit.CAU = AQ_SET;          //  Sa�da vai para HIGH quando a modulante de CMPA for menor que a portadora
    EPWM_PTR[SAIDA_EPWM]->AQCTLB.bit.CAD = AQ_CLEAR;        //  Sa�da vai para LOW quando a modulante de CMPA for maior que a portadora
    EPWM_PTR[SAIDA_EPWM]->AQCTLB.bit.CBU = AQ_NO_ACTION;    //  Nenhuma a��o
    EPWM_PTR[SAIDA_EPWM]->AQCTLB.bit.CBD = AQ_NO_ACTION;    //  Nenhuma a��o



    //****************************** TBPHS - Time Base Phase High ******************************//

    EPWM_PTR[SAIDA_EPWM]->TBPHS.bit.TBPHS = 0;              //  S� para inicializar TBPHS



    //****************************** TBPRD - Time Base Period Register ******************************//

    EPWM_PTR[SAIDA_EPWM]->TBPRD = cont_max;                 //  Configura o per�odo da portadora



    //****************************** CMPA - Counter Compare A Register ******************************//

    EPWM_PTR[SAIDA_EPWM]->CMPA.bit.CMPA = 0x0000;           //  Inicializa CMPA



    //****************************** CMPB - Counter Compare B Register ******************************//

    EPWM_PTR[SAIDA_EPWM]->CMPB.bit.CMPB = 0x0000;           //  Inicializa CMPB



    //****************************** CMPC - Counter Compare B Register ******************************//

    EPWM_PTR[SAIDA_EPWM]->CMPC = 0x0000;                    //  Inicializa CMPC



    //****************************** CMPD - Counter Compare D Register ******************************//

    EPWM_PTR[SAIDA_EPWM]->CMPD = 0x0000;                    //  Inicializa CMPD

}


/*---------------------------------------------------------------------------
 * ConfigPWM_Bipolar:
 *---------------------------------------------------------------------------
 *        Configura dois m�dulos PWM para operarem com modula��o bipolar.
 */
void ConfigPWM_Bipolar(ePWM_modulos EPWM_PERNA1, ePWM_modulos EPWM_PERNA2, ePWM_HSPCLKDIV CLKDIV1, ePWM_CLKDIV CLKDIV2, Uint16 freq){

    ConfigEPwm_REF(EPWM_PERNA1, CLKDIV1, CLKDIV2, freq);         //  Faz a configura��o padr�o para o PWM usado na perna 1 da ponte
    ConfigEPwm_REF(EPWM_PERNA2, CLKDIV1, CLKDIV2, freq);         //  Faz a configura��o padr�o para o PWM usado na perna 2 da ponte

    AqctlARegConfig(EPWM_PERNA1, AQ_CLEAR, AQ_SET, AQ_NO_ACTION, AQ_NO_ACTION, AQ_NO_ACTION, AQ_NO_ACTION);     //|     Os qualifiers definem a a��o tomada quando a portadora passa pela modulante.
    AqctlBRegConfig(EPWM_PERNA1, AQ_SET, AQ_CLEAR, AQ_NO_ACTION, AQ_NO_ACTION, AQ_NO_ACTION, AQ_NO_ACTION);     //|
                                                                                                                //| Os sinais da perna 1 e da perna 2 s�o complementares entre si, ent�o:
    AqctlARegConfig(EPWM_PERNA2, AQ_SET, AQ_CLEAR, AQ_NO_ACTION, AQ_NO_ACTION, AQ_NO_ACTION, AQ_NO_ACTION);     //|
    AqctlBRegConfig(EPWM_PERNA2, AQ_CLEAR, AQ_SET, AQ_NO_ACTION, AQ_NO_ACTION, AQ_NO_ACTION, AQ_NO_ACTION);     //|   EPWM1A = EPWM2B     e     EPWM1B = EPWM2A
}


/*---------------------------------------------------------------------------
 * ConfigPWM_Unipolar:
 *---------------------------------------------------------------------------
 *        Configura dois m�dulos PWM para operarem com modula��o unipolar.
 */
void ConfigPWM_Unipolar(ePWM_modulos EPWM_PERNA1, ePWM_modulos EPWM_PERNA2, ePWM_HSPCLKDIV CLKDIV1, ePWM_CLKDIV CLKDIV2, Uint16 freq){

    ConfigPWM_Bipolar(EPWM_PERNA1, EPWM_PERNA2, CLKDIV1, CLKDIV2, freq);         //  Faz uma configura��o padr�o igual do bipolar

    ConfigPhasePWM(EPWM_PERNA2, 180);                                                   //  Coloca o valor referente a 180 graus de defasagem em TBPHS
}


/*---------------------------------------------------------------------------
 * ConfigPhasePWM:
 *---------------------------------------------------------------------------
 *        Configura a fase de um m�dulo ePWM.
 */
void ConfigPhasePWM(ePWM_modulos SAIDA_EPWM, Uint16 Fase_graus){

    Uint16 Phase_count;

    if (EPWM_PTR[SAIDA_EPWM]->TBCTL.bit.CTRMODE == TB_COUNT_UPDOWN){
        Phase_count = (Uint16) EPWM_PTR[SAIDA_EPWM]->TBPRD  * (Fase_graus / 180);
    }else{
        Phase_count = (Uint16) EPWM_PTR[SAIDA_EPWM]->TBPRD * Fase_graus * ( Fase_graus / 360);
    }

    EPWM_PTR[SAIDA_EPWM]->TBPHS.bit.TBPHS = Phase_count;
}


/*---------------------------------------------------------------------------
 * ConfigFreqPWM:
 *---------------------------------------------------------------------------
 *        Configura a frequ�ncia de um m�dulo ePWM.
*---------------- DEFINI��O DA FREQU�NCIA DOS M�DULOS ePWM ----------------
*
*      TBCLK = EPWMCLK / (HSPCLKDIV * CLKDIV) = 150MHz / (HSPCLKDIV_DEFAULT * CLKDIV_DEFAULT)
*
*  Para CTRMODE = TB_COUNT_UP ou CTRMODE = TB_COUNT_DOWN:
*
*      TBPRD = (TBCLK / fpwm) - 1
*
*  Para CTRMODE = TB_COUNT_UPDOWN:
*
*      TBPRD = (0.5) * (TBCLK / fpwm)
*/
void ConfigFreqPWM(ePWM_modulos SAIDA_EPWM, Uint16 freq){
    Uint16 cont_max;
    Uint32 tbclk;
    Uint16 CLKDIV_TAB[8] = {1, 2, 4, 8, 16, 32, 64, 128};
    Uint16 HSPCLKDIV_TAB[8] = {1, 2, 4, 6, 8, 10, 12, 14};

    tbclk = (Uint32) (100000000 / (HSPCLKDIV_TAB[EPWM_PTR[SAIDA_EPWM]->TBCTL.bit.HSPCLKDIV] * CLKDIV_TAB[EPWM_PTR[SAIDA_EPWM]->TBCTL.bit.CLKDIV]));
    if (EPWM_PTR[SAIDA_EPWM]->TBCTL.bit.CTRMODE == TB_COUNT_UPDOWN){
        cont_max = 0.5 * (tbclk / freq);
    }else{
        cont_max = (tbclk / freq) - 1;
    }

    EPWM_PTR[SAIDA_EPWM]->TBPRD = (Uint16) cont_max;
}


/*---------------------------------------------------------------------------
 * ConfigDeadBandPWM:
 *---------------------------------------------------------------------------
 *        Configura o dead band de um m�dulo PWM.
 *
 *  Par�metros:
 *      ePWM_modulos SAIDA_EPWM:    M�dulo ePWM sendo configurado
 *      Uint16 tred:                Tempo em us de "Rising Edge Delay"
 *      Uint16 tfed:                Tempo em us de "Falling Edge Delay"
 *
 *        Tred = DBRED * TBclock
 *        Tfed = DBFED * TBclock
 *
 */
void ConfigDeadBandPWM(ePWM_modulos SAIDA_EPWM, Uint16 tred, Uint16 tfed){
    float32 tbclk, T;
    Uint16 dbred, dbfed;
    Uint16 CLKDIV_TAB[8] = {1, 2, 4, 8, 16, 32, 64, 128};
    Uint16 HSPCLKDIV_TAB[8] = {1, 2, 4, 6, 8, 10, 12, 14};

    tbclk = (100 / (HSPCLKDIV_TAB[EPWM_PTR[SAIDA_EPWM]->TBCTL.bit.HSPCLKDIV] * CLKDIV_TAB[EPWM_PTR[SAIDA_EPWM]->TBCTL.bit.CLKDIV]));
    T = 1 / tbclk;

    dbred = (Uint16) tred / T;
    dbfed = (Uint16) tfed / T;

    EPWM_PTR[SAIDA_EPWM]->DBRED.bit.DBRED = dbred;
    EPWM_PTR[SAIDA_EPWM]->DBFED.bit.DBFED = dbfed;
    EPWM_PTR[SAIDA_EPWM]->DBCTL.bit.IN_MODE = DBA_ALL;
    EPWM_PTR[SAIDA_EPWM]->DBCTL.bit.POLSEL = DBA_RED_DBB_FED;
    EPWM_PTR[SAIDA_EPWM]->DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
}
void AqctlARegConfig(ePWM_modulos SAIDA_EPWM, Uint16 AQ_CAU, Uint16 AQ_CAD, Uint16 AQ_CBU, Uint16 AQ_CBD , Uint16 AQ_ZRO, Uint16 AQ_PRD){

    EPWM_PTR[SAIDA_EPWM]->AQCTLA.bit.ZRO = AQ_ZRO;    //  A��o tomada quando CTR = 0
    EPWM_PTR[SAIDA_EPWM]->AQCTLA.bit.PRD = AQ_PRD;    //  A��o tomada quando CTR = TBPRD
    EPWM_PTR[SAIDA_EPWM]->AQCTLA.bit.CAU = AQ_CAU;    //  A��o tomada quando a modulante de CMPA for menor que a portadora
    EPWM_PTR[SAIDA_EPWM]->AQCTLA.bit.CAD = AQ_CAD;    //  A��o tomada quando a modulante de CMPA for maior que a portadora
    EPWM_PTR[SAIDA_EPWM]->AQCTLA.bit.CBU = AQ_CBU;    //  A��o tomada quando a modulante de CMPB for menor que a portadora
    EPWM_PTR[SAIDA_EPWM]->AQCTLA.bit.CBD = AQ_CBD;    //  A��o tomada quando a modulante de CMPB for maior que a portadora

}

void AqctlBRegConfig(ePWM_modulos SAIDA_EPWM, Uint16 AQ_CAU, Uint16 AQ_CAD, Uint16 AQ_CBU, Uint16 AQ_CBD , Uint16 AQ_ZRO, Uint16 AQ_PRD){

    EPWM_PTR[SAIDA_EPWM]->AQCTLB.bit.ZRO = AQ_ZRO;    //  A��o tomada quando CTR = 0
    EPWM_PTR[SAIDA_EPWM]->AQCTLB.bit.PRD = AQ_PRD;    //  A��o tomada quando CTR = TBPRD
    EPWM_PTR[SAIDA_EPWM]->AQCTLB.bit.CAU = AQ_CAU;    //  A��o tomada quando a modulante de CMPA for menor que a portadora
    EPWM_PTR[SAIDA_EPWM]->AQCTLB.bit.CAD = AQ_CAD;    //  A��o tomada quando a modulante de CMPA for maior que a portadora
    EPWM_PTR[SAIDA_EPWM]->AQCTLB.bit.CBU = AQ_CBU;    //  A��o tomada quando a modulante de CMPB for menor que a portadora
    EPWM_PTR[SAIDA_EPWM]->AQCTLB.bit.CBD = AQ_CBD;    //  A��o tomada quando a modulante de CMPB for maior que a portadora

}



/*---------------------------------------------------------------------------
 * ConfigSyncPWMs:
 *---------------------------------------------------------------------------
 *        Configura todas as PWMs para atuarem de forma sincronizada. Dessa forma
 *      apenas as fases precisam ser alteradas de acordo com a aplica��o.
 */
void ConfigSyncPWMs(void){

    //  Checar se para os PWM a partir do PWM4 � necess�ria mais alguma configura��o, de acordo com a
    // Figura 15-7 na p�gina 1876 do manual completo do dsp

    //EPwm1
    EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;                 //  Mestre (N�o recebe sinal de sincroniza��o)
    EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;             //  Gera SYNCO para PWM2 quando CTR==0
    //EPwm2
    EPwm2Regs.TBCTL.bit.PHSEN = TB_ENABLE;                  //  Recebe SINCI de PWM1
    EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;              //  Gera SYNCO para PWM3 diretamente de SINCI
    //EPwm3
    EPwm3Regs.TBCTL.bit.PHSEN = TB_ENABLE;                  //  Recebe SINCI de PWM2
    EPwm3Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;              //  Gera SYNCO para PWM4 diretamente de SINCI
    //EPwm4
    EPwm4Regs.TBCTL.bit.PHSEN = TB_ENABLE;                  //  Recebe SINCI de PWM3
    EPwm4Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;              //  Gera SYNCO para PWM5 diretamente de SINCI
    //EPwm5
    EPwm5Regs.TBCTL.bit.PHSEN = TB_ENABLE;                  //  Recebe SINCI de PWM4
    EPwm5Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;              //  Gera SYNCO para PWM6 diretamente de SINCI
    //EPwm6
    EPwm6Regs.TBCTL.bit.PHSEN = TB_ENABLE;                  //  Recebe SINCI de PWM5
    EPwm6Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;              //  Gera SYNCO para PWM7 diretamente de SINCI
    //EPwm7
    EPwm7Regs.TBCTL.bit.PHSEN = TB_ENABLE;                  //  Recebe SINCI de PWM6
    EPwm7Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;              //  Gera SYNCO para PWM8 diretamente de SINCI
    //EPwm8
    EPwm8Regs.TBCTL.bit.PHSEN = TB_ENABLE;                  //  Recebe SINCI de PWM7
    EPwm8Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;              //  Gera SYNCO para PWM9 diretamente de SINCI
    //EPwm9
    EPwm9Regs.TBCTL.bit.PHSEN = TB_ENABLE;                  //  Recebe SINCI de PWM8
    EPwm9Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;              //  Gera SYNCO para PWM10 diretamente de SINCI
    //EPwm10
    EPwm10Regs.TBCTL.bit.PHSEN = TB_ENABLE;                  //  Recebe SINCI de PWM9
    EPwm10Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;              //  Gera SYNCO para PWM11 diretamente de SINCI
    //EPwm11
    EPwm11Regs.TBCTL.bit.PHSEN = TB_ENABLE;                  //  Recebe SINCI de PWM10
    EPwm11Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;              //  Gera SYNCO para PWM12 diretamente de SINCI
    //EPwm12
    EPwm12Regs.TBCTL.bit.PHSEN = TB_ENABLE;                  //  Recebe SINCI de PWM11
}

Uint16 DutyCycle(ePWM_HSPCLKDIV CLKDIV1, ePWM_CLKDIV CLKDIV2, Uint16 freq) {
    Uint32 tbclk;
    Uint16 CLKDIV_TAB[8] = {1, 2, 4, 8, 16, 32, 64, 128};
    Uint16 HSPCLKDIV_TAB[8] = {1, 2, 4, 6, 8, 10, 12, 14};

    tbclk = (Uint32) (100000000 / (HSPCLKDIV_TAB[CLKDIV1] * CLKDIV_TAB[CLKDIV2]));

    return (0.5 * (tbclk / freq));
}

//===========================================================================
// End of file.
//===========================================================================

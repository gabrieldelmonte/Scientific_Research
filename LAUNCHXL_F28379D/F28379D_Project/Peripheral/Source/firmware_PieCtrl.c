/*======================================================================================================================
                                                         PIE Control                                                    |
                                                       ---------------                                                  |
FILE: firmware_PieCtrl.c                                                                                                |
                                                                                                                        |
    Baseado em:                                                                                                         |
        "DSP2833x Device PIE Control Register Initialization Functions"                                                 |
        $TI Release: F2833x/F2823x Header Files and Peripheral Examples V142 $                                          |
        $Release Date: November  1, 2016 $                                                                              |
                                                                                                                        |
        $TI Release: F2837xD Support Library v210 $                                                                     |
        $Release Date: Tue Nov  1 14:46:15 CDT 2016 $                                                                   |
________________________________________________________________________________________________________________________|
                                                                                                                        |
                                                                                                   JO�O GABRIEL         |
                                                                                                                        |
                                                                                                    19/Julho/2022       |
=======================================================================================================================*/


#include "Peripheral/Headers/Interrupts.h"

//
// InitPieCtrl - This function initializes the PIE control registers to a
//               known state.
//
void InitPieCtrl(void)
{
    //
    // Disable Interrupts at the CPU level:
    //
    DINT;

    //
    // Disable the PIE
    //
    PieCtrlRegs.PIECTRL.bit.ENPIE = 0;

    //
    // Clear all PIEIER registers:
    //
    PieCtrlRegs.PIEIER1.all = 0;
    PieCtrlRegs.PIEIER2.all = 0;
    PieCtrlRegs.PIEIER3.all = 0;
    PieCtrlRegs.PIEIER4.all = 0;
    PieCtrlRegs.PIEIER5.all = 0;
    PieCtrlRegs.PIEIER6.all = 0;
    PieCtrlRegs.PIEIER7.all = 0;
    PieCtrlRegs.PIEIER8.all = 0;
    PieCtrlRegs.PIEIER9.all = 0;
    PieCtrlRegs.PIEIER10.all = 0;
    PieCtrlRegs.PIEIER11.all = 0;
    PieCtrlRegs.PIEIER12.all = 0;

    //
    // Clear all PIEIFR registers:
    //
    PieCtrlRegs.PIEIFR1.all = 0;
    PieCtrlRegs.PIEIFR2.all = 0;
    PieCtrlRegs.PIEIFR3.all = 0;
    PieCtrlRegs.PIEIFR4.all = 0;
    PieCtrlRegs.PIEIFR5.all = 0;
    PieCtrlRegs.PIEIFR6.all = 0;
    PieCtrlRegs.PIEIFR7.all = 0;
    PieCtrlRegs.PIEIFR8.all = 0;
    PieCtrlRegs.PIEIFR9.all = 0;
    PieCtrlRegs.PIEIFR10.all = 0;
    PieCtrlRegs.PIEIFR11.all = 0;
    PieCtrlRegs.PIEIFR12.all = 0;
}

//
// EnableInterrupts - This function enables the PIE module and CPU __interrupts
//
void EnableInterrupts()
{
    //
    // Enable the PIE
    //
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;      //  � igual nos dois DSPs

    //
    // Enables PIE to drive a pulse into the CPU
    //
    PieCtrlRegs.PIEACK.all = 0xFFFF;


    //-----------------------------------------------------------------------------------------------
    //                              Modifica��es espec�ficas do projeto
    //-----------------------------------------------------------------------------------------------


//    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;  //  INT 1.7 (TIMER0_INT)                //  � igual nos dois DSPs
//
//    PieCtrlRegs.PIEIER1.bit.INTx8 = 1;  //  Habilita PIE Group 1 INT8 (WAKE)    //  � igual nos dois DSPs


    //-----------------------------------------------------------------------------------------------
    //                           FIM Modifica��es espec�ficas do projeto
    //-----------------------------------------------------------------------------------------------

    //
    // Enable Interrupts at the CPU level
    //
    EINT;
}

/*---------------------------------------------------------------------------
 * ConfigInterrupt:
 *---------------------------------------------------------------------------
 *      Habilita os grupos de interrup��o na CPU.
 *      Habilita as interrup��es dos perif�ricos dentro dos grupos.
 *      Habilita a interrup��o global
 */
void ConfigInterrupt(Int_Vect interrupts){
    volatile Uint32 *PierAddr;
    Uint32 mascara;
    unsigned char i;

    #ifdef ATIVAR_INT_GRUPO_1
        IER |= M_INT1;                      //  Habilita interrup��es do grupo 1    //  � igual nos dois DSPs
    #endif
    #ifdef ATIVAR_INT_GRUPO_2
        IER |= M_INT2;                      //  Habilita interrup��es do grupo 2    //  � igual nos dois DSPs
    #endif
    #ifdef ATIVAR_INT_GRUPO_3
        IER |= M_INT3;                      //  Habilita interrup��es do grupo 3    //  � igual nos dois DSPs
    #endif
    #ifdef ATIVAR_INT_GRUPO_4
        IER |= M_INT4;                      //  Habilita interrup��es do grupo 4    //  � igual nos dois DSPs
    #endif
    #ifdef ATIVAR_INT_GRUPO_5
        IER |= M_INT5;                      //  Habilita interrup��es do grupo 5    //  � igual nos dois DSPs
    #endif
    #ifdef ATIVAR_INT_GRUPO_6
        IER |= M_INT6;                      //  Habilita interrup��es do grupo 6    //  � igual nos dois DSPs
    #endif
    #ifdef ATIVAR_INT_GRUPO_7
        IER |= M_INT7;                      //  Habilita interrup��es do grupo 7    //  � igual nos dois DSPs
    #endif
    #ifdef ATIVAR_INT_GRUPO_8
        IER |= M_INT8;                      //  Habilita interrup��es do grupo 8    //  � igual nos dois DSPs
    #endif
    #ifdef ATIVAR_INT_GRUPO_9
        IER |= M_INT9;                      //  Habilita interrup��es do grupo 9    //  � igual nos dois DSPs
    #endif
    #ifdef ATIVAR_INT_GRUPO_10
        IER |= M_INT10;                      //  Habilita interrup��es do grupo 10    //  � igual nos dois DSPs
    #endif
    #ifdef ATIVAR_INT_GRUPO_11
        IER |= M_INT11;                      //  Habilita interrup��es do grupo 11    //  � igual nos dois DSPs
    #endif
    #ifdef ATIVAR_INT_GRUPO_12
        IER |= M_INT12;                      //  Habilita interrup��es do grupo 12    //  � igual nos dois DSPs
    #endif

    for(i = 0; i < sizeof(interrupts)/2 ; i++){

        PierAddr = (Uint32 *)&PieCtrlRegs + (interrupts.vetor[i].grupo) * (0x02)/2;
        mascara = 0x0001 << (interrupts.vetor[i].periferico);
        *PierAddr |= mascara;
    }

    EnableInterrupts();

}




//===========================================================================
// End of file.
//===========================================================================



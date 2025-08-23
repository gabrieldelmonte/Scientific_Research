/*
 * firmware_Watchdog.c
 *
 *  Created on: 22 de jul de 2022
 *      Author: foste
 */


#include "Peripheral/Headers/Watchdog.h"

/*---------------------------------------------------------------------------
 * ConfigWatchdog:
 *---------------------------------------------------------------------------
 *      Faz a configura��o de como o Watchdog vai operar.
 *      O Watchdog pode acionar uma interrup��o ou resetar o DSP.

 */
void ConfigWatchdog(WD_operation WD_mode){

    /*  Para evitar problemas � bom garantir que WDINT n�o est� ativado antes de modificar o modo de opera��o do Watchdog */
    while(!WdRegs.SCSR.bit.WDINTS){}

    if (WD_mode == INTERRUPT){
        EALLOW;
            WdRegs.SCSR.all = C28X_BIT1;    //  Configura o watchdog para ativar uma interrup��o ao inv�s de resetar o DSP
        EDIS;
    }else if (WD_mode == RESET){
        EALLOW;
            WdRegs.SCSR.all = 0x00000000;    //  Configura o watchdog para resetar o DSP
        EDIS;
    }

}


/*---------------------------------------------------------------------------
 * EnableWatchdog:
 *---------------------------------------------------------------------------
 *      Habilita o Watchdog, e define o prescaler.
 *
 *      INTOSC1 = 10MHz
 *      Por padr�o WDCLK = INTOSC1/512 =~ 19531Hz
 *      1/19531 =~ 51us
 *      Logo, para o Watchdog completar a contagem do registrador WDCNTR [8bits -> 0~255], �
 *    necess�rio aproximadamente 13ms
 *
 *    Exemplo:
 *
 *    SetWatchdogPrescaler(WD_PS_8);
 *
 *      Configurando o prescaler com WD_PS_8 faz:
 *          WDCLK = (INTOSC1/512)/8 =~ 2441Hz
 *          1/2441 =~ 410us
 *          Logo, para o Watchdog completar a contagem do registrador WDCNTR [8bits -> 0~255], �
 *    necess�rio aproximadamente 104ms
 */
void EnableWatchdog(WD_Prescaler prescaler){
    Uint16 temp = 0;

    temp = 0x0028 | prescaler;      //  Aciona WDDIS, escreve 0b101 em WDCHK e configura o WDPS
    EALLOW;
        WdRegs.WDCR.all = temp;
    EDIS;

}


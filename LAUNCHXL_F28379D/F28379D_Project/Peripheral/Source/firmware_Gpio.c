/*======================================================================================================================
                                                            GPIO                                                        |
                                                          --------                                                      |
FILE: firmware_GPIO.c                                                                                                   |
                                                                                                                        |
    Fun��es usadas para a configura��o das GPIOs.                                                                       |
________________________________________________________________________________________________________________________|
                                                                                                                        |
    Adaptado de:                                                                                                        |
    "DSP2833x General Purpose I/O Initialization & Support Functions" da documenta��o da TI.                            |
        $TI Release: DSP2833x/DSP2823x C/C++ Header Files V1.31 $                                                       |
        $Release Date: August 4, 2009 $                                                                                 |
                                                                                                                        |
    "GPIO module support functions" da documenta��o da TI.                                                              |
        $TI Release: F2837xD Support Library v210 $                                                                     |
        $Release Date: Tue Nov  1 14:46:15 CDT 2016 $                                                                   |
                                                                                                                        |
________________________________________________________________________________________________________________________|
                                                                                                                        |
                                                                                                   JO�O GABRIEL         |                                                                                                                       |
                                                                                                      19/Julho/2022     |
=======================================================================================================================*/


#include "Peripheral/Headers/GPIO.h"

//
//Low-level functions for GPIO configuration (CPU1 only)
//

#ifdef CPU1
    //
    // InitGpio - Sets all pins to be muxed to GPIO in input mode with pull-ups
    //            enabled. Also resets CPU control to CPU1 and disables open
    //            drain and polarity inversion and sets the qualification to
    //            synchronous. Also unlocks all GPIOs. Only one CPU should call
    //            this function.
    //
    void InitGpio()
    {
        volatile Uint32 *gpioBaseAddr;
        Uint16 regOffset;

        //
        //Disable pin locks
        //
        EALLOW;
        GpioCtrlRegs.GPALOCK.all = 0x00000000;
        GpioCtrlRegs.GPBLOCK.all = 0x00000000;
        GpioCtrlRegs.GPCLOCK.all = 0x00000000;
        GpioCtrlRegs.GPDLOCK.all = 0x00000000;
        GpioCtrlRegs.GPELOCK.all = 0x00000000;
        GpioCtrlRegs.GPFLOCK.all = 0x00000000;

        //
        // Fill all registers with zeros. Writing to each register separately
        // for six GPIO modules would make this function *very* long.
        // Fortunately, we'd be writing them all with zeros anyway, so this
        // saves a lot of space.
        //
        gpioBaseAddr = (Uint32 *)&GpioCtrlRegs;
        for (regOffset = 0; regOffset < sizeof(GpioCtrlRegs)/2; regOffset++)
        {
            //
            //Hack to avoid enabling pull-ups on all pins. GPyPUD is offset
            //0x0C in each register group of 0x40 words. Since this is a
            //32-bit pointer, the addresses must be divided by 2.
            //
            if (regOffset % (0x40/2) != (0x0C/2))
            {
                gpioBaseAddr[regOffset] = 0x00000000;
            }
        }

        gpioBaseAddr = (Uint32 *)&GpioDataRegs;
        for (regOffset = 0; regOffset < sizeof(GpioDataRegs)/2; regOffset++)
        {
            gpioBaseAddr[regOffset] = 0x00000000;
        }

        EDIS;

    }

    //
    // GPIO_SetupPinMux - Set the peripheral muxing for the specified pin. The
    //                    appropriate parameters can be found in the GPIO Muxed
    //                    Pins table(4.4) in the  datasheet. Use the GPIO index
    //                    row (0 to 15) to select a muxing option for the GPIO.
    //
    void GPIO_SetupPinMux(Uint16 pin, Uint16 cpu, Uint16 peripheral)
    {
        volatile Uint32 *gpioBaseAddr;
        volatile Uint32 *mux, *gmux, *csel;
        Uint16 pin32, pin16, pin8;

        pin32 = pin % 32;
        pin16 = pin % 16;
        pin8 = pin % 8;
        gpioBaseAddr = (Uint32 *)&GpioCtrlRegs + (pin/32)*GPY_CTRL_OFFSET;

        //
        //Sanity check for valid cpu and peripheral values
        //
        if (cpu > GPIO_MUX_CPU2CLA || peripheral > 0xF)     //   Perif�ricos definidos por 4 bits, 2 bits para GPyMUX e 2 bits para GPyGMUX
            return;

        //
        //Create pointers to the appropriate registers. This is a workaround
        //for the way GPIO registers are defined. The standard definition
        //in the header file makes it very easy to do named accesses of one
        //register or bit, but hard to do arbitrary numerical accesses. It's
        //easier to have an array of GPIO modules with identical registers,
        //including arrays for multi-register groups like GPyCSEL1-4. But
        //the header file doesn't define anything we can turn into an array,
        //so manual pointer arithmetic is used instead.
        //
        mux = gpioBaseAddr + GPYMUX + pin32/16;
        gmux = gpioBaseAddr + GPYGMUX + pin32/16;
        csel = gpioBaseAddr + GPYCSEL + pin32/8;

        //
        //Now for the actual function
        //
        EALLOW;

        //
        //To change the muxing, set the peripheral mux to 0/GPIO first to avoid
        //glitches, then change the group mux, then set the peripheral mux to
        //its target value. Finally, set the CPU select. This procedure is
        //described in the TRM. Unfortunately, since we don't know the pin in
        //advance we can't hardcode a bitfield reference, so there's some
        //tricky bit twiddling here.
        //
        *mux &= ~(0x3UL << (2*pin16));
        *gmux &= ~(0x3UL << (2*pin16));
        *gmux |= (Uint32)((peripheral >> 2) & 0x3UL) << (2*pin16);
        *mux |= (Uint32)(peripheral & 0x3UL) << (2*pin16);

        *csel &= ~(0x3L << (4*pin8));
        *csel |= (Uint32)(cpu & 0x3L) << (4*pin8);

        //
        //WARNING: This code does not touch the analog mode select registers,
        //which are needed to give the USB module control of its IOs.
        //
        EDIS;
    }

    //
    // GPIO_SetupPinOptions - Setup up the GPIO input/output options for the
    //                        specified pin.
    //
    //The flags are a 16-bit mask produced by ORing together options.
    //For input pins, the valid flags are:
    //GPIO_PULLUP    Enable pull-up
    //GPIO_INVERT    Enable input polarity inversion
    //GPIO_SYNC        Synchronize the input latch to PLLSYSCLK
    //               (default -- you don't need to specify this)
    //GPIO_QUAL3    Use 3-sample qualification
    //GPIO_QUAL6    Use 6-sample qualification
    //GPIO_ASYNC    Do not use synchronization or qualification
    //(Note: only one of SYNC, QUAL3, QUAL6, or ASYNC is allowed)
    //
    //For output pins, the valid flags are:
    //GPIO_OPENDRAIN    Output in open drain mode
    //GPIO_PULLUP        If open drain enabled, also enable the pull-up
    //and the input qualification flags (SYNC/QUAL3/QUAL6/SYNC) listed above.
    //
    //With no flags, the default input state is synchronous with no
    //pull-up or polarity inversion. The default output state is
    //the standard digital output.
    //
    void GPIO_SetupPinOptions(Uint16 pin, Uint16 output, Uint16 flags)
    {
        volatile Uint32 *gpioBaseAddr;
        volatile Uint32 *dir, *pud, *inv, *odr, *qsel;
        Uint32 pin32, pin16, pinMask, qual;

        pin32 = pin % 32;
        pin16 = pin % 16;
        pinMask = 1UL << pin32;
        gpioBaseAddr = (Uint32 *)&GpioCtrlRegs + (pin/32)*GPY_CTRL_OFFSET;

        //
        //Create pointers to the appropriate registers. This is a workaround
        //for the way GPIO registers are defined. The standard definition
        //in the header file makes it very easy to do named accesses of one
        //register or bit, but hard to do arbitrary numerical accesses. It's
        //easier to have an array of GPIO modules with identical registers,
        //including arrays for multi-register groups like GPyQSEL1-2. But
        //the header file doesn't define anything we can turn into an array,
        //so manual pointer arithmetic is used instead.
        //
        dir = gpioBaseAddr + GPYDIR;
        pud = gpioBaseAddr + GPYPUD;
        inv = gpioBaseAddr + GPYINV;
        odr = gpioBaseAddr + GPYODR;
        qsel = gpioBaseAddr + GPYQSEL + pin32/16;

        EALLOW;

        //
        //Set the data direction
        //
        *dir &= ~pinMask;
        if (output == 1)
        {
            //
            //Output, with optional open drain mode and pull-up
            //
            *dir |= pinMask;

            //
            //Enable open drain if necessary
            //
            if (flags & GPIO_OPENDRAIN)
            {
                *odr |= pinMask;
            }
            else
            {
                *odr &= ~pinMask;
            }

            //
            //Enable pull-up if necessary. Open drain mode must be active.
            //
            if (flags & (GPIO_OPENDRAIN | GPIO_PULLUP))
            {
                *pud &= ~pinMask;
            }
            else
            {
                *pud |= pinMask;
            }
        }
        else
        {
            //
            //Input, with optional pull-up, qualification, and polarity
            //inversion
            //
            *dir &= ~pinMask;

            //
            //Enable pull-up if necessary
            //
            if (flags & GPIO_PULLUP)
            {
                *pud &= ~pinMask;
            }
            else
            {
                *pud |= pinMask;
            }

            //
            //Invert polarity if necessary
            //
            if (flags & GPIO_INVERT)
            {
                *inv |= pinMask;
            }
            else
            {
                *inv &= ~pinMask;
            }
        }

        //
        //Extract the qualification parameter and load it into the register.
        //This is also needed for open drain outputs, so we might as well do it
        //all the time.
        //
        qual = (flags & GPIO_ASYNC) / GPIO_QUAL3;
        *qsel &= ~(0x3L << (2 * pin16));
        if (qual != 0x0)
        {
            *qsel |= qual << (2 * pin16);
        }

        EDIS;
    }

    //
    // GPIO_SetupLock - Enable or disable the GPIO register bit lock for the
    //                  specified pin.
    //                  The valid flags are:
    //                  GPIO_UNLOCK - Unlock the pin setup register bits for
    //                                the specified pin
    //                  GPIO_LOCK - Lock the pin setup register bits for the
    //                              specified pin
    //
    void GPIO_SetupLock(Uint16 pin, Uint16 flags)
    {
        volatile Uint32 *gpioBaseAddr;
        volatile Uint32 *lock;
        Uint32 pin32, pinMask;

        pin32 = pin % 32;
        pinMask = 1UL << pin32;
        gpioBaseAddr = (Uint32 *)&GpioCtrlRegs + (pin/32)*GPY_CTRL_OFFSET;

        //
        //Create pointers to the appropriate registers. This is a workaround
        //for the way GPIO registers are defined. The standard definition
        //in the header file makes it very easy to do named accesses of one
        //register or bit, but hard to do arbitrary numerical accesses. It's
        //easier to have an array of GPIO modules with identical registers,
        //including arrays for multi-register groups like GPyQSEL1-2. But
        //the header file doesn't define anything we can turn into an array,
        //so manual pointer arithmetic is used instead.
        //
        lock = gpioBaseAddr + GPYLOCK;

        EALLOW;
        if(flags)
        {
            //Lock the pin
            *lock |= pinMask;
        }
        else
        {
            //Unlock the pin
            *lock &= ~pinMask;
        }
        EDIS;
    }

    //
    //External interrupt setup
    //
    void GPIO_SetupXINT1Gpio(Uint16 pin)
    {
        EALLOW;
        InputXbarRegs.INPUT4SELECT = pin;      //Set XINT1 source to GPIO-pin
        EDIS;
    }
    void GPIO_SetupXINT2Gpio(Uint16 pin)
    {
        EALLOW;
        InputXbarRegs.INPUT5SELECT = pin;      //Set XINT2 source to GPIO-pin
        EDIS;
    }
    void GPIO_SetupXINT3Gpio(Uint16 pin)
    {
        EALLOW;
        InputXbarRegs.INPUT6SELECT = pin;      //Set XINT3 source to GPIO-pin
        EDIS;
    }
    void GPIO_SetupXINT4Gpio(Uint16 pin)
    {
        EALLOW;
        InputXbarRegs.INPUT13SELECT = pin;     //Set XINT4 source to GPIO-pin
        EDIS;
    }
    void GPIO_SetupXINT5Gpio(Uint16 pin)
    {
        EALLOW;
        InputXbarRegs.INPUT14SELECT = pin;     //Set XINT5 source to GPIO-pin
        EDIS;
    }

    //
    //GPIO_EnableUnbondedIOPullupsFor176Pin - Enable pullups for the unbonded
    //                                        GPIOs on the 176PTP package:
    //                                        GPIOs     Grp Bits
    //                                        95-132    C   31
    //                                                  D   31:0
    //                                                  E   4:0
    //                                        134-168   E   31:6
    //                                                  F   8:0
    //
    void GPIO_EnableUnbondedIOPullupsFor176Pin()
    {
        EALLOW;
        GpioCtrlRegs.GPCPUD.all = ~0x80000000;  //GPIO 95
        GpioCtrlRegs.GPDPUD.all = ~0xFFFFFFF7;  //GPIOs 96-127
        GpioCtrlRegs.GPEPUD.all = ~0xFFFFFFDF;  //GPIOs 128-159 except for 133
        GpioCtrlRegs.GPFPUD.all = ~0x000001FF;  //GPIOs 160-168
        EDIS;
    }

    //
    // GPIO_EnableUnbondedIOPullupsFor100Pin - Enable pullups for the unbonded
    //                                         GPIOs on the 100PZ package:
    //                                         GPIOs     Grp Bits
    //                                         0-1       A   1:0
    //                                         5-9       A   9:5
    //                                         22-40     A   31:22
    //                                                   B   8:0
    //                                         44-57     B   25:12
    //                                         67-68     C   4:3
    //                                         74-77     C   13:10
    //                                         79-83     C   19:15
    //                                         93-168    C   31:29
    //                                                   D   31:0
    //                                                   E   31:0
    //                                                   F   8:0
    //
    void GPIO_EnableUnbondedIOPullupsFor100Pin()
    {
        EALLOW;
        GpioCtrlRegs.GPAPUD.all = ~0xFFC003E3;  //GPIOs 0-1, 5-9, 22-31
        GpioCtrlRegs.GPBPUD.all = ~0x03FFF1FF;  //GPIOs 32-40, 44-57
        GpioCtrlRegs.GPCPUD.all = ~0xE10FBC18;  //GPIOs 67-68, 74-77, 79-83, 93-95
        GpioCtrlRegs.GPDPUD.all = ~0xFFFFFFF7;  //GPIOs 96-127
        GpioCtrlRegs.GPEPUD.all = ~0xFFFFFFFF;  //GPIOs 128-159
        GpioCtrlRegs.GPFPUD.all = ~0x000001FF;  //GPIOs 160-168
        EDIS;
    }

    //
    // GPIO_EnableUnbondedIOPullups - InitSysCtrl would call this function
    //                                this takes care of enabling IO pullups.
    //
    void GPIO_EnableUnbondedIOPullups()
    {
        //
        //bits 8-10 have pin count
        //
        unsigned char pin_count = ((DevCfgRegs.PARTIDL.all & 0x00000700) >> 8) ;

        //
        //5 = 100 pin
        //6 = 176 pin
        //7 = 337 pin
        //
        if(pin_count == 5)
        {
            GPIO_EnableUnbondedIOPullupsFor100Pin();
        }
        else if (pin_count == 6)
        {
            GPIO_EnableUnbondedIOPullupsFor176Pin();
        }
        else
        {
            //do nothing - this is 337 pin package
        }
    }

#endif //CPU1

//
// GPIO_ReadPin - Read the GPyDAT register bit for the specified pin. Note that
//                this returns the actual state of the pin, not the state of
//                the output latch.
//
Uint16 GPIO_ReadPin(Uint16 pin)
{
    volatile Uint32 *gpioDataReg;
    Uint16 pinVal;

    gpioDataReg = (volatile Uint32 *)&GpioDataRegs + (pin/32)*GPY_DATA_OFFSET;
    pinVal = (gpioDataReg[GPYDAT] >> (pin % 32)) & 0x1;

    return pinVal;
}

//
// GPIO_WritePin - Set the GPyDAT register bit for the specified pin.
//
void GPIO_WritePin(Uint16 pin, Uint16 outVal)
{
    volatile Uint32 *gpioDataReg;
    Uint32 pinMask;

    gpioDataReg = (volatile Uint32 *)&GpioDataRegs + (pin/32)*GPY_DATA_OFFSET;
    pinMask = 1UL << (pin % 32);

    if (outVal == 0)
    {
        gpioDataReg[GPYCLEAR] = pinMask;
    }
    else
    {
        gpioDataReg[GPYSET] = pinMask;
    }
}






/*-------------------------------------------------------------------------------------------------
                                     Fun��es espec�ficas
                                    --------------------

    Fun��es criadas para facilitar a utiliza��o das GPIO.
-------------------------------------------------------------------------------------------------*/



//---------------------------------------------------------------------------
// ConfigGPIO:
//---------------------------------------------------------------------------
//      Configura um pino de GPIO que ser� usado como IO. Atrav�s desta fun��o � poss�vel
//  fazer todas as configura��es do pino.
//
//
//  Par�metros:
//      unsigned char GPIO_number:          N�mero da GPIO [0~168]
//      unsigned char I_O:                  Escolhe se a porta vai atuar como entrada ou sa�da. [ENTRADA / SAIDA]
//      unsigned char Pullup:               Define se o pullup vai estar ativado ou n�o. [PULLUP_ATIVADO / PULLUP_DESATIVADO]                   -> Padr�o DESATIVADO
//      unsigned char Inverter_Entrada:     Define se o valor lido ser� invertido ou n�o. [INV_ATIVADO / INV_DESATIVADO]                        -> Padr�o DESATIVADO
//      unsigned char Open_Drain:           Coloca a sa�da como open drain. [OPENDRAIN_ATIVADO / OPENDRAIN_DESATIVADO]                          -> Padr�o DESATIVADO
//      unsigned char QSEL:                 Define o n�mero de amostras de per�odo, somente para modo entrada. [SYNC / QUAL3 / QUAL6 / ASYNC]   -> Padr�o SYNC
//      unsigned char QUALPRD:              Define o per�odo de qualifica��o das entradas. [0~255]                                              -> Padr�o 0 (PLLSYSCLK)
//                                  - QUALPRD � definido para o grupo de entradas que a GPIO escolhida fizer parte.
//                                  - Tqualprd = 2*QUALPRD*Tsysclkout
//
//  * Par�metros usados para ENTRADA: GPIO_number, I_O, Pullup, Inverter_Entrada, QSEL, QUALPRD
//  * Par�metros usados para SAIDA: GPIO_number, I_O, Open_Drain -> Caso Open_Drain == ATIVADO [Pullup, QSEL, QUALPRD]
//  * Usar "NULL" para os parametros que n�o forem usados.
//
//
void ConfigGPIO(unsigned char GPIO_number, unsigned char I_O, unsigned char Pullup, unsigned char Inverter_Entrada, unsigned char Open_Drain, unsigned char QSEL, unsigned char QUALPRD){
    Uint16 flags;

    flags = 0x0000;

    // Checar se o n�mero da GPIO est� dentro dos valores v�lidos
    if(GPIO_number > 168){
        return;
    }

    GPIO_SetupPinMux(GPIO_number, GPIO_MUX_CPU1, 0);        //  Configura a GPIO desejada como IO

    if(I_O == ENTRADA){
        if(Pullup == PULLUP_ATIVADO){
            flags |= GPIO_PULLUP;
        }else{
            flags |= GPIO_PUSHPULL;
        }
        if(Inverter_Entrada == INV_ATIVADO){
            flags |= GPIO_INVERT;
        }
        if(QSEL == ASYNC){
            flags |= GPIO_ASYNC;
        }else if(QSEL == QUAL3){
            flags |= GPIO_QUAL3;
        }else if(QSEL == QUAL6){
            flags |= GPIO_QUAL6;
        }else{
            flags |= GPIO_SYNC;
        }
        GPIO_SetupPinOptions(GPIO_number, GPIO_INPUT, flags);   //  GPIO definida como entrada e configurada com as flags desejadas
    }else if(I_O == SAIDA){
        if(Open_Drain == OPENDRAIN_ATIVADO){
            flags |= GPIO_OPENDRAIN;
            if(Pullup == PULLUP_ATIVADO){
                flags |= GPIO_PULLUP;
            }else{
                flags |= GPIO_PUSHPULL;
            }
            if(QSEL == ASYNC){
                flags |= GPIO_ASYNC;
            }else if(QSEL == QUAL3){
                flags |= GPIO_QUAL3;
            }else if(QSEL == QUAL6){
                flags |= GPIO_QUAL6;
            }else{
                flags |= GPIO_SYNC;
            }
        }else{
            flags |= GPIO_PUSHPULL;
        }

        GPIO_SetupPinOptions(GPIO_number, GPIO_OUTPUT, flags);   //  GPIO definida como sa�da e configurada com as flags desejadas
    }

    // Checar se QUALPRD est� dentro dos valores permitidos. (Sendo QUALPRD Unsigned CHar � necess�rio checar isso?)
    if(QUALPRD > 255){
        QUALPRD = 0;
    }

    EALLOW;
        if(GPIO_number <= 7){
            GpioCtrlRegs.GPACTRL.bit.QUALPRD0 = QUALPRD;
        }else if((GPIO_number >= 8) && (GPIO_number <= 15)){
            GpioCtrlRegs.GPACTRL.bit.QUALPRD1 = QUALPRD;
        }else if((GPIO_number >= 16) && (GPIO_number <= 23)){
            GpioCtrlRegs.GPACTRL.bit.QUALPRD2 = QUALPRD;
        }else if((GPIO_number >= 24) && (GPIO_number <= 31)){
            GpioCtrlRegs.GPACTRL.bit.QUALPRD3 = QUALPRD;
        }else if((GPIO_number >= 32) && (GPIO_number <= 39)){
            GpioCtrlRegs.GPBCTRL.bit.QUALPRD0 = QUALPRD;
        }else if((GPIO_number >= 40) && (GPIO_number <= 47)){
            GpioCtrlRegs.GPBCTRL.bit.QUALPRD1 = QUALPRD;
        }else if((GPIO_number >= 48) && (GPIO_number <= 55)){
            GpioCtrlRegs.GPBCTRL.bit.QUALPRD2 = QUALPRD;
        }else if((GPIO_number >= 56) && (GPIO_number <= 63)){
            GpioCtrlRegs.GPBCTRL.bit.QUALPRD3 = QUALPRD;
        }else if((GPIO_number >= 64) && (GPIO_number <= 71)){
            GpioCtrlRegs.GPCCTRL.bit.QUALPRD0 = QUALPRD;
        }else if((GPIO_number >= 72) && (GPIO_number <= 79)){
            GpioCtrlRegs.GPCCTRL.bit.QUALPRD1 = QUALPRD;
        }else if((GPIO_number >= 80) && (GPIO_number <= 87)){
            GpioCtrlRegs.GPCCTRL.bit.QUALPRD2 = QUALPRD;
        }else if((GPIO_number >= 88) && (GPIO_number <= 95)){
            GpioCtrlRegs.GPCCTRL.bit.QUALPRD3 = QUALPRD;
        }else if((GPIO_number >= 96) && (GPIO_number <= 103)){
            GpioCtrlRegs.GPDCTRL.bit.QUALPRD0 = QUALPRD;
        }else if((GPIO_number >= 104) && (GPIO_number <= 111)){
            GpioCtrlRegs.GPDCTRL.bit.QUALPRD1 = QUALPRD;
        }else if((GPIO_number >= 112) && (GPIO_number <= 119)){
            GpioCtrlRegs.GPDCTRL.bit.QUALPRD2 = QUALPRD;
        }else if((GPIO_number >= 120) && (GPIO_number <= 127)){
            GpioCtrlRegs.GPDCTRL.bit.QUALPRD3 = QUALPRD;
        }else if((GPIO_number >= 128) && (GPIO_number <= 135)){
            GpioCtrlRegs.GPECTRL.bit.QUALPRD0 = QUALPRD;
        }else if((GPIO_number >= 136) && (GPIO_number <= 143)){
            GpioCtrlRegs.GPECTRL.bit.QUALPRD1 = QUALPRD;
        }else if((GPIO_number >= 144) && (GPIO_number <= 151)){
            GpioCtrlRegs.GPECTRL.bit.QUALPRD2 = QUALPRD;
        }else if((GPIO_number >= 152) && (GPIO_number <= 159)){
            GpioCtrlRegs.GPECTRL.bit.QUALPRD3 = QUALPRD;
        }else if((GPIO_number >= 160) && (GPIO_number <= 167)){
            GpioCtrlRegs.GPFCTRL.bit.QUALPRD0 = QUALPRD;
        }else if(GPIO_number == 168){
            GpioCtrlRegs.GPFCTRL.bit.QUALPRD1 = QUALPRD;
        }
    EDIS;


}


/*---------------------------------------------------------------------------
 * GPIO_SetupQualCtrl:
 *---------------------------------------------------------------------------
 *   Configura o registro GPyCTRL para definir o per�odo de amostragem
 * dos pinos de entrada. Lembrar que os per�odos de qualifica��o s�o definidos
 * para grupos de entradas.
 *
 *    Par�metros:
 *       Uint16 pin:             N�mero da GPIO [0~168]
 *       unsigned char QUALPRD:  Define o per�odo de qualifica��o das entradas. [0~255]
 *
 */

void GPIO_SetupQualCtrl(Uint16 GPIO_number, unsigned char QUALPRD){

    volatile Uint32 *ctrl;
    Uint32 pin32;

    pin32 = GPIO_number%32;

    if(GPIO_number <= 168){
        EALLOW;
            ctrl = (Uint32 *)&GpioCtrlRegs + (GPIO_number/32)*GPY_CTRL_OFFSET;
            *ctrl &= ~(0xFFUL << (8*(pin32/8)));
            *ctrl |= (Uint32)(QUALPRD & 0xFFUL) << (8*(pin32/8));
        EDIS;
    }

}

//===========================================================================
// End of file.
//===========================================================================

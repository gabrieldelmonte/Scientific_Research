/*======================================================================================================================
                                             Analog to Digital Converter                                                |
                                        ------------------------------------                                            |
FILE: firmware_ADC.c                                                                                                    |
                                                                                                                        |
    Fun��es para configura��o e opera��o do perif�rico ADC.                                                             |
                                                                                                                        |
    Baseado nos exemplos do ControlSUITE e na documenta��o do DSP TMS320F28335:                                         |
        Technical Reference:    https://www.ti.com/lit/ug/sprui07/sprui07.pdf?ts=1657282731527                          |
        Datasheet:              https://www.ti.com/lit/ds/symlink/tms320f28335.pdf?ts=1657262846286                     |
                                                                                                                        |
    Baseado em: F2837xD Adc Support Functions                                                                           |
        $TI Release: F2837xD Support Library v210 $                                                                     |
        $Release Date: Tue Nov  1 14:46:15 CDT 2016 $                                                                   |
________________________________________________________________________________________________________________________|
                                                                                                                        |
                                                                                                   JO�O GABRIEL         |
                                                                                                                        |
                                                                                                    19/Julho/2022       |
=======================================================================================================================*/


#include "Peripheral/Headers/ADC.h"

//
// AdcSetMode - Set the resolution and signalmode for a given ADC. This will
//              ensure that the correct trim is loaded.
//
void AdcSetMode(Uint16 adc, Uint16 resolution, Uint16 signalmode)
{
    Uint16 adcOffsetTrimOTPIndex; //index into OTP table of ADC offset trims
    Uint16 adcOffsetTrim;         //temporary ADC offset trim

    //
    //re-populate INL trim
    //
    CalAdcINL(adc);

    if(0xFFFF != *((Uint16*)GetAdcOffsetTrimOTP))
    {
        //
        //offset trim function is programmed into OTP, so call it
        //

        //
        //calculate the index into OTP table of offset trims and call
        //function to return the correct offset trim
        //
        adcOffsetTrimOTPIndex = 4*adc + 2*resolution + 1*signalmode;
        adcOffsetTrim = (*GetAdcOffsetTrimOTP)(adcOffsetTrimOTPIndex);
    }
    else
    {
        //
        //offset trim function is not populated, so set offset trim to 0
        //
        adcOffsetTrim = 0;
    }

    //
    //Apply the resolution and signalmode to the specified ADC.
    //Also apply the offset trim and, if needed, linearity trim correction.
    //
    switch(adc)
    {
        case ADC_ADCA:
            AdcaRegs.ADCCTL2.bit.RESOLUTION = resolution;
            AdcaRegs.ADCCTL2.bit.SIGNALMODE = signalmode;
            AdcaRegs.ADCOFFTRIM.all = adcOffsetTrim;
            if(ADC_RESOLUTION_12BIT == resolution)
            {
                //
                //12-bit linearity trim workaround
                //
                AdcaRegs.ADCINLTRIM1 &= 0xFFFF0000;
                AdcaRegs.ADCINLTRIM2 &= 0xFFFF0000;
                AdcaRegs.ADCINLTRIM4 &= 0xFFFF0000;
                AdcaRegs.ADCINLTRIM5 &= 0xFFFF0000;
            }
        break;
        case ADC_ADCB:
            AdcbRegs.ADCCTL2.bit.RESOLUTION = resolution;
            AdcbRegs.ADCCTL2.bit.SIGNALMODE = signalmode;
            AdcbRegs.ADCOFFTRIM.all = adcOffsetTrim;
            if(ADC_RESOLUTION_12BIT == resolution)
            {
                //
                //12-bit linearity trim workaround
                //
                AdcbRegs.ADCINLTRIM1 &= 0xFFFF0000;
                AdcbRegs.ADCINLTRIM2 &= 0xFFFF0000;
                AdcbRegs.ADCINLTRIM4 &= 0xFFFF0000;
                AdcbRegs.ADCINLTRIM5 &= 0xFFFF0000;
            }
        break;
        case ADC_ADCC:
            AdccRegs.ADCCTL2.bit.RESOLUTION = resolution;
            AdccRegs.ADCCTL2.bit.SIGNALMODE = signalmode;
            AdccRegs.ADCOFFTRIM.all = adcOffsetTrim;
            if(ADC_RESOLUTION_12BIT == resolution)
            {
                //
                //12-bit linearity trim workaround
                //
                AdccRegs.ADCINLTRIM1 &= 0xFFFF0000;
                AdccRegs.ADCINLTRIM2 &= 0xFFFF0000;
                AdccRegs.ADCINLTRIM4 &= 0xFFFF0000;
                AdccRegs.ADCINLTRIM5 &= 0xFFFF0000;
            }
        break;
        case ADC_ADCD:
            AdcdRegs.ADCCTL2.bit.RESOLUTION = resolution;
            AdcdRegs.ADCCTL2.bit.SIGNALMODE = signalmode;
            AdcdRegs.ADCOFFTRIM.all = adcOffsetTrim;
            if(ADC_RESOLUTION_12BIT == resolution)
            {
                //
                //12-bit linearity trim workaround
                //
                AdcdRegs.ADCINLTRIM1 &= 0xFFFF0000;
                AdcdRegs.ADCINLTRIM2 &= 0xFFFF0000;
                AdcdRegs.ADCINLTRIM4 &= 0xFFFF0000;
                AdcdRegs.ADCINLTRIM5 &= 0xFFFF0000;
            }
        break;
    }
}

//
// CalAdcINL - Loads INL trim values from OTP into the trim registers of the
//             specified ADC. Use only as part of AdcSetMode function, since
//             linearity trim correction is needed for some modes.
//
void CalAdcINL(Uint16 adc)
{
    switch(adc)
    {
        case ADC_ADCA:
            if(0xFFFF != *((Uint16*)CalAdcaINL))
            {
                //
                //trim function is programmed into OTP, so call it
                //
                (*CalAdcaINL)();
            }
            else
            {
                //
                //do nothing, no INL trim function populated
                //
            }
            break;
        case ADC_ADCB:
            if(0xFFFF != *((Uint16*)CalAdcbINL))
            {
                //
                //trim function is programmed into OTP, so call it
                //
                (*CalAdcbINL)();
            }
            else
            {
                //
                //do nothing, no INL trim function populated
                //
            }
            break;
        case ADC_ADCC:
            if(0xFFFF != *((Uint16*)CalAdccINL))
            {
                //
                //trim function is programmed into OTP, so call it
                //
                (*CalAdccINL)();
            }
            else
            {
                //
                //do nothing, no INL trim function populated
                //
            }
            break;
        case ADC_ADCD:
            if(0xFFFF != *((Uint16*)CalAdcdINL))
            {
                //
                //trim function is programmed into OTP, so call it
                //
                (*CalAdcdINL)();
            }
            else
            {
                //
                //do nothing, no INL trim function populated
                //
            }
            break;
    }
}

//
// End of file
//


/*-------------------------------------------------------------------------------------------------
                                     Fun��es espec�ficas
                                    --------------------

    Fun��es criadas para facilitar a utiliza��o do ADC.
-------------------------------------------------------------------------------------------------*/

volatile struct ADC_REGS* ADC_PTR[5] = {0x0, &AdcaRegs, &AdcbRegs, &AdccRegs, &AdcdRegs};
volatile struct ADC_RESULT_REGS* ADC_RESULT_PTR[5] = {0x0, &AdcaResultRegs, &AdcbResultRegs, &AdccResultRegs, &AdcdResultRegs};



/*---------------------------------------------------------------------------
 * InitADC:
 *---------------------------------------------------------------------------
 *      Faz uma configura��o padr�o e inicializa os ADCs que ser�o utilizados. Caso
 *    seja necess�rio alterar a configura��o padr�o, modificar os valores alocados
 *    nos registros por esta fun��o, de acordo com as defini��es em "ADC.h".
 *      A configura��o padr�o �:
 *          - ADCCLKx = SYSCLK/4
 *          - Medida Single-ended (resolu��o 12 bits)
 *          - EOC gerado ao fim da convers�o
 *
 *      * Necess�rio configurar a quantidade de entradas ADC que ser�o usadas. Isso �
 *      feito alterando SENSOR_BUF_SIZE em "defines.h". Caso sejam usados os objetos
 *      do tipo sensor tamb�m � aconselh�vel dar uma ID �nica para cada sensor, atrav�s
 *      da altera��o do tipo enum "Sensores", tamb�m definido em "defines.h"
 *
 */
void InitADC(void){



    EALLOW;     //  Permite escrita em registradores protegidos

// ------------------------------------------------------------------------------- //
// ******************** AdcaRegs - ADC-A Control Register *************************//
// ------------------------------------------------------------------------------- //

        #ifdef ATIVAR_ADC_A

            //  PRESCALE -> Divis�o do clock dos ADCs
            AdcaRegs.ADCCTL2.bit.PRESCALE = PRESCALE_DIV_4;                     //  ADCCLK(A) = SYSCLK/4

            AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);  //  Configura a resolu��o e o modo de aquisi��o (single-ended / diferencial) do ADC

            AdcaRegs.ADCCTL1.bit.INTPULSEPOS = FIM_CONVERSAO;                   //  EOC � gerado ao fim da convers�o
        #endif

// ------------------------------------------------------------------------------- //
// ******************** AdcbRegs - ADC-B Control Register *************************//
// ------------------------------------------------------------------------------- //

        #ifdef ATIVAR_ADC_B

            //  PRESCALE -> Divis�o do clock dos ADCs
            AdcbRegs.ADCCTL2.bit.PRESCALE = PRESCALE_DIV_4;     //  ADCCLK(A) = SYSCLK/4

            AdcSetMode(ADC_ADCB, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);  //  Configura a resolu��o e o modo de aquisi��o (single-ended / diferencial) do ADC

            AdcbRegs.ADCCTL1.bit.INTPULSEPOS = FIM_CONVERSAO;                   //  EOC � gerado ao fim da convers�o

        #endif


// ------------------------------------------------------------------------------- //
// ******************** AdccRegs - ADC-C Control Register *************************//
// ------------------------------------------------------------------------------- //

        #ifdef ATIVAR_ADC_C

            //  PRESCALE -> Divis�o do clock dos ADCs
            AdccRegs.ADCCTL2.bit.PRESCALE = PRESCALE_DIV_4;     //  ADCCLK(A) = SYSCLK/4

            AdcSetMode(ADC_ADCC, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);  //  Configura a resolu��o e o modo de aquisi��o (single-ended / diferencial) do ADC

            AdccRegs.ADCCTL1.bit.INTPULSEPOS = FIM_CONVERSAO;                   //  EOC � gerado ao fim da convers�o

        #endif

// ------------------------------------------------------------------------------- //
// ******************** AdcdRegs - ADC-D Control Register *************************//
// ------------------------------------------------------------------------------- //

        #ifdef ATIVAR_ADC_D

            //  PRESCALE -> Divis�o do clock dos ADCs
            AdcdRegs.ADCCTL2.bit.PRESCALE = PRESCALE_DIV_4;     //  ADCCLK(A) = SYSCLK/4

            AdcSetMode(ADC_ADCD, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);  //  Configura a resolu��o e o modo de aquisi��o (single-ended / diferencial) do ADC

            AdcdRegs.ADCCTL1.bit.INTPULSEPOS = FIM_CONVERSAO;                   //  EOC � gerado ao fim da convers�o

        #endif

// ******************** Power-up the ADCs *************************//

        #ifdef ATIVAR_ADC_A
            AdcaRegs.ADCCTL1.bit.ADCPWDNZ = ATIVADO;        //  Ativa ADC-A
        #endif

        #ifdef ATIVAR_ADC_B
            AdcbRegs.ADCCTL1.bit.ADCPWDNZ = ATIVADO;        //  Ativa ADC-B
        #endif

        #ifdef ATIVAR_ADC_C
            AdccRegs.ADCCTL1.bit.ADCPWDNZ = ATIVADO;        //  Ativa ADC-C
        #endif

        #ifdef ATIVAR_ADC_D
            AdcdRegs.ADCCTL1.bit.ADCPWDNZ = ATIVADO;        //  Ativa ADC-D
        #endif


    DELAY_US(1000);
    EDIS;       //  Desabilita escrita em registradores protegidos


}


/*---------------------------------------------------------------------------
 * InitBufferSensor:
 *---------------------------------------------------------------------------
 *      Inicializa o buffer que vai ser usador para armazenar as informa��es
 *    dos sensores do sistema.
 */
void InitBufferSensor(SENSOR_BUFFER* Buffer_Sensor){

    unsigned char i;

    Buffer_Sensor->Index = 0;              //  Buffer index tem que ser iniciado com 0 para que funcione a l�gica que checa se o buffer est� cheio
    Buffer_Sensor->Flag_full = 0;          //  0 indica que o buffer n�o est� cheio

    for(i=0 ; i<sizeof(Buffer_Sensor->BUFFER) ; i++){
        Buffer_Sensor->BUFFER[i]->ID = nao_atribuido;       //  Inicializa a identifica��o de todos os sensores com "nao_atribuido"
        Buffer_Sensor->BUFFER[i]->fator_conv = 1;           //  Inicializando com "fator_conv = 1"
    }
}


/*---------------------------------------------------------------------------
 * SetupADC:
 *---------------------------------------------------------------------------
 *      Esta fun��o faz toda a configura��o necess�ria para o funcionamento de uma entrada ADC do DSP.
 *
 *      Parametros:
 *          Conversores_ADC CONVERSOR_ADC:      Conversor que ser� configurado [CONV_ADC_A, CONV_ADC_B, CONV_ADC_C, CONV_ADC_D]
 *          Canais_ADC CANAL_ADC:               Canal do conversor que vai ser usado nesta medida [ADCIN0 ~ ADCIN15]
 *          Resultados_ADC SOC_NUMBER:          Registrador que vai armazenar o valor lido por este ADC [RESULT0 ~ RESULT15]
 *          ADC_TRG TRIGGER:                    Trigger que vai iniciar a convers�o [checar no arquivo ADC.h]
 *          ADC_INT ADC_INT_DEF                 Flag de interrup��o que ser� setada com o sinal EOC [ADC_INT_OFF | ADC_INT1 ~ ADC_INT4]
 *          ADC_INT_SOURCE INT_SOURCE           EOC que vai indicar o fim da convers�o [INT_OFF | INT_EOC0 ~ INT_EOC15]
 *
 *      Exemplo:
 *      SetupADC(CONV_ADC_A, ADCIN2, RESULT0, TRIG_CPU1_TIMER0, ADC_INT_OFF, INT_OFF);
 *      SetupADC(CONV_ADC_C, ADCIN3, RESULT0, TRIG_CPU1_TIMER0, ADC_INT1, INT_EOC0);
 */
void SetupADC(Conversores_ADC CONVERSOR_ADC, Canais_ADC CANAL_ADC, Resultados_ADC SOC_NUMBER, ADC_TRIG TRIGGER, ADC_INT ADC_INT_DEF, ADC_INT_SOURCE INT_SOURCE){

    Uint16 acqps;
    volatile Uint32 *AdcSOCBaseAddr;
    volatile Uint32 mascara = 0;
    //
    //  Determina a janela de aquisi��o m�nima baseado na resolu��o
    //
    if(ADC_RESOLUTION_12BIT == ADC_PTR[CONVERSOR_ADC]->ADCCTL2.bit.RESOLUTION)
    {
        acqps = 14; //14; //75ns
    }
    else //resolution is 16-bit
    {
        acqps = 63; //320ns
    }

    if((CONVERSOR_ADC == CONV_ADC_C) && (CANAL_ADC < ADCIN2)){  //  N�o existe ADCINC0 e ADCINC1
        return;
    }

    EALLOW;     //  Permite escrita em registradores protegidos

        //  SOC -> Configura qual SOC vai ser respons�vel pela leitura de qual entrada do ADC

        AdcSOCBaseAddr = (Uint32 *)ADC_PTR[CONVERSOR_ADC] + (0x10/2) + SOC_NUMBER*(0x2/2);      //  Obt�m o endere�o do registrador AdcxRegs.AdcSOCyCTL

        mascara |= TRIGGER;             //|     Cria a m�scara usada para configurar AdcxRegs.AdcSOCyCTL
        mascara <<= 5;                  //|
        mascara |= CANAL_ADC;           //| TRIGGER     -> Define o trigger que inicia a convers�o
        mascara <<= 15;                 //| CANAL_ADC   -> Define o canal do ADC
        mascara |= acqps;               //| acqps       -> Define a janela de aquisi��o

        *AdcSOCBaseAddr |= mascara;     //  Configura AdcxRegs.AdcSOCyCTL

        /*      Caso seja definida a interrup��o que ser� ativada ao fim da convers�o ent�o a configura��o da interrup��o � feita. Foi feito
            dessa forma para que a interrup��o seja definida apenas na hora de configurar a �ltima ADC que vai ser usada por cada conversor,
            fazendo assim esta configura��o apenas uma vez, mas tamb�m pode ser feita todas as vezes que uma nova entrada ADC for ser configurada.
        */
        if(ADC_INT_DEF != ADC_INT_OFF){

            switch (ADC_INT_DEF){

                case ADC_INT1:
                    ADC_PTR[CONVERSOR_ADC]->ADCINTSEL1N2.bit.INT1SEL = INT_SOURCE;      //  Configura 'INT_SOURCE' como fonte da interrup��o 1 de 'CONVERSOR_ADC'

                    if(ADC_PTR[CONVERSOR_ADC]->ADCINTSEL1N2.bit.INT1E == DESATIVADO){   //|  Ativa a interrup��o 1 de 'CONVERSOR_ADC' caso ainda n�o tenha sido
                        ADC_PTR[CONVERSOR_ADC]->ADCINTSEL1N2.bit.INT1E = ATIVADO;       //| ativada durante a configura��o de outra entrada ADC.
                    }                                                                   //|
                    ADC_PTR[CONVERSOR_ADC]->ADCINTFLGCLR.bit.ADCINT1 = 1;               //  Garante que a flag de INT1 est� limpa

                    break;

                case ADC_INT2:
                    ADC_PTR[CONVERSOR_ADC]->ADCINTSEL1N2.bit.INT2SEL = INT_SOURCE;      //  Configura 'INT_SOURCE' como fonte da interrup��o 2 de 'CONVERSOR_ADC'

                    if(ADC_PTR[CONVERSOR_ADC]->ADCINTSEL1N2.bit.INT2E == DESATIVADO){   //|  Ativa a interrup��o 1 de 'CONVERSOR_ADC' caso ainda n�o tenha sido
                        ADC_PTR[CONVERSOR_ADC]->ADCINTSEL1N2.bit.INT2E = ATIVADO;       //| ativada durante a configura��o de outra entrada ADC.
                    }                                                                   //|
                    ADC_PTR[CONVERSOR_ADC]->ADCINTFLGCLR.bit.ADCINT2 = 1;               //  Garante que a flag de INT2 est� limpa

                    break;

                case ADC_INT3:
                    ADC_PTR[CONVERSOR_ADC]->ADCINTSEL3N4.bit.INT3SEL = INT_SOURCE;      //  Configura 'INT_SOURCE' como fonte da interrup��o 3 de 'CONVERSOR_ADC'

                    if(ADC_PTR[CONVERSOR_ADC]->ADCINTSEL3N4.bit.INT3E == DESATIVADO){   //|  Ativa a interrup��o 1 de 'CONVERSOR_ADC' caso ainda n�o tenha sido
                        ADC_PTR[CONVERSOR_ADC]->ADCINTSEL3N4.bit.INT3E = ATIVADO;       //| ativada durante a configura��o de outra entrada ADC.
                    }                                                                   //|
                    ADC_PTR[CONVERSOR_ADC]->ADCINTFLGCLR.bit.ADCINT3 = 1;               //  Garante que a flag de INT3 est� limpa
                    break;

                case ADC_INT4:
                    ADC_PTR[CONVERSOR_ADC]->ADCINTSEL3N4.bit.INT4SEL = INT_SOURCE;      //  Configura 'INT_SOURCE' como fonte da interrup��o 4 de 'CONVERSOR_ADC'

                    if(ADC_PTR[CONVERSOR_ADC]->ADCINTSEL3N4.bit.INT4E == DESATIVADO){   //|  Ativa a interrup��o 1 de 'CONVERSOR_ADC' caso ainda n�o tenha sido
                        ADC_PTR[CONVERSOR_ADC]->ADCINTSEL3N4.bit.INT4E = ATIVADO;       //| ativada durante a configura��o de outra entrada ADC.
                    }                                                                   //|
                    ADC_PTR[CONVERSOR_ADC]->ADCINTFLGCLR.bit.ADCINT4 = 1;               //  Garante que a flag de INT4 est� limpa
                    break;
            }

        }

    EDIS;       //  Desabilita escrita em registradores protegidos
}


/*---------------------------------------------------------------------------
 * SetupADC_Object:
 *---------------------------------------------------------------------------
 *      Esta fun��o, al�m de fazer toda a configura��o da mesma forma que a
 * fun��o SetupADC, vai criar, e retornar, um objeto do tipo sensor, que vai representar
 * o sensor conectado na entrada anal�gica sendo configurada.
 *
 *      Parametros:
 *          Conversores_ADC CONVERSOR_ADC:      Conversor que ser� configurado [CONV_ADC_A, CONV_ADC_B, CONV_ADC_C, CONV_ADC_D]
 *          Canais_ADC CANAL_ADC:               Canal do conversor que vai ser usado nesta medida [ADCIN0 ~ ADCIN15]
 *          Resultados_ADC SOC_NUMBER:          Registrador que vai armazenar o valor lido por este ADC [RESULT0 ~ RESULT15]
 *          ADC_TRG TRIGGER:                    Trigger que vai iniciar a convers�o [checar no arquivo ADC.h]
 *          ADC_INT ADC_INT_DEF                 Flag de interrup��o que ser� setada com o sinal EOC [ADC_INT_OFF | ADC_INT1 ~ ADC_INT4]
 *          ADC_INT_SOURCE INT_SOURCE           EOC que vai indicar o fim da convers�o [INT_OFF | INT_EOC0 ~ INT_EOC15]
 *          Sensores ID                         Identifica��o �nica para o sensor sendo configurado
 *          Acoplamento CA_CC                   Tipo de acoplamento da leitura do sensor
 *          float32 fatorConv                   Fator que ser� usado para obter o valor real da vari�vel a partir da tens�o lida na entrada anal�gica
 *          MEDIDA* medidas                     Ponteiro para a vari�vel do tipo MEDIDA que vai armazenar os valores medidos pelo sensor
 *
 *      Exemplo:
 *      sensor_tensao_rede = SetupADC_Object(CONV_ADC_A, ADCIN2, RESULT0, TRIG_CPU1_TIMER0, ADC_INT_OFF, INT_OFF, Tensao_rede, AC, 1, &medidas);
 *      sensor_tensao_DC = SetupADC_Object(CONV_ADC_C, ADCIN3, RESULT0, TRIG_CPU1_TIMER0, ADC_INT1, INT_EOC0, Tensao_DC, CC, 1, &medidas);
 */
SENSOR_OBJ SetupADC_Object(Conversores_ADC CONVERSOR_ADC, Canais_ADC CANAL_ADC, Resultados_ADC SOC_NUMBER,
                           ADC_TRIG TRIGGER, ADC_INT ADC_INT_DEF, ADC_INT_SOURCE INT_SOURCE,
                           Sensores ID, Acoplamento CA_CC, float32 fatorConv, MEDIDA* medidas){

    SENSOR_OBJ adcObject;

    SetupADC(CONVERSOR_ADC, CANAL_ADC, SOC_NUMBER, TRIGGER, ADC_INT_DEF, INT_SOURCE);

    //--- Cria o objeto ADC
        adcObject.ID = ID;
        adcObject.modulo_ADC = CONVERSOR_ADC;
        adcObject.Canal = CANAL_ADC;
        adcObject.Result_Reg =  ((Uint16 *)ADC_RESULT_PTR[CONVERSOR_ADC] + (Uint16) SOC_NUMBER);
        adcObject.fator_conv = fatorConv;
        adcObject.valorDigital = &medidas->leituras_dig[ID];
        adcObject.valorReal = &medidas->valor_real[ID];
        adcObject.nivelDC = &medidas->valor_DC[ID];

        medidas->tipo[ID] = CA_CC;

        return adcObject;
}


/*---------------------------------------------------------------------------
 * SetupADC_Buffer:
 *---------------------------------------------------------------------------
 *      Configura um canal ADC para operar da forma desejada.
 *      Esta fun��o tamb�m preenche um buffer com os dados relacionados com as leituras
 *    feitas pelos ADCs, este buffer pode ser usado para facilitar o processo de obten��o
 *    dos valores medidos pelos ADCs, por�m consome mais tempo de processamento.
 */
void SetupADC_Buffer(SENSOR_OBJ* objeto_Sensor, MEDIDA* medidas, SENSOR_BUFFER* buffer_Sensor){

    if( ((buffer_Sensor->Index + 1) % (SENSOR_BUF_SIZE + 1)) == 0){
            buffer_Sensor->Flag_full = 1;
                return;                         // O buffer est� cheio ent�o a fun��o n�o continua
        }

        buffer_Sensor->BUFFER[buffer_Sensor->Index] = objeto_Sensor;   //  Cria o objeto ADC e coloca na �ltima posi��o do buffer

        buffer_Sensor->Index = (buffer_Sensor->Index + 1);     //  Rotaciona a posi��o do buffer
}


/*---------------------------------------------------------------------------
 * SampleADC:
 *---------------------------------------------------------------------------
 *      Esta fun��o retorna o valor que foi aquisitado por uma entrada anal�gica espec�fica.
 *
 *      Parametros:
 *          Conversores_ADC CONVERSOR_ADC:      Conversor que ser� configurado [CONV_ADC_A, CONV_ADC_B, CONV_ADC_C, CONV_ADC_D]
 *          Resultados_ADC SOC_NUMBER:          Registrador RESULTx no qual est� armazenado o resultado da entrada desejada [RESULT0 ~ RESULT15]
 *      Exemplo:
 *          medida_dig = SampleADC(ADC_CONV_A, RESULT2);
 */
Uint16 SampleADC(Conversores_ADC CONVERSOR_ADC, Resultados_ADC SOC_NUMBER){
    return (Uint16) *((Uint16 *)ADC_RESULT_PTR[CONVERSOR_ADC] + (Uint16) SOC_NUMBER);
}


/*---------------------------------------------------------------------------
 * SampleADC_Object:
 *---------------------------------------------------------------------------
 *      Esta fun��o faz a leitura do valor aquisitado por um sensor espec�fico,
 *  converte este valor para o valor real da vari�vel e j� deixa todos os valores
 *  salvos na vari�vel do tipo MEDIDA.
 *
 *      Parametros:
 *          SENSOR_OBJ* sensorObject:      Ponteiro para o objeto sensor que ser� lido
 *      Exemplo:
 *          SampleADC_Object(&sensor_Tensao);
 *
 */
void SampleADC_Object(SENSOR_OBJ* sensorObject){
    *sensorObject->valorDigital = *sensorObject->Result_Reg;
    *sensorObject->valorReal = ( (*sensorObject->valorDigital - *sensorObject->nivelDC) * Kad) * sensorObject->fator_conv;
}


/*---------------------------------------------------------------------------
 * SampleADC_Buffer:
 *---------------------------------------------------------------------------
 *      Faz a leitura dos registro com os resultados de todos os objetos sensores que foram
 *    colocados no buffer, e salva as leituras nas estruturas dos objetos sensores.
 *      Esta fun��o serve para facilitar a escrita do c�digo principal, por�m
 *    � cerca de 7 vezes mais lenta do que fazer diretamente pelo c�digo, ent�o se
 *    a aplica��o tiver problemas de tempo de execu��o n�o � recomendado usar
 *    esta fun��o.
 *      Diretamente pelo c�digo 22 ciclos, usando esta fun��o 151 ciclos [Para a leitura de 3 registros].
 *      Exemplo:
 *          SampleADC_Buffer(&Buffer_Sensores);
 */
void SampleADC_Buffer(SENSOR_BUFFER* buffer_Sensor){
    unsigned char i;

    for (i = 0 ; i<buffer_Sensor->Index ; i++){
        *buffer_Sensor->BUFFER[i]->valorDigital = *buffer_Sensor->BUFFER[i]->Result_Reg;
    }
}

//===========================================================================
// End of file.
//===========================================================================

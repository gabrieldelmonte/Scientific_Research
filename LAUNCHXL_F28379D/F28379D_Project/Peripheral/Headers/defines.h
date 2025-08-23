/*================================== defines.h =================================
                                                                             |
  Defini��es gerais utilizadas pelo c�digo.                                  |
_____________________________________________________________________________|
                                                                             |
                                                        JO�O GABRIEL         |                                                                                                                       |
                                                           24/Mar�o/2022     |
____________________________________________________________________________*/

#ifndef DEFINES_H_
#define DEFINES_H_

/*----------------------------------------------------------------------------*/
/*==================== DEFINI��ES RELACIONADAS AO PROJETO ====================*/
/*----------------------------------------------------------------------------*/
/*  Alterar estas configura��es de acordo com o projeto.
 *
 *  Configura��es:
 *
 *      -> Qual DSP ser� utilizado
 *      -> Quantidade de canais ADC (Sensores) ser�o utilizados
 *      -> Nomes que ser�o dados para os sensores (Manter a definic�o "nao_atribuido" e adicionar ou remover
 *                                                    as outras defini��es de acordo com o projeto)
 *      -> M�dulos ePWM que ser�o utilizados
 *      -> M�dulos ADC que ser�o utilizados (somente para o DSP F28379D)
 */


/*----------------------    DEFINI��O DO DSP    ----------------------*/
    /* Comentar a linha do DSP que N�O ser� utilizado */
//    #define DSP_F28335
    #define DSP_F28379D

/*----------------------------    ADC    ----------------------------*/
    /* Definir o n�mero m�ximo de canais ADC que v�o ser usados para sensoriamento */
    /* Alocar as leituras no m�ximo at� RESULTx, onde x = (SENSOR_BUF_SIZE - 1)
    * Exemplo:   Se SENSOR_BUF_SIZE = 10 somente podem ser usados os
    * registros AdcMirror.ADCRESULT0 ~ AdcMirror.ADCRESULT9
    * para armazenar os resultados, caso seja usado
    * AdcMirror.ADCRESULT10 (ou maior) a leitura n�o vai ser feita.
    */
    #define SENSOR_BUF_SIZE 2 // 3

    /* Definir um nome para os sensores e medidas que ser�o utilizados */
    /* Fazendo atrav�s de um "typedef enum" facilita o processo de debug */
    typedef enum Sensores{
//        Tensao_rede,
        Tensao_DC,
        Corrente_carga,
        nao_atribuido   //  N�O REMOVER
    }Sensores;

    /* Comentar as linhas dos m�dulo ADC que N�O ser�o utilizados
     * (Somente para o F28379D, pois o F28335 tem apenas um m�dulo ADC) */
    #ifdef DSP_F28379D
        #define ATIVAR_ADC_A
        #define ATIVAR_ADC_B
        #define ATIVAR_ADC_C
//        #define ATIVAR_ADC_D
    #endif


    /* Comentar a linha dos m�dulos ePWM que N�O ser�o utilizados */
    #define ATIVAR_EPWM1
//    #define ATIVAR_EPWM2
//    #define ATIVAR_EPWM3
//    #define ATIVAR_EPWM4
//    #define ATIVAR_EPWM5
//    #define ATIVAR_EPWM6
    #ifdef DSP_F28379D                  //  No caso do DSP F28379D s�o 12 m�dulos ePWM
    //    #define ATIVAR_EPWM7
    //    #define ATIVAR_EPWM8
    //    #define ATIVAR_EPWM9
    //    #define ATIVAR_EPWM10
    //    #define ATIVAR_EPWM11
    //    #define ATIVAR_EPWM12
    #endif

/*----------------------------    Interrup��es    ----------------------------*/
    /* Definir quantas interrup��es ser�o usadas*/
    #define MAX_INT 2

    /* Comentar os grupos de interrup��es que N�O ser�o usados */
    #define ATIVAR_INT_GRUPO_1
//    #define ATIVAR_INT_GRUPO_2
//    #define ATIVAR_INT_GRUPO_3
//    #define ATIVAR_INT_GRUPO_4
//    #define ATIVAR_INT_GRUPO_5
//    #define ATIVAR_INT_GRUPO_6
//    #define ATIVAR_INT_GRUPO_7
//    #define ATIVAR_INT_GRUPO_8
//    #define ATIVAR_INT_GRUPO_9
//    #define ATIVAR_INT_GRUPO_10
//    #define ATIVAR_INT_GRUPO_11
//    #define ATIVAR_INT_GRUPO_12


/*----------------------------------------------------------------------------*/
/*================ FIM DAS DEFINI��ES RELACIONADAS AO PROJETO ================*/
/*----------------------------------------------------------------------------*/

    //                                                 20kHz            ||          40kHz
    #define Ts                  0.00002495                  //  Define o per�odo de amostragem      ->    1/20040Hz = 49.9 us   ||      1/40080Hz = 24.95 us
    #define BUF_SIZE_SIGNAL     668                         //  Tamanho do buffer para um per�odo   -> 20040Hz/60Hz = 334       ||   40080Hz/60Hz = 668
    #define divAmostras         0.001497005988              //  = 1/BUF_SIZE_SIGNAL                 ->     0.002994011976       ||   0.001497005988

/*=============================================================================*/

    #define PI_inv 0.31830988618379  // 1/PI

    typedef enum FLAGS_UTEIS{
        DESABILITAR = 0,
        HABILITAR,

        CLEAR = 0,
        SET,

        OFF = 0,
        ON,

        DESATIVADO = 0,
        ATIVADO
    }FLAGS_UTEIS;




//    #define Ts  0.00002495                //  Define o per�odo de amostragem -> 1/40080Hz = 24.95 us
//    #define BUF_SIZE 668                  //  Tamanho do buffer para um per�odo -> 40080Hz/60Hz
//    #define div_BUF   0.0014970059880239  //  = div por 668

//----------Defini��es do sistema
    //
    //  Valores usados:     Sbase = 7.5 kVA
    //                      Vbase1 = 440*sqrt(2) [V]    ;   Vbase2 = 127*sqrt(2) [V]
    //                      Ibase1 = 17.0455 [A]        ;   Ibase2 = 59.0551 [A]
    //                      Zbase1 = 25.8133 [Ohm]      ;   Zbase2 = 2.1505 [Ohm]
    //
    //                      Rf = 0.6 [Ohm]  ;   Lf = 1.5e-3 [H]     ->  Resist�ncia e indut�ncia do lado do filtro
    //                      Rcf = 1 [Ohm]   ;   Cf = 68.6e-6 [F]    ->  Capacit�ncia e resist�ncia do capacitor do filtro
    //                      Rc = 1 [Ohm] ;   C = 136.8e-6 [F]       ->  Capacit�ncia e resist�ncia do banco de capacitores
    //                      Rt = 0.17 [Ohm] ;   Lt = 1,06e-3 [H]    ->  Resist�ncia e indut�ncia do transformador
    //                      Tr = 440/127 = 3.4646                   ->  Rela��o de transforma��o do transformador
    //

    #define S_base 7500
    #define V_base1 440
    #define I_base1 17
    #define Z_base1 25.8133
    #define V_base2 127
    #define I_base2 59
    #define Z_base2 2.1505

    #define div_Vbase1 0.001607061      //  = dividir por 440*sqrt(2) -> valor pu (Vbase1 = 440V)
    #define div_Vbase2 0.005567770      //  = dividir por 127*sqrt(2) -> valor pu (Vbase2 = 127V)
    #define div_Ibase1 0.041594516      //0.058823529      //    = dividir por 17 -> valor pu (Ibase1 = 17.0455A)
    #define div_Ibase2 0.011964582      //0.016920474// 0.016949153      //    = dividir por 59 -> valor pu (Ibase2 = 59.0551)


#endif /* DEFINES_H_ */

/*======================================================================================================================
                                                        Medidas                                                         |
                                                      -----------                                                       |
FILE: medidas.h                                                                                                         |
                                                                                                                        |
    Defini��es e fun��es usadas para armazenar e tratar das medidas feitas.                                             |
________________________________________________________________________________________________________________________|
                                                                                                                        |
     *  Todas as estruturas e fun��es deste arquivo e de "medidas.c" dependem do projeto no qual o algoritmo ser�       |
       aplicado, sendo assim devem ser modificadas de acordo com as necessidades do projeto. A estrutura das fun��es    |
       n�o precisa ser alterada, apenas a quantidade e o nome das vari�veis.                                            |
                                                                                                                        |
    Estruturas:                                                                                                         |
        Val_Real:   Armazena todos os valores das vari�veis sendo usadas pelo algoritmo, ap�s a convers�o de valor      |
                  medida [Uint16] para valor real [float32]. Esta estrutura depende do projeto, ent�o deve ser alterada |
                  de acordo com o algoritmo                                                                             |
        Nivel_DC:   Armazena a informa��o dos n�veis DC dos sensores, usado para fazer a corre��o das leituras. Esta    |
                  estrutura depende do projeto, ent�o deve ser alterada de acordo com o algoritmo                       |
                                                                                                                        |
    Fun��es:                                                                                                            |
        InitMedidas:    Inicializa todas as estruturas relacionadas com os valores medidos                              |
        Conv_Real_pu:   Faz a convers�o dos valores da estrutura "Val_Real" para os seus valores em pu. Para isso �     |
                      necess�rio que os valores de base sejam definidos em "ADC_f28335.h", e que a fun��o seja alterada |
                      em "medidas.c" uma vez que depende do projeto.                                                    |
        DCLevelAdjust:  Subtrai o valor DC do sensor do valor lido pelo ADC.                                            |
                                                                                                                        |
________________________________________________________________________________________________________________________|
                                                                                                                        |
                                                                                                   JO�O GABRIEL         |                                                                                                                       |
                                                                                                      18/Mar�o/2022     |
                                                                                                                        |
 - Ser� que posso universalizar essas coisas usando uma estrutura com vetores? Pensar nisso para uma pr�xima vers�o.    |
=======================================================================================================================*/


#ifndef MEDIDAS_H
#define MEDIDAS_H

    #include "ADC.h"               //  Necess�rio para a defini��o das estruturas "MEDIDA" e "SENSOR_BUFFER

//     #include "Ref_Sincrona.h"   //  Necess�rio para a defini��o da estrutura "SRF_data"


     //---Prot�tipos de fun��es
    void InitMedidas(MEDIDA* medidas);
    void AjusteNivelDC(MEDIDA* medidas);
    void Medida_Real(SENSOR_OBJ* sensor);
//     void Conv_Real_PU(MEDIDA* medidas, Val_Real *valorReal_pu);


     //---Function Prototypes Calculations
//         void AverageValueCalculation(Average_Value *valorMedio, Val_Real *valorReal);    // Talvez mudar o nome da fun��o?
//         void DerivativeCalculation(Derivative_Value *derivada, Average_Value *valorMedio);
//         float RMSCalculation(SRF_data *refSincrona);


#endif

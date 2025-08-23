/*======================================================================================================================
                                                        Medidas                                                         |
                                                      -----------                                                       |
FILE: medidas.c                                                                                                         |
                                                                                                                        |
    Funções usadas para efetuar cálculos e ajustes relacionados nas medidas feitas.                                     |
________________________________________________________________________________________________________________________|
                                                                                                                        |
                                                                                                   JOÃO GABRIEL         |
                                                                                                                        |
                                                                                                    24/Março/2022       |
=======================================================================================================================*/

#include "Peripheral/Headers/medidas.h"

/*---------------------------------------------------------------------------
 * InitMedidas:
 *---------------------------------------------------------------------------
 * Inicializa as structs que armazenam as medidas e os valores relacionados.
 *
 *  Parâmetros:
 *      MEDIDA *medidas: Ponteiro para a struct do tipo "MEDIDA" que armazena todos os dados relacionados às medidas.
 */
void InitMedidas(MEDIDA* medidas){
    unsigned char i;

    for (i=0 ; i<sizeof(medidas->leituras_dig) ; i++){
        medidas->tipo[i] = CA;
        medidas->leituras_dig[i] = 0;
        medidas->valor_real[i] = 0;
        medidas->soma[i] = 0;
        medidas->valor_DC[i] = 0;
    }
    medidas->amostras = 0;
}


/*---------------------------------------------------------------------------
 * Medidas_Real:
 *---------------------------------------------------------------------------
 *  Converte o valor medido pelo ADC para o valor real da variável
 * através do ganho que foi definido ao se criar o objeto sensor e do
 * nível DC calculado.
 *
 *  Parâmetros:
 *      SENSOR_OBJ* sensor: Endereço de memória do sensor
 *
 */
void Medida_Real(SENSOR_OBJ* sensor){
    *sensor->valorReal = ( (*sensor->valorDigital - *sensor->nivelDC) * Kad) * sensor->fator_conv;
}


/*---------------------------------------------------------------------------
 * AjusteNivelDC:
 *---------------------------------------------------------------------------
 *  Realiza o ajuste dos níveis DC dos sensores. Esse processo somente é necessário
 * para o caso de medidas feitas com acoplamento CA.
 *
 *  Parâmetros:
 *      MEDIDA* medidas: Endereço de memória no qual as medidas estão armazenadas
 */
void AjusteNivelDC(MEDIDA* medidas){
    unsigned char i;

    medidas->amostras += 1;

    for(i=0 ; i<sizeof(medidas->leituras_dig) ; i++){       //  Faz a soma das medidas feitas
        if (medidas->tipo[i] == CA){
            medidas->soma[i] = medidas->soma[i] + medidas->leituras_dig[i];
        }
    }

    if(medidas->amostras == BUF_SIZE_SIGNAL){               //|  Caso o tempo de 1 período do sinal tenha sido alcançado,
                                                            //| calcula o nível DC do sinal.
        medidas->amostras = 0;
        for(i=0 ; i<sizeof(medidas->leituras_dig) ; i++){
            if (medidas->tipo[i] == CA){
                medidas->valor_DC[i] = (int16) (medidas->soma[i] * divAmostras);
                medidas->soma[i] = 0;
            }else{
                medidas->valor_DC[i] = 0;
            }
        }

    }



}





/*---------------------------------------------------------------------------
 * Real_pu:
 *---------------------------------------------------------------------------
 * Converte os valores reais das variáveis em valores pu.
 *
 *  Parâmetros:
 *      Val_Real *valorReal: Ponteiro para a struct do tipo "Val_Real" que armazena os valores reais das variáveis.
 *      Val_Real *valorReal_pu: Ponteiro para a struct do tipo "Val_Real" que vai armazenar os valores pu calculados.
 */
//void Conv_Real_PU(MEDIDA* medidas){

//    valorReal_pu->Vsource = valorReal->Vsource * div_Vbase2;
//    valorReal_pu->Vf = valorReal->Vf * div_Vbase1;
//    valorReal_pu->Vt = valorReal->Vt*div_Vbase2;
//    valorReal_pu->I_inv = valorReal->I_inv*div_Ibase1;
//    valorReal_pu->I_load = valorReal->I_load*div_Ibase2;
//    valorReal_pu->I_source = valorReal->I_source*div_Ibase2;
//    valorReal_pu->If = valorReal->If*div_Ibase2;
//    valorReal_pu->Vdc_p1 = valorReal->Vdc_p1*div_Vbase1;
//    valorReal_pu->Vdc_p2 = valorReal->Vdc_p2*div_Vbase1;
//    valorReal_pu->Vdc_p3 = valorReal->Vdc_p3*div_Vbase1;
//}










//==========================Cálculos com as medidas===============================
// * Colocar em outro arquivo?

//---------------------------------------------------------------------------
// AverageValueCalculation:
//---------------------------------------------------------------------------
// Calcula o valor médio de "Vdc", "Vdif_AC" e "Iloop".
//
//  Parâmetros:
//      Average_Value *valorMedio: Ponteiro para struct do tipo "Average_Value" que armazena os valores médios.
//      Val_Real *valorReal: Ponteiro para struct do tipo "Val_Real" que armazena o valor real das variáveis.
//

//void AverageValueCalculation(Average_Value *valorMedio, Val_Real *valorReal){
//
//
//    valorMedio->cont += 1;      // Tempo para o cálculo da média
//
//    if (valorMedio->cont >= AVG_time){      // [obtendo a media a cada 4.165ms] [???]
//
//        valorMedio->Vdc = (valorMedio->sum_Vdc) / AVG_time;             //|
//        valorMedio->Vdif_ac = (valorMedio->sum_Vdif_ac) / AVG_time;     //| Calcula as médias
//        valorMedio->Iloop = (valorMedio->sum_Iloop) / AVG_time;         //|
//
//        valorMedio->cont = 0;           //|
//        valorMedio->sum_Vdc = 0;        //| Reinicializa as variáveis
//        valorMedio->sum_Vdif_ac = 0;    //|
//        valorMedio->sum_Iloop = 0;      //|
//    }
//
//    valorMedio->sum_Vdc = valorMedio->sum_Vdc + fabs(valorReal->Vdc);               //|
//    valorMedio->sum_Vdif_ac = valorMedio->sum_Vdif_ac + fabs(valorReal->Vdif_ac);   //| Atualiza as somas
//    valorMedio->sum_Iloop = valorMedio->sum_Iloop + fabs(valorReal->Iloop);         //|
//
//}


//---------------------------------------------------------------------------
// DerivativeCalculation:
//---------------------------------------------------------------------------
// Calcula a derivada de "Vdif_AC"
//
//  Parâmetros:
//      Derivative_Value *derivada: Ponteiro para struct do tipo "Derivative_Value" que armazena a derivada da variável.
//      Average_Value *valorMedio: Ponteiro para struct do tipo "Average_Value" que armazena os valores médios.
//
//void DerivativeCalculation(Derivative_Value *derivada, Average_Value *valorMedio){
//
//    derivada->cont += 1;    //  Tempo para o cálculo da derivada
//
//    if (derivada->cont <= 1){
//        derivada->eVdif_ac[0] = valorMedio->Vdif_ac;    //  Salva valor atual de "Vdif_ac"
//    }
//    if (derivada->cont >= DERIV_time){                  //  Obtendo a derivada a cada 75us
//        derivada->eVdif_ac[1] = valorMedio->Vdif_ac;    //  Salva valor de "Vdif_ac" após 75us
//        derivada->cont = 0;                             //  Reinicializa a contagem
//    }
//
//    if (derivada->eVdif_ac[0] < 0.00002){               //  Limita o valor [?]
//        derivada->eVdif_ac[0]=0.000021;
//    }
//
//    derivada->derivada_Vdif_ac = fabs( (derivada->eVdif_ac[1] - derivada->eVdif_ac[0]) / (derivada->eVdif_ac[0]) );  //  Calcula a derivada [Isso não deveria estar dentro do loop de 75us ?]
//
//}

//---------------------------------------------------------------------------
// RMSCalculation:
//---------------------------------------------------------------------------
//  Calcula o valor RMS da variável baseado nas componentes DQ obtidas da referências síncrona.
//
//  Parâmtros:
//      SRF_data *refSincrona:  Ponteiro para struct do tipo "SRF_data" que armazena os dados da referência síncrona
//
//float RMSCalculation(SRF_data *refSincrona){
//    float temp;
//
//    temp =  (sqrt( (refSincrona->D_dc * refSincrona->D_dc) + (refSincrona->Q_dc * refSincrona->Q_dc) )) * RAIZ2_2;
//    return temp;
//
//    // Deve funcionar da mesma forma
////    return  (sqrt( (refSincrona->D_dc * refSincrona->D_dc) + (refSincrona->Q_dc * refSincrona->Q_dc) )) * RAIZ2_2;
//}


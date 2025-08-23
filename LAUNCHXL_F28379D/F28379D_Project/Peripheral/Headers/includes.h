/*================================== includes.h =================================
                                                                                |
    Include dos headers dos algoritmos utilizados no projeto.                   |
                                                                                |
    Para cada projeto estes includes podem ser alterados da forma desejada,     |
  sendo assim, para testar um novo algoritmo � apenas necess�rio colocar o      |
  header .h na pasta 'headerFiles_Project', e tamb�m incluir o arquivo source   |
  .c na pasta do projeto.                                                       |
________________________________________________________________________________|
                                                                                |
                                                        JO�O GABRIEL            |                                                                                                                       |
                                                           08/Julho/2022        |
_______________________________________________________________________________*/

#ifndef INCLUDES_H_
#define INCLUDES_H_

//======================================= Headers de algoritmo =======================================//
//
//  Neste trecho devem ser inclu�dos os arquivos header com as defini��es e os prot�tipos de fun��es dos algoritmos
// que v�o ser utilizados no programa.

    #include <stdlib.h>
    #include <stdio.h>
    #include <math.h>

    #include "defines.h"            //  Defini��es gerais usadas no c�digo

    #include "medidas.h"            //  Valores reais das medidas e n�veis DC dos sensores [*Colocar isso no mesmo arquivo do ADC?]
    #include "MaquinaDeEstados.h"   //  Algoritmo da M�quina de Estados

//    #include "PLL.h"                //  PLL por refer�ncia s�ncrona modificada -> Depende de 'IIR_Filter.h' e 'medidas.h'
//    #include "ANF.h"                //  ANF -> Depende de 'PLL.h' e 'medidas.h'

//    #include "PI.h"                 //  Controlador PI
//    #include "PropRes.h"            //  Controlador PR
//    #include "Ref_Sincrona.h"       //  Refer�ncia s�ncrona

//    #include "LowPassFilter.h"      //  Filtros Passa Baixas
//    #include "IIR_Filters.h"        //  Filtros IIR

//    #include "Controle.h"           //  Algoritmo de Controle
//    #include "Protecao.h"           //  Algoritmo de Prote��o do equipamento
//    #include "Acionamentos.h"       //  Acionamentos de dispositivos


//    #include "MPC.h"
//    #include "debug.h"              //  Defini��es e algoritmos usados para fazer debug



#endif /* INCLUDES_H_ */

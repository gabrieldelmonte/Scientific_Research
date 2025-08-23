/*======================================================================================================================
                                                  M�quina de estados                                                    |
                                                 --------------------                                                   |
FILE: MaquinasDeEstados.h                                                                                               |
                                                                                                                        |
    Defini��es usadas pelo algoritmo da m�quina de estados.                                                             |
________________________________________________________________________________________________________________________|
                                                                                                                        |
    A configura��o da m�quina de estados � feita atrav�s da modifica��o do enum "estados" de acordo com o projeto.      |
    Tamb�m � necess�rio colocar os prot�tipos das fun��es que v�o ser executadas para cada estado da m�quina de estados,|
  sendo isso dependente do projeto.                                                                                     |
                                                                                                                        |
                                                                                                                        |
    Baseado na m�quina de estados descrita em:                                                                          |
                                                                                                                        |
        http://isa.uniovi.es/docencia/redes/EmbeddedSatateMachinesImplementation.pdf                                    |
________________________________________________________________________________________________________________________|
                                                                                                                        |
                                                                                                   JO�O GABRIEL         |                                                                                                                       |
                                                                                                      21/Julho/2022     |
                                                                                                                        |
=======================================================================================================================*/

#ifndef MAQUINADEESTADOS_H
#define MAQUINADEESTADOS_H

    #define MAX_STATE_NUMBER    3       //  Define a quantidade de estados da m�quina de estados

    typedef enum estados{           //  Define todos os estados poss�veis da m�quina de estados
        INICIALIZA = 0,
        LEITURA_ADC,
        PWM
    }estados;


    typedef struct SM_Flags{        //  Flags que podem ser usadas pela m�quina de estados para definir o pr�ximo estado
        estados estado_atual;       //  Estado atual da m�quina de estados
        estados estado_anterior;    //  Estado anterior da m�quina de estados
//        erros error_state;        //  Talvez colocar uma flag que indique algum erro, ou relacionada com a prote��o
    }SM_Flags;


    typedef struct SM_Functions{    //  Define as fun��es que a m�quina de estados vai chamar dependendo do estado atual
        void (*function)(void);     //  Ponteiro para a fun��o que ser� executada
    }SM_Functions;


    void SystemInit(void);          //|     Prot�tipo das fun��es que v�o ser executadas pelo
    void ReadADC(void);             //|  algoritmo dependendo do estado atual da m�quina de estados
    void SwitchPWM(void);           //|

#endif

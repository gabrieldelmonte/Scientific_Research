/*======================================================================================================================
                                                  Máquina de estados                                                    |
                                                 --------------------                                                   |
FILE: MaquinasDeEstados.h                                                                                               |
                                                                                                                        |
    Definições usadas pelo algoritmo da máquina de estados.                                                             |
________________________________________________________________________________________________________________________|
                                                                                                                        |
    A configuração da máquina de estados é feita através da modificação do enum "estados" de acordo com o projeto.      |
    Também é necessário colocar os protótipos das funções que vão ser executadas para cada estado da máquina de estados,|
  sendo isso dependente do projeto.                                                                                     |
                                                                                                                        |
                                                                                                                        |
    Baseado na máquina de estados descrita em:                                                                          |
                                                                                                                        |
        http://isa.uniovi.es/docencia/redes/EmbeddedSatateMachinesImplementation.pdf                                    |
________________________________________________________________________________________________________________________|
                                                                                                                        |
                                                                                                   JOÃO GABRIEL         |                                                                                                                       |
                                                                                                      21/Julho/2022     |
                                                                                                                        |
=======================================================================================================================*/

#ifndef MAQUINADEESTADOS_H
#define MAQUINADEESTADOS_H

    #define MAX_STATE_NUMBER    3       //  Define a quantidade de estados da máquina de estados

    typedef enum estados{           //  Define todos os estados possíveis da máquina de estados
        INICIALIZA = 0,
        LEITURA_ADC,
        PWM
    }estados;


    typedef struct SM_Flags{        //  Flags que podem ser usadas pela máquina de estados para definir o próximo estado
        estados estado_atual;       //  Estado atual da máquina de estados
        estados estado_anterior;    //  Estado anterior da máquina de estados
//        erros error_state;        //  Talvez colocar uma flag que indique algum erro, ou relacionada com a proteção
    }SM_Flags;


    typedef struct SM_Functions{    //  Define as funções que a máquina de estados vai chamar dependendo do estado atual
        void (*function)(void);     //  Ponteiro para a função que será executada
    }SM_Functions;


    void SystemInit(void);          //|     Protótipo das funções que vão ser executadas pelo
    void ReadADC(void);             //|  algoritmo dependendo do estado atual da máquina de estados
    void SwitchPWM(void);           //|

#endif

/*======================================================================================================================
                                             Analog to Digital Converter                                                |
                                        ------------------------------------                                            |
FILE: ADC.h                                                                                                             |
                                                                                                                        |
    Todas as defini��es necess�rias para a utilizar o perif�rico ADC.                                                   |
    Dependendo do DSP definido o arquivo j� faz as mudan�as necess�rias nas defini��es.                                 |
                                                                                                                        |
  ** N�o esquecer de configurar 'ADC_BUF_SIZE'                                                                          |
________________________________________________________________________________________________________________________|
                                                                                                                        |
    Estruturas:                                                                                                         |
        MEDIDA:    Armazena todos os valores relacionados com as medidas.                                               |
        SENSOR_OBJECT: Define um objeto sensor.                                                                         |
        SENSOR_BUFFER: Armazena as informa��es de todos os objetos do tipo sensor que v�o ser usados no c�digo.         |
                                                                                                                        |
    Fun��es:                                                                                                            |
        InitADC:    Configura e inicializa o perif�rico ADC.                                                            |
        SetupADC:   Faz todas as configura��es para o funcionamento de uma entrada do ADC.                              |
        SetupADC_Object:    Configura uma entrada do ADC e cria um objeto ADC que representa o sinal lido               |
                          por esta entrada.                                                                             |
        SetupADC_Buffer:    Configura uma entrada do ADC, cria um objeto ADC e coloca este objeto em um buffer          |
                          que vai armazenar todos os objetos.                                                           |
        SampleADC:  Obt�m o valor lido por uma entrada do ADC.                                                          |
        SampleADC_Object:   Obt�m o valor medido relacionado a um objeto ADC.                                           |
        SempleADC_Buffer:   Faz a leitura de todos os objetos ADC.                                                      |
        SocADC:     Inicia a convers�o de valores das entradas configuradas do ADC.                                     |
________________________________________________________________________________________________________________________|
                                                                                                                        |
    Para mais detalhes do funcionamento do ADC, e como configurar da forma desejada os registradores ver o cap�tulo 7   |
  do documento:                                                                                                         |
                                                                                                                        |
    https://www.ti.com/lit/ug/sprui07/sprui07.pdf?ts=1657282731527                                                      |
________________________________________________________________________________________________________________________|
                                                                                                                        |
                                                                                                    JO�O GABRIEL        |                                                                                                                       |
                                                                                                      10/Julho/2022     |
_______________________________________________________________________________________________________________________*/

#ifndef ADC_H
#define ADC_H

#include "Peripheral/Headers/defines.h"
#include "Libraries/Common/F28x_Project.h"

    //--- General Defines

        //  Identifica��o dos conversores ADCs
        typedef enum Conversores_ADC{
            CONV_ADC_A = 1,
            CONV_ADC_B,
            CONV_ADC_C,
            CONV_ADC_D
        }Conversores_ADC;

        //  Identifica��o dos SOCs dos ADCs
        typedef enum Resultados_ADC{
            RESULT0 = 0,
            RESULT1,
            RESULT2,
            RESULT3,
            RESULT4,
            RESULT5,
            RESULT6,
            RESULT7,
            RESULT8,
            RESULT9,
            RESULT10,
            RESULT11,
            RESULT12,
            RESULT13,
            RESULT14,
            RESULT15
        }Resultados_ADC;

    //---Register Related Defines

        // PRESCALE
        typedef enum ADC_PS{
            PRESCALE_DIV_1 = 0x0,       //  PRESCALE ADCCLK = SYSCLK/1
            PRESCALE_DIV_2 = 0x2,       //  PRESCALE ADCCLK = SYSCLK/2
            PRESCALE_DIV_2_5,           //  PRESCALE ADCCLK = SYSCLK/2.5
            PRESCALE_DIV_3,             //  PRESCALE ADCCLK = SYSCLK/3
            PRESCALE_DIV_3_5,           //  PRESCALE ADCCLK = SYSCLK/3.5
            PRESCALE_DIV_4,             //  PRESCALE ADCCLK = SYSCLK/4
            PRESCALE_DIV_4_5,           //  PRESCALE ADCCLK = SYSCLK/4.5
            PRESCALE_DIV_5,             //  PRESCALE ADCCLK = SYSCLK/5
            PRESCALE_DIV_5_5,           //  PRESCALE ADCCLK = SYSCLK/5.5
            PRESCALE_DIV_6,             //  PRESCALE ADCCLK = SYSCLK/6
            PRESCALE_DIV_6_5,           //  PRESCALE ADCCLK = SYSCLK/6.5
            PRESCALE_DIV_7,             //  PRESCALE ADCCLK = SYSCLK/7
            PRESCALE_DIV_7_5,           //  PRESCALE ADCCLK = SYSCLK/7.5
            PRESCALE_DIV_8,             //  PRESCALE ADCCLK = SYSCLK/8
            PRESCALE_DIV_8_5            //  PRESCALE ADCCLK = SYSCLK/8.5
        }ADC_PS;

        // TRIGGER
        typedef enum ADC_TRIG{
            TRIG_SOFTWARE = 0x00,   //  Trigger por software
            TRIG_CPU1_TIMER0,       //  Trigger TIMER0 da CPU1
            TRIG_CPU1_TIMER1,       //  Trigger TIMER1 da CPU1
            TRIG_CPU1_TIMER2,       //  Trigger TIMER2 da CPU1
            TRIG_GPIO_ADCEXTSOC,    //  Trigger atrav�s de GPIO
            TRIG_EPWM1_ADCSOCA,     //  Trigger por EPWM selecionado
            TRIG_EPWM1_ADCSOCB,     //  Trigger por EPWM selecionado
            TRIG_EPWM2_ADCSOCA,     //  Trigger por EPWM selecionado
            TRIG_EPWM2_ADCSOCB,     //  Trigger por EPWM selecionado
            TRIG_EPWM3_ADCSOCA,     //  Trigger por EPWM selecionado
            TRIG_EPWM3_ADCSOCB,     //  Trigger por EPWM selecionado
            TRIG_EPWM4_ADCSOCA,     //  Trigger por EPWM selecionado
            TRIG_EPWM4_ADCSOCB,     //  Trigger por EPWM selecionado
            TRIG_EPWM5_ADCSOCA,     //  Trigger por EPWM selecionado
            TRIG_EPWM5_ADCSOCB,     //  Trigger por EPWM selecionado
            TRIG_EPWM6_ADCSOCA,     //  Trigger por EPWM selecionado
            TRIG_EPWM6_ADCSOCB,     //  Trigger por EPWM selecionado
            TRIG_EPWM7_ADCSOCA,     //  Trigger por EPWM selecionado
            TRIG_EPWM7_ADCSOCB,     //  Trigger por EPWM selecionado
            TRIG_EPWM8_ADCSOCA,     //  Trigger por EPWM selecionado
            TRIG_EPWM8_ADCSOCB,     //  Trigger por EPWM selecionado
            TRIG_EPWM9_ADCSOCA,     //  Trigger por EPWM selecionado
            TRIG_EPWM9_ADCSOCB,     //  Trigger por EPWM selecionado
            TRIG_EPWM10_ADCSOCA,    //  Trigger por EPWM selecionado
            TRIG_EPWM10_ADCSOCB,    //  Trigger por EPWM selecionado
            TRIG_EPWM11_ADCSOCA,    //  Trigger por EPWM selecionado
            TRIG_EPWM11_ADCSOCB,    //  Trigger por EPWM selecionado
            TRIG_EPWM12_ADCSOCA,    //  Trigger por EPWM selecionado
            TRIG_EPWM12_ADCSOCB,    //  Trigger por EPWM selecionado
            TRIG_CPU2_TIMER0,       //  Trigger TIMER0 da CPU2
            TRIG_CPU2_TIMER1,       //  Trigger TIMER1 da CPU2
            TRIG_CPU2_TIMER2,       //  Trigger TIMER2 da CPU2
        }ADC_TRIG;

        //CANAIS DE ENTRADA
        typedef enum Canais_ADC{
            ADCIN0 = 0x0,           //  ADCIN0 ou diferencial ADCIN0(P) e ADCIN1(N)
            ADCIN1,                 //  ADCIN1 ou diferencial ADCIN0(P) e ADCIN1(N)
            ADCIN2,                 //  ADCIN2 ou diferencial ADCIN2(P) e ADCIN3(N)
            ADCIN3,                 //  ADCIN3 ou diferencial ADCIN2(P) e ADCIN3(N)
            ADCIN4,                 //  ADCIN4 ou diferencial ADCIN4(P) e ADCIN5(N)
            ADCIN5,                 //  ADCIN5 ou diferencial ADCIN4(P) e ADCIN5(N)
            ADCIN14 = 0xE,          //  ADCIN14 ou diferencial ADCIN14(P) e ADCIN15(N)
            ADCIN15                 //  ADCIN15 ou diferencial ADCIN14(P) e ADCIN15(N)
        }Canais_ADC;

        //EOC
        typedef enum ADC_EOC{
            FIM_SH = 0,             //  EOC gerado ao final do S/H
            FIM_CONVERSAO           //  EOC gerado ao final da convers�o
        }ADC_EOC;

        //FONTE DE INTERRUP��O
        typedef enum ADC_INT_SOURCE{
            INT_EOC0 = 0x0,         //  EOC0 como fonte de interrupc�o do ADC
            INT_EOC1,               //  EOC1 como fonte de interrupc�o do ADC
            INT_EOC2,               //  EOC2 como fonte de interrupc�o do ADC
            INT_EOC3,               //  EOC3 como fonte de interrupc�o do ADC
            INT_EOC4,               //  EOC4 como fonte de interrupc�o do ADC
            INT_EOC5,               //  EOC5 como fonte de interrupc�o do ADC
            INT_EOC6,               //  EOC6 como fonte de interrupc�o do ADC
            INT_EOC7,               //  EOC7 como fonte de interrupc�o do ADC
            INT_EOC8,               //  EOC8 como fonte de interrupc�o do ADC
            INT_EOC9,               //  EOC9 como fonte de interrupc�o do ADC
            INT_EOC10,              //  EOC10 como fonte de interrupc�o do ADC
            INT_EOC11,              //  EOC11 como fonte de interrupc�o do ADC
            INT_EOC12,              //  EOC12 como fonte de interrupc�o do ADC
            INT_EOC13,              //  EOC13 como fonte de interrupc�o do ADC
            INT_EOC14,              //  EOC14 como fonte de interrupc�o do ADC
            INT_EOC15,              //  EOC15 como fonte de interrupc�o do ADC
            INT_OFF                 //  N�o vai ser gerada uma interrup��o
        }ADC_INT_SOURCE;

        typedef enum ADC_INT{
            ADC_INT_OFF = 0x0,      //  N�o vai ser gerada uma interrup��o
            ADC_INT1,               //  EOC gera interrup��o INT1
            ADC_INT2,               //  EOC gera interrup��o INT2
            ADC_INT3,               //  EOC gera interrup��o INT3
            ADC_INT4                //  EOC gera interrup��o INT4
        }ADC_INT;


    //---Structure & Constants Defines

        #define Kad 0.000732421875    //  Ganho do AD val. digital(int) para tens�o medida (float)

        typedef enum Acoplamento{   //      Define os padr�es de acoplamento das medidas
            CA = 0,                 //      Nas medidas feitas com acoplamento CA (AC) o n�vel DC � removido
            CC = 1,                 //   das medidas na fun��o "AjusteNivelDC".
            AC = 0,
            DC = 1
        }Acoplamento;

//        typedef struct{
//            float32 medida_sensor[SENSOR_BUF_SIZE];
//        }Val_Real;

        typedef struct{             //-->Struct que vai armazenar todos os dados relacionados com as medidas feitas
            unsigned int amostras;                      //  Contagem de amostras para o c�lculo do n�vel DC do sensor
            Acoplamento tipo[SENSOR_BUF_SIZE];          //  Indica o tipo de acoplamento da medida [CA] ou [CC]
            int16 leituras_dig[SENSOR_BUF_SIZE];        //  Valor armazenado no registro do ADC
            float32 valor_real[SENSOR_BUF_SIZE];        //  Valor convertido em valor real da medida
            float32 valor_pu[SENSOR_BUF_SIZE];          //  Valor real convertido em pu
            int16 valor_DC[SENSOR_BUF_SIZE];            //  Valor DC do sensor
            float32 soma[SENSOR_BUF_SIZE];              //  Soma total de um per�odo de medidas, usada para c�lculo do n�vel DC do sensor
        }MEDIDA;

        typedef struct{                     //-->Struct que define um objeto sensor, com todas as caracter�sticas necess�rias
            Sensores ID;                //  ID do objeto sensor
            int16* valorDigital;        //  Endere�o no qual ser� salvo o valor lido pelo ADC
            float32* valorReal;         //  Endere�o no qual ser� salvo o valor real da vari�vel medida
            int16* nivelDC;             //  Endere�o no qual est� salvo o n�vel DC calculado para esta medida
            float32 fator_conv;         //  Valor do fator usado para se converter o valor de tens�o medido pelo ADC no valor real da vari�vel
            Conversores_ADC modulo_ADC;  //  M�dulo ADC no qual o sensor est� conectado
            Canais_ADC Canal;           //  Canal do m�dulo ADC no qual o sensor est� conectado
            Uint16* Result_Reg;         //  Endere�o no qual o resultado da leitura deste objeto ADC vai ficar armazenado [&AdcMirror.RESULTxx]
        }SENSOR_OBJ;

        typedef struct{         //-->Struct que armazena a informa��o de todos os objetos sensores
            unsigned char Index;                //  Quantidade de objetos que foram salvos no buffer
            unsigned char Flag_full;            //  Indica que o buffer est� cheio
            SENSOR_OBJ* BUFFER[SENSOR_BUF_SIZE];   //  Buffer com os endere�os dos objetos sensores
        }SENSOR_BUFFER;


        //---Function Prototypes
        void InitADC(void);

        void SetupADC(Conversores_ADC CONVERSOR_ADC, Canais_ADC CANAL_ADC,
                      Resultados_ADC SOC_NUMBER, ADC_TRIG TRIGGER, ADC_INT ADC_INT, ADC_INT_SOURCE INT_SOURCE);
        SENSOR_OBJ SetupADC_Object(Conversores_ADC CONVERSOR_ADC, Canais_ADC CANAL_ADC, Resultados_ADC SOC_NUMBER,
                                   ADC_TRIG TRIGGER, ADC_INT ADC_INT_DEF, ADC_INT_SOURCE INT_SOURCE, Sensores ID, Acoplamento CA_CC,
                                   float32 fatorConv, MEDIDA* medidas);

        Uint16 SampleADC(Conversores_ADC CONVERSOR_ADC, Resultados_ADC SOC_NUMBER);
        void SampleADC_Object(SENSOR_OBJ* sensorObject);
        void SampleADC_Buffer(SENSOR_BUFFER* buffer_Sensor);

        //void SocADC(void);

        void InitBufferSensor(SENSOR_BUFFER* Buffer_Sensor);
        void SetupADC_Buffer(SENSOR_OBJ* objeto_Sensor, MEDIDA* medidas, SENSOR_BUFFER* buffer_Sensor);

#endif

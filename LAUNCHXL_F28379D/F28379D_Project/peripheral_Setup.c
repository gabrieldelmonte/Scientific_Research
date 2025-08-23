/**
 * @file peripheral_Setup.c
 * @brief Peripheral initialization for F28379D Buck Converter Control
 * @author Gabriel Del Monte
 * @date 2025
 */

#include "peripheral_Setup.h"

// Global variables
Int_Vect int_vectors = { { {grupo_1, interrupt_7} } };

MEDIDA medidasADC;
uint16_t i2c_status = 0;
uint16_t pwm_factor = 0;

uint16_t hours = 0;
uint16_t minutes = 0;
uint16_t seconds = 0;

uint16_t hours_decimal = 0;
uint16_t minutes_decimal = 0;
uint16_t seconds_decimal = 0;

float duty_cycle = 0.0f;

char system_state = OFF;


SetpointFilter setpoint_filter = {
    .pos = 0,
    .values = {0.0f},
    .setpoint = 0.0f
};

InputMonitor input_monitor = {
    .conv_factor = INPUT_CONVERSION_FACTOR,
    .raw = 0,
    .voltage = 0.0f
};

/**
 * @brief Timer 0 interrupt service routine
 * Handles ADC reading, button monitoring, and setpoint calculation
 */
interrupt void timer0_isr(void) {
    ServiceDog();

    static const int filter_size = sizeof(setpoint_filter.values) / sizeof(setpoint_filter.values[0]);
    int index;

    // Wait for ADC completion
    while (!AdccRegs.ADCINTFLG.bit.ADCINT1);
    AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;

    // Button monitoring
    if (!GpioDataRegs.GPCDAT.bit.GPIO67)
        system_state = ON;
    else if (!GpioDataRegs.GPDDAT.bit.GPIO111) {
        EPWM1_Modulante_CMPA = 0;
        GpioDataRegs.GPASET.bit.GPIO31 = 1;
        GpioDataRegs.GPBSET.bit.GPIO34 = 1;
        system_state = OFF;
    }

    // ADC data processing
    if (system_state) {
        medidasADC.leituras_dig[Tensao_DC] = AdcbResultRegs.ADCRESULT0;
        medidasADC.valor_real[Tensao_DC] = medidasADC.leituras_dig[Tensao_DC] * VOLTAGE_CONVERSION_FACTOR;

        medidasADC.leituras_dig[Corrente_carga] = AdccResultRegs.ADCRESULT0;
        medidasADC.valor_real[Corrente_carga] = medidasADC.leituras_dig[Corrente_carga] * CURRENT_CONVERSION_FACTOR;
    }

    // Input voltage monitoring
    input_monitor.raw = AdcbResultRegs.ADCRESULT1;
    input_monitor.voltage = (input_monitor.raw * input_monitor.conv_factor);

    // Setpoint calculation with rolling average
    setpoint_filter.setpoint -= setpoint_filter.values[setpoint_filter.pos];

    setpoint_filter.values[setpoint_filter.pos] = ((AdcaResultRegs.ADCRESULT0 / MAX_ADC) * MAX_VOLTAGE) / filter_size;
    setpoint_filter.setpoint += setpoint_filter.values[setpoint_filter.pos];

    setpoint_filter.pos = (setpoint_filter.pos + 1) % filter_size;

    // Safety limiting
    if (setpoint_filter.setpoint > (0.95f * input_monitor.voltage)) {
        setpoint_filter.setpoint = input_monitor.voltage;

        for (index = 0; index < filter_size; index++)
            setpoint_filter.values[index] = input_monitor.voltage / filter_size;
    }

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

/**
 * @brief Initialize GPIO pins
 */
void gpio_init(void) {
    InitSysCtrl();
    InitGpio();

    // Status LEDs (active low)
    ConfigGPIO(31, SAIDA, NULL, NULL, NULL, NULL, NULL);
    GpioDataRegs.GPASET.bit.GPIO31 = 1;

    ConfigGPIO(34, SAIDA, NULL, NULL, NULL, NULL, NULL);
    GpioDataRegs.GPBSET.bit.GPIO34 = 1;

    // Buttons (with pull-up)
    ConfigGPIO(67, ENTRADA, PULLUP_ATIVADO, INV_DESATIVADO, OPENDRAIN_DESATIVADO, QUAL3, 100);
    ConfigGPIO(111, ENTRADA, PULLUP_ATIVADO, INV_DESATIVADO, OPENDRAIN_DESATIVADO, QUAL3, 100);

    return;
}

/**
 * @brief Initialize ADC modules
 */
void adc_init(void) {
    InitADC();

    SetupADC(CONV_ADC_A, ADCIN2, RESULT0, TRIG_CPU1_TIMER0, ADC_INT_OFF, INT_OFF);  // Setpoint
    SetupADC(CONV_ADC_B, ADCIN2, RESULT1, TRIG_CPU1_TIMER0, ADC_INT_OFF, INT_OFF);  // Input voltage
    SetupADC(CONV_ADC_B, ADCIN3, RESULT0, TRIG_CPU1_TIMER0, ADC_INT_OFF, INT_OFF);  // Voltage
    SetupADC(CONV_ADC_C, ADCIN3, RESULT0, TRIG_CPU1_TIMER0, ADC_INT1, INT_EOC0);    // Current

    InitMedidas(&medidasADC);
    medidasADC.tipo[Tensao_DC] = DC;
    medidasADC.tipo[Corrente_carga] = AC;

    return;
}

/**
 * @brief Initialize PWM module for 20kHz switching
 */
void pwm_init(void) {
    StartEPWMConfig();

    ConfigEPwm_REF(EPWM1, ePWM_HSPCLKDIV_1, ePWM_CLKDIV_1, 20000);
    pwm_factor = DutyCycle(ePWM_HSPCLKDIV_1, ePWM_CLKDIV_1, 20000);

    ConfigSyncPWMs();
    InitEPwmGpio();
    EndEPWMConfig();

    EPWM1_Modulante_CMPA = 0;

    return;
}

/**
 * @brief Initialize watchdog timer
 */
void watchdog_init(void) {
    ConfigWatchdog(RESET);
    ServiceDog();
    EnableWatchdog(WD_PS_1);

    return;
}

/**
 * @brief Initialize DAC module
 */
void dac_init(void) {
    return;
}

/**
 * @brief Initialize UART communication (9600 baud)
 */
void uart_init(void) {
    EALLOW;

        // Configure GPIO18 as SCITX_B
        GpioCtrlRegs.GPAGMUX2.bit.GPIO18 = 0;
        GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 2;
        GpioCtrlRegs.GPAPUD.bit.GPIO18 = 1;
        GpioCtrlRegs.GPADIR.bit.GPIO18 = 1;

        // Configure GPIO19 as SCIRX_B
        GpioCtrlRegs.GPAGMUX2.bit.GPIO19 = 0;
        GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 2;
        GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;
        GpioCtrlRegs.GPADIR.bit.GPIO19 = 0;
        GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3;

    EDIS;

    // 1 stop bit
    // No loopback
    // No parity
    // 8 char bits
    // Async mode
    // Idle-line protocol
    // enable TX/RX
    // Internal SCICLK
    // Disable RX ERR, SLEEP, TXWAKE
    ScibRegs.SCICCR.all = 0x0007;
    ScibRegs.SCICTL1.all = 0x0003;
    ScibRegs.SCICTL2.all = 0x0003;
    ScibRegs.SCICTL2.bit.TXINTENA = 0;
    ScibRegs.SCICTL2.bit.RXBKINTENA = 0;

    // SCIB at 9600 baud
    // @LSPCLK = 50 MHz (200 MHz SYSCLK) HBAUD = 0x02 and LBAUD = 0x8B.
    ScibRegs.SCIHBAUD.all = 0x02;
    ScibRegs.SCILBAUD.all = 0x8B;

    // Initialize the SCI FIFO
    ScibRegs.SCIFFTX.all = 0xC022;
    ScibRegs.SCIFFRX.all = 0x0028;
    ScibRegs.SCIFFCT.all = 0x0000;

    // Relinquish SCI from Reset
    ScibRegs.SCICTL1.all = 0x0023;
    ScibRegs.SCIFFTX.bit.TXFIFORESET = 1;
    ScibRegs.SCIFFRX.bit.RXFIFORESET = 1;

    return;
}

/**
 * @brief Initialize SPI module
 */
void spi_init(void) {
    return;
}

/**
 * @brief Initialize I2C module
 */
void i2c_init(void) {
    EALLOW;

        // Configure GPIO104 as SDA_A
        GpioCtrlRegs.GPDGMUX1.bit.GPIO104 = 0;
        GpioCtrlRegs.GPDMUX1.bit.GPIO104 = 1;

        // Configure GPIO105 SCL_A
        GpioCtrlRegs.GPDGMUX1.bit.GPIO105 = 0;
        GpioCtrlRegs.GPDMUX1.bit.GPIO105 = 1;

        // Enable internal pull-up resistors
        GpioCtrlRegs.GPDPUD.bit.GPIO104 = 0;    // Enable pull-up for SDA
        GpioCtrlRegs.GPDPUD.bit.GPIO105 = 0;    // Enable pull-up for SCL

        // Set qualification for I2C pins (asynchronous input)
        GpioCtrlRegs.GPDQSEL1.bit.GPIO104 = 3;
        GpioCtrlRegs.GPDQSEL1.bit.GPIO105 = 3;

        // Enable I2C_A peripheral clock
        CpuSysRegs.PCLKCR9.bit.I2C_A = 1;

    EDIS;

    // I2C Module Configuration
    // Set prescaler for 7-12MHz module clock
    I2caRegs.I2CPSC.all = 6;                    // (200MHz / (6+1)) = ~28.6MHz, need to reduce further

    // Set clock dividers for ~100kHz I2C clock
    // I2C_CLK = Module_CLK / ((CLKL + d) + (CLKH + d)) where d=5 for prescaler >= 1
    I2caRegs.I2CCLKL = 10;                      // Low period
    I2caRegs.I2CCLKH = 5;                       // High period

    // Disable all interrupts initially
    I2caRegs.I2CIER.all = 0x0000;

    // Configure I2C mode register
    I2caRegs.I2CMDR.all = 0x0020;               // Take I2C out of reset, free run mode

    // Configure FIFO
    I2caRegs.I2CFFTX.all = 0x6000;              // Enable FIFO mode and TXFIFO
    I2caRegs.I2CFFRX.all = 0x2040;              // Enable RXFIFO, clear RXFFINT

    i2c_status = ds3231_init_and_set_zero();

    DELAY_US(1000);
    if (i2c_status == I2C_SUCCESS)
        return;
    else
        asm("ESTOP0");
}

/**
 * @brief Initialize interrupt system and Timer0 (50ms period)
 */
void interrupt_init(void) {
    InitPieCtrl();
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();

    EALLOW;
        PieVectTable.TIMER0_INT = &timer0_isr;
    EDIS;

    InitCpuTimers();
    ConfigCpuTimer(&CpuTimer0, 100, 50);
    ConfigInterrupt(int_vectors);
    StartCpuTimer0();

    return;
}

/**
 * @brief Initialize all peripherals
 */
void peripheral_Setup(void) {
    gpio_init();
    adc_init();
    pwm_init();
    watchdog_init();
    //  dac_init();
    uart_init();
    //  spi_init();
    i2c_init();
    interrupt_init();

    return;
}

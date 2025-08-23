/**
 * @file peripheral_Setup.h
 * @brief Peripheral setup for F28379D Buck Converter Control
 * @author Gabriel Del Monte
 * @date 2025
 */

#ifndef PERIPHERAL_SETUP_H
#define PERIPHERAL_SETUP_H

    #include "Peripheral/includes_FirmWare.h"

    #include "Libraries/Common/F28x_Project.h"
    #include "Libraries/Common/F2837xD_Gpio_defines.h"
    #include "Libraries/Common/F2837xD_GlobalPrototypes.h"

    #include "Libraries/Driverlib/driver_inclusive_terminology_mapping.h"
    #include "Libraries/Driverlib/sci.h"
    #include "Libraries/Driverlib/gpio.h"

    #include "Libraries/Headers/F2837xD_gpio.h"
    #include "Libraries/Headers/F2837xD_adc.h"
    #include "Libraries/Headers/F2837xD_i2c.h"
    #include "Libraries/Headers/F2837xD_device.h"
    #include "Libraries/Headers/F2837xD_sysctrl.h"
    #include "Libraries/Headers/F2837xD_spi.h"

    #include "Peripheral/Headers/ADC.h"
    #include "Peripheral/Headers/defines.h"
    #include "Peripheral/Headers/EPWM.h"
    #include "Peripheral/Headers/GPIO.h"
    #include "Peripheral/Headers/Watchdog.h"

    #include "sys/_stdint.h"

    // System configuration
    #define VOLTAGE_CONVERSION_FACTOR   0.0060f
    #define CURRENT_CONVERSION_FACTOR   0.6300f
    #define INPUT_CONVERSION_FACTOR     0.0045f

    #define MAX_VOLTAGE                 10.0f
    #define MAX_CURRENT                 1.00f
    #define MAX_CURRENT_mA              (MAX_CURRENT * 1000.0f)
    #define MAX_ADC                     4095.0f

    #define SIZE_ADC_READINGS           10

    #define CPU_FREQ                    200E6
    #define LSPCLK_FREQ                 (CPU_FREQ/4)
    #define SCI_FREQ                    9600
    #define SCI_PRD                     ((LSPCLK_FREQ/(SCI_FREQ*8))-1)

    #define ON                          1
    #define OFF                         0

    // DS3231 I2C Address and Register Definitions
    #define DS3231_I2C_ADDR             0x68
    #define DS3231_REG_SECONDS          0x00
    #define DS3231_REG_MINUTES          0x01
    #define DS3231_REG_HOURS            0x02
    #define DS3231_REG_CONTROL          0x0E
    #define DS3231_REG_STATUS           0x0F
    #define I2C_TIMEOUT_ERROR           0x04

    /**
     * @brief Setpoint with rolling average filter
     */
    typedef struct {
        unsigned int pos;
        float values[SIZE_ADC_READINGS];
        float setpoint;
    } SetpointFilter;

    /**
     * @brief Input voltage monitoring
     */
    typedef struct {
        unsigned int raw;
        float conv_factor;
        float voltage;
    } InputMonitor;

    // Global variables
    extern SetpointFilter setpoint_filter;
    extern InputMonitor input_monitor;

    // Function prototypes
    interrupt void timer0_isr(void);

    void gpio_init(void);

    void adc_init(void);

    void pwm_init(void);

    void watchdog_init(void);

    void dac_init(void);

    void uart_init(void);

    void spi_init(void);

    void i2c_init(void);

    void interrupt_init(void);

    void peripheral_Setup(void);

    // Auxiliary inline functions

    /**
     * @brief Convert BCD to decimal
     * @param bcd BCD value
     * @return Decimal value
     */
    inline Uint16 bcd_to_decimal(Uint16 bcd) {
        return ((bcd >> 4) * 10) + (bcd & 0x0F);
    }

    /**
     * @brief Convert integer to string
     * @param value 32-bit integer value
     * @param buffer Buffer to hold the string representation
     * @return Length of the string
     */
    inline int int_to_char(int value, char* buffer) {
        char is_negative = 0;
        int i = 0;
        int j = 0;

        // Handle negative numbers
        if (value < 0) {
            is_negative = 1;
            value = -value;
        }

        // Process individual digits
        do {
            buffer[i++] = (value % 10) + '0';
            value /= 10;
        } while (value != 0);

        // Add negative sign if needed
        if (is_negative)
            buffer[i++] = '-';

        buffer[i] = '\0';

        // Reverse the string
        for (j = 0; j < i / 2; j++) {
            char temp = buffer[j];
            buffer[j] = buffer[i - j - 1];
            buffer[i - j - 1] = temp;
        }

        // Return length of the string
        return i;
    }

    // UART inline functions

    /**
     * @brief Send a character over UART
     * @param data Character to send
     */
    inline void uart_send_char(char data) {
        while (!ScibRegs.SCICTL2.bit.TXRDY);
        ScibRegs.SCITXBUF.all = data;

        return;
    }

    inline void uart_send_string(const char *str) {
        while (*str) {
            uart_send_char(*str++);
        }

        return;
    }

    /**
     * @brief Send an integer over UART
     * @param data Integer to send
     */
    inline void uart_send_int(int data) {
        char buffer[12];
        char *p;

        int_to_char(data, buffer);

        for (p = buffer; *p != '\0'; p++)
            uart_send_char(*p);

        return;
    }


    // I2C inline functions
    /**
     * @brief Wait for I2C bus to be ready
     * @return I2C_SUCCESS if ready, error code otherwise
     */
    inline Uint16 i2c_wait_bus_ready(void) {
        Uint32 timeout = 10000;

        // Wait until STP bit is cleared
        while (I2caRegs.I2CMDR.bit.STP == 1 && timeout > 0) {
            timeout--;
            DELAY_US(1);
        }

        if (timeout == 0)
            return I2C_STP_NOT_READY_ERROR;

        // Check if bus is busy
        if (I2caRegs.I2CSTR.bit.BB == 1)
            return I2C_BUS_BUSY_ERROR;

        return I2C_SUCCESS;
    }


    // DS3231 RTC functions

    /**
     * @brief Write data to DS3231 register
     * @param reg_addr Register address
     * @param data Data to write
     * @return I2C_SUCCESS if successful, error code otherwise
     */
    inline Uint16 ds3231_write_register(Uint16 reg_addr, Uint16 data) {
        Uint16 status;
        Uint32 timeout = 10000;

        // Wait for bus to be ready
        status = i2c_wait_bus_ready();
        if (status != I2C_SUCCESS)
            return status;

        // Set slave address
        I2caRegs.I2CSAR.all = DS3231_I2C_ADDR;

        // Set number of bytes to send (register address + data)
        I2caRegs.I2CCNT = 2;

        // Load data into transmit register
        I2caRegs.I2CDXR.all = reg_addr;   // Register address
        I2caRegs.I2CDXR.all = data;       // Data

        // Start transmission (Master, Transmit, Start, Stop)
        I2caRegs.I2CMDR.all = 0x6E20;

        // Wait for stop condition to be detected
        while (I2caRegs.I2CSTR.bit.SCD == 0 && timeout > 0) {
            // Check for NACK
            if (I2caRegs.I2CSTR.bit.NACK == 1) {
                I2caRegs.I2CSTR.all = I2caRegs.I2CSTR.all; // Clear NACK
                I2caRegs.I2CMDR.bit.STP = 1;               // Generate stop
                return I2C_NACK_ERROR;
            }

            timeout--;
            DELAY_US(1);
        }

        if (timeout == 0)
            return I2C_TIMEOUT_ERROR;

        // Clear stop condition detected flag
        I2caRegs.I2CSTR.bit.SCD = 1;

        return I2C_SUCCESS;
    }

    /**
     * @brief Read data from DS3231 register
     * @param reg_addr Register address
     * @param data Pointer to store read data
     * @return I2C_SUCCESS if successful, error code otherwise
     */
    inline Uint16 ds3231_read_register(Uint16 reg_addr, Uint16* data) {
        Uint16 status;
        Uint32 timeout = 10000;

        // First, write the register address (without stop condition)
        status = i2c_wait_bus_ready();
        if (status != I2C_SUCCESS)
            return status;

        I2caRegs.I2CSAR.all = DS3231_I2C_ADDR;
        I2caRegs.I2CCNT = 1;
        I2caRegs.I2CDXR.all = reg_addr;

        // Start transmission without stop condition
        I2caRegs.I2CMDR.all = 0x2620;

        // Wait for ARDY (address ready)
        timeout = 10000;
        while (I2caRegs.I2CSTR.bit.ARDY == 0 && timeout > 0) {
            if (I2caRegs.I2CSTR.bit.NACK == 1) {
                I2caRegs.I2CSTR.all = I2caRegs.I2CSTR.all;
                I2caRegs.I2CMDR.bit.STP = 1;
                return I2C_NACK_ERROR;
            }

            timeout--;
            DELAY_US(1);
        }

        if (timeout == 0)
            return I2C_TIMEOUT_ERROR;

        // Clear ARDY flag
        I2caRegs.I2CSTR.bit.ARDY = 1;

        // Now read the data with restart condition
        I2caRegs.I2CCNT = 1;
        I2caRegs.I2CMDR.all = 0x2C20; // Restart as master receiver with stop

        // Wait for stop condition
        timeout = 10000;
        while (I2caRegs.I2CSTR.bit.SCD == 0 && timeout > 0) {
            timeout--;
            DELAY_US(1);
        }

        if (timeout == 0)
            return I2C_TIMEOUT_ERROR;

        // Read received data
        *data = I2caRegs.I2CDRR.all;

        // Clear stop condition detected flag
        I2caRegs.I2CSTR.bit.SCD = 1;

        return I2C_SUCCESS;
    }

    /**
     * @brief Set DS3231 time to 00:00:00
     * @return I2C_SUCCESS if successful, error code otherwise
     */
    inline Uint16 ds3231_set_time_zero(void) {
        Uint16 status;

        // Set seconds to 00 (0x00 in BCD)
        status = ds3231_write_register(DS3231_REG_SECONDS, 0x00);
        if (status != I2C_SUCCESS)
            return status;

        // Small delay between writes
        DELAY_US(100);

        // Set minutes to 00 (0x00 in BCD)
        status = ds3231_write_register(DS3231_REG_MINUTES, 0x00);
        if (status != I2C_SUCCESS)
            return status;

        // Small delay between writes
        DELAY_US(100);

        // Set hours to 00 in 24-hour format (0x00 in BCD, bit 6 = 0 for 24-hour mode)
        status = ds3231_write_register(DS3231_REG_HOURS, 0x00);
        if (status != I2C_SUCCESS)
            return status;

        return I2C_SUCCESS;
    }

    /**
     * @brief Initialize DS3231 RTC and set time to 00:00:00
     * @return I2C_SUCCESS if successful, error code otherwise
     */
    inline Uint16 ds3231_init_and_set_zero(void) {
        Uint16 status;

        // Give DS3231 time to power up
        DELAY_US(1000);

        // Clear OSF (Oscillator Stop Flag) in status register if needed
        Uint16 status_reg;
        status = ds3231_read_register(DS3231_REG_STATUS, &status_reg);
        if (status == I2C_SUCCESS && (status_reg & 0x80)) {
            // OSF is set, clear it
            status = ds3231_write_register(DS3231_REG_STATUS, status_reg & 0x7F);

            if (status != I2C_SUCCESS)
                return status;

            DELAY_US(100);
        }

        // Set time to 00:00:00
        status = ds3231_set_time_zero();

        return status;
    }

    /**
     * @brief Read current time from DS3231
     * @param hours Pointer to store hours
     * @param minutes Pointer to store minutes  
     * @param seconds Pointer to store seconds
     * @return I2C_SUCCESS if successful, error code otherwise
     */
    inline Uint16 ds3231_read_time(Uint16* hours, Uint16* minutes, Uint16* seconds) {
        Uint16 status;

        status = ds3231_read_register(DS3231_REG_SECONDS, seconds);
        if (status != I2C_SUCCESS)
            return status;

        status = ds3231_read_register(DS3231_REG_MINUTES, minutes);
        if (status != I2C_SUCCESS)
            return status;

        status = ds3231_read_register(DS3231_REG_HOURS, hours);
        if (status != I2C_SUCCESS)
            return status;

        return I2C_SUCCESS;
    }

#endif /* PERIPHERAL_SETUP_H */

#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include <stdio.h>
#include "driver/i2c.h"
#include "esp_log.h"

#define WRITE_BIT  I2C_MASTER_WRITE /*!< I2C master write */
#define READ_BIT   I2C_MASTER_READ  /*!< I2C master read */
#define ACK_CHECK_EN   0x1     /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS  0x0     /*!< I2C master will not check ack from slave */
#define ACK_VAL    0x0         /*!< I2C ack value */
#define NACK_VAL   0x1         /*!< I2C nack value */

#define SI7021_DEFAULT_ADDRESS         (0x40)

#define SI7021_MEASRH_HOLD_CMD           0xE5
#define SI7021_MEASRH_NOHOLD_CMD         0xF5
#define SI7021_MEASTEMP_HOLD_CMD         0xE3
#define SI7021_MEASTEMP_NOHOLD_CMD       0xF3
#define SI7021_READPREVTEMP_CMD          0xE0
#define SI7021_RESET_CMD                 0xFE
#define SI7021_WRITERHT_REG_CMD          0xE6
#define SI7021_READRHT_REG_CMD           0xE7
#define SI7021_WRITEHEATER_REG_CMD       0x51
#define SI7021_READHEATER_REG_CMD        0x11
#define SI7021_ID1_CMD                   0xFA0F
#define SI7021_ID2_CMD                   0xFCC9
#define SI7021_FIRMVERS_CMD              0x84B8

#define TSL2561_ADDR_LOW        (0x29)
#define TSL2561_ADDR_FLOAT      (0x39)    // Default address (pin left floating)
#define TSL2561_ADDR_HIGH       (0x49)

#define TSL2561_ADDR TSL2561_ADDR_FLOAT

#define TSL2561_COMMAND_BIT       (0x80)
#define TSL2561_CONTROL_POWERON   (0x03)

#define I2C_MASTER_SCL_IO    17    /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO    16    /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM I2C_NUM_1   /*!< I2C port number for master dev */
#define I2C_MASTER_TX_BUF_DISABLE   0   /*!< I2C master do not need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0   /*!< I2C master do not need buffer */
#define I2C_MASTER_FREQ_HZ    100000     /*!< I2C master clock frequency */

#define WRITE_BIT  I2C_MASTER_WRITE /*!< I2C master write */
#define READ_BIT   I2C_MASTER_READ  /*!< I2C master read */
#define ACK_CHECK_EN   0x1     /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS  0x0     /*!< I2C master will not check ack from slave */


#define TSL2561_LUX_LUXSCALE      (14)      // Scale by 2^14
#define TSL2561_LUX_RATIOSCALE    (9)       // Scale ratio by 2^9
#define TSL2561_LUX_CHSCALE       (10)      // Scale channel values by 2^10

#define TSL2561_LUX_CHSCALE_TINT0 (0x7517)  // 322/11 * 2^TSL2561_LUX_CHSCALE
#define TSL2561_LUX_CHSCALE_TINT1 (0x0FE7)  // 322/81 * 2^TSL2561_LUX_CHSCALE

#define TSL2561_LUX_K1T           (0x0040)  // 0.125 * 2^RATIO_SCALE
#define TSL2561_LUX_B1T           (0x01f2)  // 0.0304 * 2^LUX_SCALE
#define TSL2561_LUX_M1T           (0x01be)  // 0.0272 * 2^LUX_SCALE
#define TSL2561_LUX_K2T           (0x0080)  // 0.250 * 2^RATIO_SCALE
#define TSL2561_LUX_B2T           (0x0214)  // 0.0325 * 2^LUX_SCALE
#define TSL2561_LUX_M2T           (0x02d1)  // 0.0440 * 2^LUX_SCALE
#define TSL2561_LUX_K3T           (0x00c0)  // 0.375 * 2^RATIO_SCALE
#define TSL2561_LUX_B3T           (0x023f)  // 0.0351 * 2^LUX_SCALE
#define TSL2561_LUX_M3T           (0x037b)  // 0.0544 * 2^LUX_SCALE
#define TSL2561_LUX_K4T           (0x0100)  // 0.50 * 2^RATIO_SCALE
#define TSL2561_LUX_B4T           (0x0270)  // 0.0381 * 2^LUX_SCALE
#define TSL2561_LUX_M4T           (0x03fe)  // 0.0624 * 2^LUX_SCALE
#define TSL2561_LUX_K5T           (0x0138)  // 0.61 * 2^RATIO_SCALE
#define TSL2561_LUX_B5T           (0x016f)  // 0.0224 * 2^LUX_SCALE
#define TSL2561_LUX_M5T           (0x01fc)  // 0.0310 * 2^LUX_SCALE
#define TSL2561_LUX_K6T           (0x019a)  // 0.80 * 2^RATIO_SCALE
#define TSL2561_LUX_B6T           (0x00d2)  // 0.0128 * 2^LUX_SCALE
#define TSL2561_LUX_M6T           (0x00fb)  // 0.0153 * 2^LUX_SCALE
#define TSL2561_LUX_K7T           (0x029a)  // 1.3 * 2^RATIO_SCALE
#define TSL2561_LUX_B7T           (0x0018)  // 0.00146 * 2^LUX_SCALE
#define TSL2561_LUX_M7T           (0x0012)  // 0.00112 * 2^LUX_SCALE
#define TSL2561_LUX_K8T           (0x029a)  // 1.3 * 2^RATIO_SCALE
#define TSL2561_LUX_B8T           (0x0000)  // 0.000 * 2^LUX_SCALE
#define TSL2561_LUX_M8T           (0x0000)  // 0.000 * 2^LUX_SCALE

esp_err_t event_handler(void *ctx, system_event_t *event)
{
    return ESP_OK;
}

void init_2561() {
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA_IO;
    conf.scl_io_num = I2C_MASTER_SCL_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = 100000;
    ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &conf));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0));

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    ESP_ERROR_CHECK(i2c_master_start(cmd));
    i2c_master_write_byte(cmd, (TSL2561_ADDR << 1) | I2C_MASTER_WRITE, 1);
    i2c_master_write_byte(cmd, TSL2561_COMMAND_BIT, 1);
    i2c_master_write_byte(cmd, TSL2561_CONTROL_POWERON, 1);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
}

long calculateLux(uint16_t ch0, uint16_t ch1) {
    unsigned long chScale;
    unsigned long channel1;
    unsigned long channel0;

    chScale = (1 << (10));

    // Scale for gain (1x or 16x)
    //if (!_gain) chScale = chScale << 4;
    chScale = chScale << 4;

    // scale the channel values
    channel0 = (ch0 * chScale) >> TSL2561_LUX_CHSCALE;
    channel1 = (ch1 * chScale) >> TSL2561_LUX_CHSCALE;

    // find the ratio of the channel values (Channel1/Channel0)
    unsigned long ratio1 = 0;
    if (channel0 != 0)
        ratio1 = (channel1 << (TSL2561_LUX_RATIOSCALE + 1)) / channel0;

    // round the ratio value
    long ratio = (ratio1 + 1) >> 1;

    unsigned int b =0, m = 0;

    if ((ratio >= 0) && (ratio <= TSL2561_LUX_K1T)) {
        b = TSL2561_LUX_B1T;
        m = TSL2561_LUX_M1T;
    } else if (ratio <= TSL2561_LUX_K2T) {
        b = TSL2561_LUX_B2T;
        m = TSL2561_LUX_M2T;
    } else if (ratio <= TSL2561_LUX_K3T) {
        b = TSL2561_LUX_B3T;
        m = TSL2561_LUX_M3T;
    } else if (ratio <= TSL2561_LUX_K4T) {
        b = TSL2561_LUX_B4T;
        m = TSL2561_LUX_M4T;
    } else if (ratio <= TSL2561_LUX_K5T) {
        b = TSL2561_LUX_B5T;
        m = TSL2561_LUX_M5T;
    } else if (ratio <= TSL2561_LUX_K6T) {
        b = TSL2561_LUX_B6T;
        m = TSL2561_LUX_M6T;
    } else if (ratio <= TSL2561_LUX_K7T) {
        b = TSL2561_LUX_B7T;
        m = TSL2561_LUX_M7T;
    } else if (ratio > TSL2561_LUX_K8T) {
        b = TSL2561_LUX_B8T;
        m = TSL2561_LUX_M8T;
    }

    long temp;
    temp = ((channel0 * b) - (channel1 * m));

    // do not allow negative lux value
    if (temp < 0){
        temp = 0;
    }

    // round lsb (2^(LUX_SCALE-1))
    temp += (1 << (TSL2561_LUX_LUXSCALE - 1));

    // strip off fractional portion
    uint32_t lux = temp >> TSL2561_LUX_LUXSCALE;

    // Signal I2C had no errors
    return lux;
}

void readTSL2561(uint8_t addr, uint8_t *data){
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    ESP_ERROR_CHECK(i2c_master_start(cmd));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (TSL2561_ADDR << 1) | I2C_MASTER_WRITE, 1));
    i2c_master_write_byte(cmd, addr, 1);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_NUM_0, cmd, 500 / portTICK_RATE_MS);

    i2c_cmd_link_delete(cmd);

    cmd = i2c_cmd_link_create();
    ESP_ERROR_CHECK(i2c_master_start(cmd));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (TSL2561_ADDR << 1) | I2C_MASTER_READ, 1));
    i2c_master_read_byte(cmd, data, 1);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_NUM_0, cmd, 500 / portTICK_RATE_MS);

    i2c_cmd_link_delete(cmd);
}

void set_tsl2561_interrupt(){
    //TBD
}

esp_err_t read_current_lux() {
    uint8_t data1_h = 0;
    uint8_t data2_h = 0;
    uint8_t data1_l = 0;
    uint8_t data2_l = 0;

    readTSL2561(0x9C, &data1_l);
    readTSL2561(0x9D, &data1_h);
    readTSL2561(0x9E, &data2_l);
    readTSL2561(0x9F, &data2_h);

    printf("data_1l: %02x\n", data1_l);
    printf("data_1h: %02x\n", data1_h);
    printf("data_2l: %02x\n", data2_l);
    printf("data_2h: %02x\n", data2_h);
    long value = ((data1_l << 8) | data1_h) / 1.2;
    ESP_LOGI("mailbox", "1x value %ld", value);

    uint16_t ch0 = (data1_l << 8) | data1_h;
    uint16_t ch1 = (data2_l << 8) | data2_h;
    long lux = calculateLux(ch0, ch1);
    ESP_LOGI("mailbox", "calculated lux value %ld", lux);

    return ESP_OK;
}

void readSi7201(uint8_t addr, uint8_t *data){
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    ESP_ERROR_CHECK(i2c_master_start(cmd));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (SI7021_DEFAULT_ADDRESS  << 1) | I2C_MASTER_WRITE, 1));
    i2c_master_write_byte(cmd, addr, 1);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_NUM_0, cmd, 500 / portTICK_RATE_MS);

    i2c_cmd_link_delete(cmd);

    cmd = i2c_cmd_link_create();
    ESP_ERROR_CHECK(i2c_master_start(cmd));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (SI7021_DEFAULT_ADDRESS < 1) | I2C_MASTER_READ, 1));
    i2c_master_read_byte(cmd, data, 1);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_NUM_0, cmd, 500 / portTICK_RATE_MS);

    i2c_cmd_link_delete(cmd);
}

void app_main(void) {
    init_2561();
    while (1) {
        read_current_lux( );
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

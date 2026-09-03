#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_gpio.h"
#include "core/include/spi_display.h"
#include "core/include/defs.h"

extern SPI_HandleTypeDef hspi2;

// All on the same port
static GPIO_TypeDef *PIN_PORT = GPIOA;
static GPIO_TypeDef *PIN_RST_PORT = GPIOB;


static uint16_t CS_PIN = GPIO_PIN_9;
static uint16_t DC_PIN = GPIO_PIN_10;
static uint16_t RESET_PIN = GPIO_PIN_5;

static platform_err_t spi_init(void) {
  HAL_GPIO_WritePin(PIN_RST_PORT, RESET_PIN, GPIO_PIN_SET);
  HAL_Delay(10);
  HAL_GPIO_WritePin(PIN_RST_PORT, RESET_PIN, GPIO_PIN_RESET);
  HAL_Delay(10);
  HAL_GPIO_WritePin(PIN_RST_PORT, RESET_PIN, GPIO_PIN_SET);
  HAL_Delay(120);

  return PLATFORM_OK;
}

static void spi_send_data_byte(uint8_t data) {
  HAL_GPIO_WritePin(PIN_PORT, DC_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(PIN_PORT, CS_PIN, GPIO_PIN_SET);

  HAL_SPI_Transmit(&hspi2, &data, 1, HAL_MAX_DELAY);

  HAL_GPIO_WritePin(PIN_PORT, DC_PIN, GPIO_PIN_SET);
}

static void spi_send_command(uint8_t cmd) {
  HAL_GPIO_WritePin(PIN_PORT, DC_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(PIN_PORT, CS_PIN, GPIO_PIN_RESET);

  HAL_SPI_Transmit(&hspi2, &cmd, 1, HAL_MAX_DELAY);

  HAL_GPIO_WritePin(PIN_PORT, DC_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(PIN_PORT, CS_PIN, GPIO_PIN_SET);
}

static void spi_send_data(uint8_t *data, uint32_t len) {
  if (len == 0) {
    return;
  }

  HAL_GPIO_WritePin(PIN_PORT, DC_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(PIN_PORT, CS_PIN, GPIO_PIN_SET);

  HAL_SPI_Transmit(&hspi2, data, len, HAL_MAX_DELAY);

  HAL_GPIO_WritePin(PIN_PORT, DC_PIN, GPIO_PIN_SET);
}

spi_display_t stm32_spi = {
    .spi_init = spi_init,
    .spi_send_command = spi_send_command,
    .spi_send_data = spi_send_data,
    .spi_send_data_byte = spi_send_data_byte,
};

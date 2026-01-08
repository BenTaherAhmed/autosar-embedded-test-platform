#ifndef MCAL_MOCK_SPI_H
#define MCAL_MOCK_SPI_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    SPI_OK = 0,
    SPI_E_NOT_INIT = 1,
    SPI_E_PARAM = 2
} Spi_ReturnType;

typedef enum
{
    SPI_MODE_0 = 0,
    SPI_MODE_1,
    SPI_MODE_2,
    SPI_MODE_3
} Spi_ModeType;

typedef struct
{
    uint32_t baudrate_hz;
    Spi_ModeType mode;
    uint8_t bits_per_word; /* typically 8 */
} Spi_ConfigType;

/* AUTOSAR-like API */
void Spi_Init(const Spi_ConfigType* ConfigPtr);
Spi_ReturnType Spi_DeInit(void);

Spi_ReturnType Spi_Transmit(const uint8_t* tx, size_t tx_len);
Spi_ReturnType Spi_Receive(uint8_t* rx, size_t rx_len);
Spi_ReturnType Spi_TransmitReceive(const uint8_t* tx, uint8_t* rx, size_t len);

/* For testing/inspection */
uint8_t Spi_IsInitialized(void);

#ifdef __cplusplus
}
#endif

#endif /* MCAL_MOCK_SPI_H */

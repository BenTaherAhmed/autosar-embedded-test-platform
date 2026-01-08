#include "spi.h"

static uint8_t g_spi_initialized = 0U;
static Spi_ConfigType g_cfg;

void Spi_Init(const Spi_ConfigType* ConfigPtr)
{
    if (ConfigPtr == NULL)
    {
        g_spi_initialized = 0U;
        return;
    }

    g_cfg = *ConfigPtr;
    g_spi_initialized = 1U;
}

Spi_ReturnType Spi_DeInit(void)
{
    g_spi_initialized = 0U;
    return SPI_OK;
}

uint8_t Spi_IsInitialized(void)
{
    return g_spi_initialized;
}

static Spi_ReturnType spi_check_init_and_params(const void* p, size_t len)
{
    if (g_spi_initialized == 0U)
    {
        return SPI_E_NOT_INIT;
    }
    if ((p == NULL) && (len > 0U))
    {
        return SPI_E_PARAM;
    }
    return SPI_OK;
}

Spi_ReturnType Spi_Transmit(const uint8_t* tx, size_t tx_len)
{
    (void)g_cfg; /* not used yet in mock */

    Spi_ReturnType rc = spi_check_init_and_params(tx, tx_len);
    if (rc != SPI_OK)
    {
        return rc;
    }

    /* Mock: do nothing */
    return SPI_OK;
}

Spi_ReturnType Spi_Receive(uint8_t* rx, size_t rx_len)
{
    (void)g_cfg;

    Spi_ReturnType rc = spi_check_init_and_params(rx, rx_len);
    if (rc != SPI_OK)
    {
        return rc;
    }

    /* Mock: fill with deterministic pattern */
    for (size_t i = 0; i < rx_len; i++)
    {
        rx[i] = (uint8_t)(0xA5U ^ (uint8_t)i);
    }

    return SPI_OK;
}

Spi_ReturnType Spi_TransmitReceive(const uint8_t* tx, uint8_t* rx, size_t len)
{
    (void)g_cfg;

    Spi_ReturnType rc_tx = spi_check_init_and_params(tx, len);
    Spi_ReturnType rc_rx = spi_check_init_and_params(rx, len);

    if (rc_tx != SPI_OK) return rc_tx;
    if (rc_rx != SPI_OK) return rc_rx;

    /* Mock: rx depends on tx for traceability */
    for (size_t i = 0; i < len; i++)
    {
        rx[i] = (uint8_t)(tx[i] ^ 0x5AU);
    }

    return SPI_OK;
}

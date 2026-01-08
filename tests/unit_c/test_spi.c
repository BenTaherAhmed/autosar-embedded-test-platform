// REQ: REQ_SPI_001
#include <stdio.h>
#include <string.h>
#include "../../src/mcal_mock/spi.h"

/* Minimal test macros (no dependency) */
#define ASSERT_TRUE(cond) do { if (!(cond)) { \
    printf("ASSERT_TRUE failed at %s:%d\n", __FILE__, __LINE__); return 1; } } while(0)

#define ASSERT_EQ(a,b) do { if ((a) != (b)) { \
    printf("ASSERT_EQ failed at %s:%d (got %d expected %d)\n", __FILE__, __LINE__, (int)(a), (int)(b)); return 1; } } while(0)

static int test_not_initialized_returns_error(void)
{
    uint8_t rx[4];
    ASSERT_EQ(Spi_IsInitialized(), 0);

    ASSERT_EQ(Spi_Transmit((const uint8_t*)"AB", 2), SPI_E_NOT_INIT);
    ASSERT_EQ(Spi_Receive(rx, sizeof(rx)), SPI_E_NOT_INIT);
    ASSERT_EQ(Spi_TransmitReceive((const uint8_t*)"ABCD", rx, sizeof(rx)), SPI_E_NOT_INIT);
    return 0;
}

static int test_init_and_transmit_receive(void)
{
    Spi_ConfigType cfg = {
        .baudrate_hz = 1000000U,
        .mode = SPI_MODE_0,
        .bits_per_word = 8U
    };

    Spi_Init(&cfg);
    ASSERT_EQ(Spi_IsInitialized(), 1);

    uint8_t tx[4] = {1,2,3,4};
    uint8_t rx[4] = {0};

    ASSERT_EQ(Spi_Transmit(tx, sizeof(tx)), SPI_OK);
    ASSERT_EQ(Spi_TransmitReceive(tx, rx, sizeof(tx)), SPI_OK);

    for (size_t i = 0; i < sizeof(tx); i++)
    {
        ASSERT_EQ(rx[i], (uint8_t)(tx[i] ^ 0x5A));
    }

    ASSERT_EQ(Spi_DeInit(), SPI_OK);
    ASSERT_EQ(Spi_IsInitialized(), 0);
    return 0;
}

int main(void)
{
    int rc = 0;
    rc |= test_not_initialized_returns_error();
    rc |= test_init_and_transmit_receive();

    if (rc == 0)
        printf("All SPI unit tests PASSED.\n");
    else
        printf("Some SPI unit tests FAILED.\n");

    return rc;
}

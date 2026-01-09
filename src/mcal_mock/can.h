#ifndef MCAL_MOCK_CAN_H
#define MCAL_MOCK_CAN_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CAN_MAX_DLC 8u

typedef enum {
    CAN_OK = 0,
    CAN_NOT_INITIALIZED,
    CAN_INVALID_PARAM,
    CAN_TX_OVERFLOW,
    CAN_RX_EMPTY
} Can_ReturnType;

typedef struct {
    uint32_t id;              /* 11-bit or 29-bit ID stored in 32-bit */
    bool     extended;        /* false = standard, true = extended */
    uint8_t  dlc;             /* 0..8 */
    uint8_t  data[CAN_MAX_DLC];
} Can_FrameType;

/* REQ: REQ_CAN_001 */
void Can_Init(void);

/* REQ: REQ_CAN_001 */
void Can_Deinit(void);

/* REQ: REQ_CAN_002
   REQ: REQ_CAN_005 */
Can_ReturnType Can_Send(const Can_FrameType* frame);

/* REQ: REQ_CAN_003
   REQ: REQ_CAN_005 */
Can_ReturnType Can_Receive(Can_FrameType* out_frame);

/* Test helper: inject an RX frame (simulates reception from bus)
   REQ: REQ_CAN_004
   REQ: REQ_CAN_005 */
Can_ReturnType Can_InjectRx(const Can_FrameType* frame);

/* Optional helper for tests */
bool Can_IsInitialized(void);

#ifdef __cplusplus
}
#endif

#endif /* MCAL_MOCK_CAN_H */

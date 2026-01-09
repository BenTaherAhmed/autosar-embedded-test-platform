#include "can.h"
#include <string.h>

/* Simple deterministic ring buffers for TX and RX */
#ifndef CAN_TX_QUEUE_SIZE
#define CAN_TX_QUEUE_SIZE 8u
#endif

#ifndef CAN_RX_QUEUE_SIZE
#define CAN_RX_QUEUE_SIZE 8u
#endif

typedef struct {
    Can_FrameType buf[CAN_TX_QUEUE_SIZE];
    uint8_t head;
    uint8_t tail;
    uint8_t count;
} Can_TxQueue;

typedef struct {
    Can_FrameType buf[CAN_RX_QUEUE_SIZE];
    uint8_t head;
    uint8_t tail;
    uint8_t count;
} Can_RxQueue;

static bool g_initialized = false;
static Can_TxQueue g_txq;
static Can_RxQueue g_rxq;

static void txq_reset(void) {
    memset(&g_txq, 0, sizeof(g_txq));
}
static void rxq_reset(void) {
    memset(&g_rxq, 0, sizeof(g_rxq));
}

static bool frame_is_valid(const Can_FrameType* f) {
    if (f == NULL) return false;
    if (f->dlc > CAN_MAX_DLC) return false;
    /* ID validation can be extended later */
    return true;
}

/* REQ: REQ_CAN_001 */
void Can_Init(void) {
    g_initialized = true;
    txq_reset();
    rxq_reset();
}

/* REQ: REQ_CAN_001 */
void Can_Deinit(void) {
    g_initialized = false;
    txq_reset();
    rxq_reset();
}

bool Can_IsInitialized(void) {
    return g_initialized;
}

/* REQ: REQ_CAN_002
   REQ: REQ_CAN_005
   REQ: REQ_CAN_004 */
Can_ReturnType Can_Send(const Can_FrameType* frame) {
    if (!g_initialized) {
        return CAN_NOT_INITIALIZED;
    }
    if (!frame_is_valid(frame)) {
        return CAN_INVALID_PARAM;
    }
    if (g_txq.count >= CAN_TX_QUEUE_SIZE) {
        return CAN_TX_OVERFLOW;
    }

    g_txq.buf[g_txq.head] = *frame;
    g_txq.head = (uint8_t)((g_txq.head + 1u) % CAN_TX_QUEUE_SIZE);
    g_txq.count++;
    return CAN_OK;
}

/* REQ: REQ_CAN_003
   REQ: REQ_CAN_005 */
Can_ReturnType Can_Receive(Can_FrameType* out_frame) {
    if (!g_initialized) {
        return CAN_NOT_INITIALIZED;
    }
    if (out_frame == NULL) {
        return CAN_INVALID_PARAM;
    }
    if (g_rxq.count == 0u) {
        return CAN_RX_EMPTY;
    }

    *out_frame = g_rxq.buf[g_rxq.tail];
    g_rxq.tail = (uint8_t)((g_rxq.tail + 1u) % CAN_RX_QUEUE_SIZE);
    g_rxq.count--;
    return CAN_OK;
}

/* Test helper: simulate bus RX push
   REQ: REQ_CAN_004
   REQ: REQ_CAN_005 */
Can_ReturnType Can_InjectRx(const Can_FrameType* frame) {
    if (!g_initialized) {
        return CAN_NOT_INITIALIZED;
    }
    if (!frame_is_valid(frame)) {
        return CAN_INVALID_PARAM;
    }
    if (g_rxq.count >= CAN_RX_QUEUE_SIZE) {
        return CAN_TX_OVERFLOW; /* reuse overflow code; could introduce CAN_RX_OVERFLOW later */
    }

    g_rxq.buf[g_rxq.head] = *frame;
    g_rxq.head = (uint8_t)((g_rxq.head + 1u) % CAN_RX_QUEUE_SIZE);
    g_rxq.count++;
    return CAN_OK;
}

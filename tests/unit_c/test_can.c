/* REQ: REQ_CAN_001 */
/* REQ: REQ_CAN_002 */
/* REQ: REQ_CAN_003 */
/* REQ: REQ_CAN_004 */
/* REQ: REQ_CAN_005 */


#include <stdio.h>
#include <string.h>
#include "../../src/mcal_mock/can.h"

#define ASSERT_TRUE(cond) do { \
    if (!(cond)) { \
        printf("ASSERT_TRUE failed at %s:%d\n", __FILE__, __LINE__); \
        return 1; \
    } \
} while (0)

#define ASSERT_EQ(a,b) do { \
    if ((a) != (b)) { \
        printf("ASSERT_EQ failed at %s:%d (got=%d expected=%d)\n", __FILE__, __LINE__, (int)(a), (int)(b)); \
        return 1; \
    } \
} while (0)

static Can_FrameType make_frame(uint32_t id, bool ext, uint8_t dlc, uint8_t fill) {
    Can_FrameType f;
    f.id = id;
    f.extended = ext;
    f.dlc = dlc;
    for (uint8_t i = 0; i < CAN_MAX_DLC; i++) f.data[i] = fill;
    return f;
}

int main(void) {
    /* REQ: REQ_CAN_001 */
    Can_Deinit();
    ASSERT_TRUE(!Can_IsInitialized());

    Can_FrameType out;

    /* REQ: REQ_CAN_001 */
    ASSERT_EQ(Can_Send(NULL), CAN_NOT_INITIALIZED);
    ASSERT_EQ(Can_Receive(&out), CAN_NOT_INITIALIZED);

    Can_Init();
    ASSERT_TRUE(Can_IsInitialized());

    /* REQ: REQ_CAN_005 */
    Can_FrameType bad = make_frame(0x123, false, 9, 0xAA);
    ASSERT_EQ(Can_Send(&bad), CAN_INVALID_PARAM);
    ASSERT_EQ(Can_Receive(NULL), CAN_INVALID_PARAM);

    /* REQ: REQ_CAN_003 */
    ASSERT_EQ(Can_Receive(&out), CAN_RX_EMPTY);

    /* REQ: REQ_CAN_002 */
    Can_FrameType f1 = make_frame(0x123, false, 8, 0x11);
    ASSERT_EQ(Can_Send(&f1), CAN_OK);

    /* REQ: REQ_CAN_003 */
    Can_FrameType rx = make_frame(0x456, true, 4, 0x22);
    ASSERT_EQ(Can_InjectRx(&rx), CAN_OK);
    memset(&out, 0, sizeof(out));
    ASSERT_EQ(Can_Receive(&out), CAN_OK);
    ASSERT_TRUE(out.id == rx.id);
    ASSERT_TRUE(out.extended == rx.extended);
    ASSERT_TRUE(out.dlc == rx.dlc);
    ASSERT_TRUE(out.data[0] == 0x22);

    /* REQ: REQ_CAN_004 (overflow) */
    /* Fill RX queue to overflow (CAN_RX_QUEUE_SIZE is 8 by default). */
    for (int i = 0; i < 8; i++) {
        Can_FrameType tmp = make_frame((uint32_t)(0x500 + i), false, 1, (uint8_t)i);
        ASSERT_EQ(Can_InjectRx(&tmp), CAN_OK);
    }
    Can_FrameType extra = make_frame(0x999, false, 1, 0xFF);
    ASSERT_EQ(Can_InjectRx(&extra), CAN_TX_OVERFLOW);

    printf("All CAN unit tests PASSED.\n");
    return 0;
}

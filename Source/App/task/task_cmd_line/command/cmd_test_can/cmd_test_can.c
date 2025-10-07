/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* Task common includes. */
#include "task_common.h"

/* Component includes. */
#include "cmd_line.h"

/* USER include. */
#include "cmd_test_can.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Private Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* :::::::::: Test CAN Command :::::::: */
int CMD_CAN_SEND_DEMO(int argc, char *argv[])
{
    /* CMD Input Guard */
    if (argc < 1)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 1)
		return CMDLINE_TOO_MANY_ARGS;

    flexcan_frame_t demo_frame =
    {
        .dataByte0  = 3,
        .dataByte1  = 7,
        .dataByte2  = 7,
        .dataByte3  = 0xc,
        .dataByte4  = 2,

        .id         = FLEXCAN_ID_STD(CAN_TX_IDENTIFIER),
        .format     = (uint8_t)kFLEXCAN_FrameFormatStandard,
        .type       = (uint8_t)kFLEXCAN_FrameTypeData,
        .length     = 8,
    };

	bsp_libcsp_can_send_frame(&demo_frame);

	// Return success.
	return CMDLINE_OK;
}

int CMD_CAN_SEND_2BYTE(int argc, char *argv[])
{
    /* CMD Input Guard */
    if (argc < 3)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 3)
		return CMDLINE_TOO_MANY_ARGS;

	int receive_argm[2];

	receive_argm[0] = atoi(argv[1]);
	receive_argm[1] = atoi(argv[2]);

	if ((receive_argm[0] > 255) || (receive_argm[0] < 0))
		return CMDLINE_INVALID_ARG;
	else if ((receive_argm[1] > 255) || (receive_argm[1] < 0))
		return CMDLINE_INVALID_ARG;
    
    flexcan_frame_t _2byte_frame_ =
    {
        .dataByte0  = (uint8_t)receive_argm[0],
        .dataByte1  = (uint8_t)receive_argm[1],

        .id         = FLEXCAN_ID_STD(CAN_TX_IDENTIFIER),
        .format     = (uint8_t)kFLEXCAN_FrameFormatStandard,
        .type       = (uint8_t)kFLEXCAN_FrameTypeData,
        .length     = 8,
    };

	bsp_libcsp_can_send_frame(&_2byte_frame_);

	// Return success.
	return CMDLINE_OK;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

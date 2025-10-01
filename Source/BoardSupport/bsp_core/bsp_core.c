/* Freescale includes. */
#include "fsl_common.h"
#include "fsl_clock.h"
#include "fsl_lpuart.h"
#include "fsl_flexcan.h"

/* USER includes. */
#include "bsp_core.h"
#include "bsp_board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void bsp_core_init_uart(void);

static void bsp_core_init_can(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief bsp_core_init
 */
void bsp_core_init(void)
{
    bsp_core_init_uart();
    bsp_core_init_can();
}

/*!
 * @brief bsp_core_init_uart
 */
static void bsp_core_init_uart(void)
{
    lpuart_config_t config;
    
    /* clang-format off */

    const clock_root_config_t lpuartClkCfg =
    {
        .clockOff = false,
	    .mux = 0,
	    .div = 1
    };
    /* clang-format on */

    CLOCK_SetRootClock(DEBUG_DB9_LPUART_CLOCK_ROOT, &lpuartClkCfg);
    CLOCK_EnableClock(DEBUG_DB9_LPUART_CLOCK_GATE);

    /*
     * config.baudRate_Bps = 115200U;
     * config.parityMode = kLPUART_ParityDisabled;
     * config.stopBitCount = kLPUART_OneStopBit;
     * config.txFifoWatermark = 0;
     * config.rxFifoWatermark = 0;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    LPUART_GetDefaultConfig(&config);
    config.baudRate_Bps = DEBUG_DB9_LPUART_BAUDRATE;
    config.enableTx     = true;
    config.enableRx     = true;

    LPUART_Init(DEBUG_DB9_LPUART_BASE, &config, DEBUG_DB9_LPUART_CLK_FREQ);
}

static void bsp_core_init_can(void)
{
    flexcan_config_t flexcanConfig;
    flexcan_timing_config_t flexcan_timing_config;

    /* clang-format off */

    const clock_root_config_t flexcanClkCfg =
    {
        .clockOff = false,
	    .mux = 2,
	    .div = 10
    };
    /* clang-format on */

    CLOCK_SetRootClock(LIBCSP_CAN_CLOCK_ROOT, &flexcanClkCfg);
    CLOCK_EnableClock(LIBCSP_CAN_CLOCK_GATE);

    /* Get FlexCAN module default Configuration. */
    /*
     * flexcanConfig.clkSrc                 = kFLEXCAN_ClkSrc0;
     * flexcanConfig.bitRate               = 1000000U;
     * flexcanConfig.bitRateFD             = 2000000U;
     * flexcanConfig.maxMbNum               = 16;
     * flexcanConfig.enableLoopBack         = false;
     * flexcanConfig.enableSelfWakeup       = false;
     * flexcanConfig.enableIndividMask      = false;
     * flexcanConfig.disableSelfReception   = false;
     * flexcanConfig.enableListenOnlyMode   = false;
     * flexcanConfig.enableDoze             = false;
     */
    FLEXCAN_GetDefaultConfig(&flexcanConfig);

    // Config Improved Timing Values
    memset(&flexcan_timing_config, 0, sizeof(flexcan_timing_config_t));

    if (FLEXCAN_CalculateImprovedTimingValues(LIBCSP_CAN_BASE, flexcanConfig.bitRate, LIBCSP_CAN_CLK_FREQ, &flexcan_timing_config))
    {
        /* Update the improved timing configuration*/
        memcpy(&(flexcanConfig.timingConfig), &flexcan_timing_config, sizeof(flexcan_timing_config_t));
    }

    // Load the default flexcan config and improved timing configuration to init flex can
    FLEXCAN_Init(LIBCSP_CAN_BASE, &flexcanConfig, LIBCSP_CAN_CLK_FREQ);
}
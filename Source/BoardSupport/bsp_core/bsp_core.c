/* Freescale includes. */
#include "fsl_common.h"
#include "fsl_clock.h"
#include "fsl_lpuart.h"
#include "fsl_flexcan.h"

#include "fsl_lpspi.h"
#include "fsl_rgpio.h"

#include "fsl_lpi2c.h"

#include "fsl_tpm.h"

/* USER includes. */
#include "bsp_core.h"
#include "bsp_board.h"

#include "i2c_io.h"
#include "spi_io.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void bsp_core_init_uart(void);
static void bsp_core_init_can(void);
static void bsp_core_init_onboard_adc_spi(void);
static void bsp_core_init_onboard_adc_cs_gpio(void);
static void bsp_core_init_tec_cs_gpio(void);
// static void bsp_core_init_gpio(void);
static void bsp_core_init_io_expander_i2c(void);
// static void bsp_core_init_tim(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
// const volatile uint32_t temp[1*1024] = {0xAA};
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
    bsp_core_init_io_expander_i2c();
    bsp_core_init_onboard_adc_spi();
    bsp_core_init_onboard_adc_cs_gpio();
    bsp_core_init_tec_cs_gpio();
    // bsp_core_init_spi();
    // bsp_core_init_gpio();
    // bsp_core_init_tim();
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

SPI_Io_t onboard_adc_spi =
{
    .ui32SpiPort = 1,
};
static void bsp_core_init_onboard_adc_spi(void)
{
    uint32_t srcClock_Hz;
    lpspi_master_config_t masterConfig;

    const clock_root_config_t lpspiClkCfg =
    {
        .clockOff = false,
	    .mux = 0,
	    .div = 1
    };

    CLOCK_SetRootClock(ONBOARD_ADC_SPI_CLOCK_ROOT, &lpspiClkCfg);
    CLOCK_EnableClock(ONBOARD_ADC_SPI_CLOCK_GATE);

    /* Get LPSPI module default Configuration. */
    /*
     * 
     * masterConfig->baudRate                       = 500000;
     * masterConfig->bitsPerFrame                   = 8;
     * masterConfig->cpol                           = kLPSPI_ClockPolarityActiveHigh;
     * masterConfig->cpha                           = kLPSPI_ClockPhaseFirstEdge;
     * masterConfig->direction                      = kLPSPI_MsbFirst;

     * masterConfig->pcsToSckDelayInNanoSec         = (1000000000U / masterConfig->baudRate) / 2U;
     * masterConfig->lastSckToPcsDelayInNanoSec     = (1000000000U / masterConfig->baudRate) / 2U;
     * masterConfig->betweenTransferDelayInNanoSec  = (1000000000U / masterConfig->baudRate) / 2U;

     * masterConfig->whichPcs                       = kLPSPI_Pcs0;
     * masterConfig->pcsActiveHighOrLow             = kLPSPI_PcsActiveLow;

     * masterConfig->pinCfg                         = kLPSPI_SdiInSdoOut;
     * masterConfig->dataOutConfig                  = kLpspiDataOutRetained;

     * masterConfig->enableInputDelay               = false;
     */
    LPSPI_MasterGetDefaultConfig(&masterConfig);
    masterConfig.pcsToSckDelayInNanoSec         = 0;
    masterConfig.lastSckToPcsDelayInNanoSec     = 0;
    masterConfig.betweenTransferDelayInNanoSec  = 0;
    masterConfig.direction                      = kLPSPI_MsbFirst;
    masterConfig.cpol                           = kLPSPI_ClockPolarityActiveLow;
    masterConfig.cpha                           = kLPSPI_ClockPhaseSecondEdge;
    masterConfig.pinCfg                         = kLPSPI_SdoInSdiOut;
    masterConfig.baudRate = ONBOARD_ADC_SPI_BAUDRATE;
    masterConfig.whichPcs = kLPSPI_Pcs1;
    
    srcClock_Hz = ONBOARD_ADC_SPI_CLK_FREQ;
    LPSPI_MasterInit(ONBOARD_ADC_SPI_BASE, &masterConfig, srcClock_Hz);

    // Disable before config
    ONBOARD_ADC_SPI_BASE->CR &= ~LPSPI_CR_MEN_MASK;

    // Allow stalls (safe)
    ONBOARD_ADC_SPI_BASE->CFGR1 &= ~LPSPI_CFGR1_NOSTALL_MASK;

    // Force 8-bit frames; choose PCS1; hold PCS internally (no real pin toggling)
    ONBOARD_ADC_SPI_BASE->TCR = (ONBOARD_ADC_SPI_BASE->TCR & ~(LPSPI_TCR_FRAMESZ_MASK |
                            LPSPI_TCR_RXMSK_MASK   |
                            LPSPI_TCR_TXMSK_MASK   |
                            LPSPI_TCR_PCS_MASK     |
                            LPSPI_TCR_CONT_MASK    |
                            LPSPI_TCR_CONTC_MASK))
            |  LPSPI_TCR_FRAMESZ(7)         // 8-bit
            |  LPSPI_TCR_PCS(1)             // "PCS1" (not pin-muxed)
            |  LPSPI_TCR_RXMSK(0)
            |  LPSPI_TCR_TXMSK(0)
            |  LPSPI_TCR_CONT(1)            // keep PCS asserted internally
            |  LPSPI_TCR_CONTC(1);

    // Clean state
    ONBOARD_ADC_SPI_BASE->CR |=  (LPSPI_CR_RTF_MASK | LPSPI_CR_RRF_MASK);       // flush TX/RX FIFOs
    ONBOARD_ADC_SPI_BASE->SR  =   LPSPI_SR_WCF_MASK | LPSPI_SR_FCF_MASK | LPSPI_SR_TCF_MASK; // clear sticky

    // Enable
    ONBOARD_ADC_SPI_BASE->CR |= LPSPI_CR_MEN_MASK;
}

do_t onboard_adc0_cs =
{
    .port = 4,
    .pin  = 16,
};

do_t onboard_adc1_cs =
{
    .port = 4,
    .pin  = 20,
};
static void bsp_core_init_onboard_adc_cs_gpio(void)
{
    /* Define the init structure for the output LED pin*/
    rgpio_pin_config_t onboard_ADC_CS_config =
    {
        kRGPIO_DigitalOutput,
        1,
    };

    /* Board pin, clock, debug console init */
    /* clang-format off */

    const clock_root_config_t rgpioClkCfg =
    {
        .clockOff = false,
        .mux = 0, // 24Mhz Mcore root buswake clock
        .div = 1
    };

    CLOCK_SetRootClock(ONBOARD_ADC_GPIO_CS_CLOCK_ROOT, &rgpioClkCfg);
    CLOCK_EnableClock(ONBOARD_ADC_GPIO_CS_CLOCK_GATE);

    /* Set PCNS register value to 0x0 to prepare the RGPIO initialization */
    ONBOARD_ADC_GPIO_CS_PORT->PCNS = 0x0;

    /* Init output LED GPIO. */
    RGPIO_PinInit(ONBOARD_ADC_GPIO_CS_PORT, ONBOARD_ADC_GPIO_CS0_PIN, &onboard_ADC_CS_config);
    RGPIO_PinInit(ONBOARD_ADC_GPIO_CS_PORT, ONBOARD_ADC_GPIO_CS1_PIN, &onboard_ADC_CS_config);
    RGPIO_PinInit(ONBOARD_ADC_GPIO_CS_PORT, 17, &onboard_ADC_CS_config);
    RGPIO_PinInit(ONBOARD_ADC_GPIO_CS_PORT, 21, &onboard_ADC_CS_config);
}

static void bsp_core_init_tec_cs_gpio(void)
{
    /* Define the init structure for the output LED pin*/
    rgpio_pin_config_t TEC_CS_config =
    {
        kRGPIO_DigitalOutput,
        1,
    };

    /* Board pin, clock, debug console init */
    /* clang-format off */

    const clock_root_config_t rgpioClkCfg =
    {
        .clockOff = false,
        .mux = 0, // 24Mhz Mcore root buswake clock
        .div = 1
    };

    CLOCK_SetRootClock(kCLOCK_Root_BusWakeup, &rgpioClkCfg);
    CLOCK_EnableClock(kCLOCK_Gpio3);

    /* Set PCNS register value to 0x0 to prepare the RGPIO initialization */
    GPIO3->PCNS = 0x0;

    /* Init output LED GPIO. */
    RGPIO_PinInit(GPIO3, 29, &TEC_CS_config);
    RGPIO_PinInit(GPIO3, 31, &TEC_CS_config);
    RGPIO_PinInit(GPIO3, 28, &TEC_CS_config);
    RGPIO_PinInit(GPIO3, 30, &TEC_CS_config);
}

// static void bsp_core_init_gpio(void)
// {
//     /* Define the init structure for the output LED pin*/
//     rgpio_pin_config_t photo_ADC_CS_config =
//     {
//         kRGPIO_DigitalOutput,
//         1,
//     };

//     rgpio_pin_config_t photo_ADC_CV_config =
//     {
//         kRGPIO_DigitalOutput,
//         0,
//     };

//     /* Board pin, clock, debug console init */
//     /* clang-format off */

//     const clock_root_config_t rgpioClkCfg =
//     {
//         .clockOff = false,
//         .mux = 1, // 24Mhz Mcore root buswake clock
//         .div = 2
//     };

//     CLOCK_SetRootClock(TEC_SPI_GPIO_CS_CLOCK_ROOT, &rgpioClkCfg);
//     CLOCK_EnableClock(TEC_SPI_GPIO_CS_CLOCK_GATE);

//     /* Set PCNS register value to 0x0 to prepare the RGPIO initialization */
//     PHOTO_ADC_GPIO_PORT->PCNS = 0x0;

//     /* Init output LED GPIO. */
//     RGPIO_PinInit(PHOTO_ADC_GPIO_PORT, PHOTO_ADC_GPIO_SPI_CS_PIN, &photo_ADC_CS_config);
//     RGPIO_PinInit(PHOTO_ADC_GPIO_PORT, PHOTO_ADC_GPIO_SPI_CV_PIN, &photo_ADC_CV_config);
// }

i2c_io_t io_expander_i2c =
{
		.ui32I2cPort = 7
};
i2c_io_t heater_i2c =
{
		.ui32I2cPort = 7
};
static void bsp_core_init_io_expander_i2c(void)
{
    lpi2c_master_config_t i2c_masterConfig;

    /* clang-format off */
    const clock_root_config_t lpi2cClkCfg =
    {
        .clockOff = false,
	    .mux = 0, // 24MHz oscillator source
	    .div = 1
    };
    /* clang-format on */

    CLOCK_SetRootClock(IO_EXPAN_CLOCK_ROOT, &lpi2cClkCfg);
    CLOCK_EnableClock(IO_EXPAN_CLOCK_GATE);

    /*
     * i2c_masterConfig.debugEnable = false;
     * i2c_masterConfig.ignoreAck = false;
     * i2c_masterConfig.pinConfig = kLPI2C_2PinOpenDrain;
     * i2c_masterConfig.baudRate_Hz = 100000U;
     * i2c_masterConfig.busIdleTimeout_ns = 0;
     * i2c_masterConfig.pinLowTimeout_ns = 0;
     * i2c_masterConfig.sdaGlitchFilterWidth_ns = 0;
     * i2c_masterConfig.sclGlitchFilterWidth_ns = 0;
     */
    LPI2C_MasterGetDefaultConfig(&i2c_masterConfig);

    /* Change the default baudrate configuration */
    i2c_masterConfig.baudRate_Hz = IO_EXPAN_BAUDRATE_HZ;

    /* Initialize the LPI2C master peripheral */
    LPI2C_MasterInit(IO_EXPAN_BASE, &i2c_masterConfig, IO_EXPAN_CLK_FREQ);

    // i2c_io_init(&io_expander_i2c);
    // i2c_io_init(&heater_i2c);
}

// static void bsp_core_init_tim(void)
// {
//     tpm_config_t tpmInfo;

//     const clock_root_config_t lptpmClkCfg =
//     {
//         .clockOff = false,
// 	    .mux = 0,
// 	    .div = 1
//     };

//     CLOCK_SetRootClock(PHOTO_ADC_TIM_CLOCK_ROOT, &lptpmClkCfg);
//     CLOCK_EnableClock(PHOTO_ADC_TIM_CLOCK_GATE);

//     TPM_GetDefaultConfig(&tpmInfo);

//     /* TPM clock divide by TPM_PRESCALER */
//     tpmInfo.prescale = PHOTO_ADC_TIM_PRESCALER;

//     /* Initialize TPM module */
//     TPM_Init(PHOTO_ADC_TIM_BASE, &tpmInfo);

//     /* Set timer period */
//     TPM_SetTimerPeriod(PHOTO_ADC_TIM_BASE, USEC_TO_COUNT(PHOTO_ADC_TIM_PERIOD_US, PHOTO_ADC_TIM_CLK_FREQ / (1U << tpmInfo.prescale)));
// }
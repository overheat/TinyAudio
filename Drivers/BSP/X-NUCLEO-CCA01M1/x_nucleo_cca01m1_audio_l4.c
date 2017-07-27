/**
******************************************************************************
* @file    x_nucleo_cca01m1_audio_L4.c
* @author  Central Labs
* @version V2.0.0
* @date    2-March-2017
* @brief   This file provides a set of functions needed to manage the 
*          sound terminal device on STM32L4.
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*   1. Redistributions of source code must retain the above copyright notice,
*      this list of conditions and the following disclaimer.
*   2. Redistributions in binary form must reproduce the above copyright notice,
*      this list of conditions and the following disclaimer in the documentation
*      and/or other materials provided with the distribution.
*   3. Neither the name of STMicroelectronics nor the names of its contributors
*      may be used to endorse or promote products derived from this software
*      without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#include "x_nucleo_cca01m1_audio_l4.h"

/** @addtogroup BSP BSP
* @{
*/

/** @addtogroup X_NUCLEO_CCA01M1 X_NUCLEO_CCA01M1
* @{
*/

/** @addtogroup X_NUCLEO_CCA01M1_AUDIO X_NUCLEO_CCA01M1_AUDIO
* @{
*/

/** @defgroup X_NUCLEO_CCA01M1_AUDIO_Private_Defines Private Defines
* @{
*/
#ifndef NULL
#define NULL      (void *) 0
#endif

#define SAIClockDivider(__FREQUENCY__) \
        (__FREQUENCY__ == AUDIO_FREQUENCY_8K)  ? 12 \
      : (__FREQUENCY__ == AUDIO_FREQUENCY_11K) ? 2 \
      : (__FREQUENCY__ == AUDIO_FREQUENCY_16K) ? 6 \
      : (__FREQUENCY__ == AUDIO_FREQUENCY_22K) ? 1 \
      : (__FREQUENCY__ == AUDIO_FREQUENCY_32K) ? 3 \
      : (__FREQUENCY__ == AUDIO_FREQUENCY_44K) ? 0 \
      : (__FREQUENCY__ == AUDIO_FREQUENCY_48K) ? 2 : 1  \

/**
* @}
*/

/** @defgroup X_NUCLEO_CCA01M1_AUDIO_Private_Variables Private Variables
* @{
*/
static DrvContextTypeDef CODEC_Handle[SOUNDTERMINAL_DEVICE_NBR];
SAI_HandleTypeDef hAudioOutSAI[SOUNDTERMINAL_DEVICE_NBR];
/**
* @}
*/

/** @defgroup X_NUCLEO_CCA01M1_AUDIO_Private_Function_Prototypes Private Function Prototypes
* @{
*/
static void SAIx_Init(SAI_HandleTypeDef *hSai, uint32_t AudioFreq);
/**
* @}
*/

/** @defgroup X_NUCLEO_CCA01M1_AUDIO_Exported_Functions Exported Functions
* @{
*/

/**
* @brief  Initialization of AUDIO Device.
* @param  OutputDevice: device to be initialized
*         This parameter can be a value of @ref SOUNDTERMINAL_devices  
* @param  Volume: initialization volume
* @param  AudioFreq: sampling frequency
* @retval AUDIO_OK if no problem during initialization, AUDIO_ERROR otherwise
*/
  uint8_t BSP_AUDIO_OUT_Init(CODEC_ID_t id, void **handle, uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq)
{  
  *handle = NULL;   
  SOUNDTERMINAL_Drv_t *driver = NULL;    
  
  uint8_t tmp = id;
  uint8_t adr_tmp = 0;
  
  switch(id)
  {
  case CODEC_SENSORS_AUTO:
  default:
    {
      tmp = STA350BW_0;  
      hAudioOutSAI[STA350BW_0].Instance = AUDIO_OUT1_SAI_INSTANCE;   
      adr_tmp = STA350BW_ADDRESS_1;
      break;
    }
  case STA350BW_0:
    {
      tmp = STA350BW_0;
      hAudioOutSAI[STA350BW_0].Instance = AUDIO_OUT1_SAI_INSTANCE;   
      adr_tmp = STA350BW_ADDRESS_1;
      break;
    }
  case STA350BW_1:
    {
      tmp = STA350BW_1;
      hAudioOutSAI[STA350BW_1].Instance = AUDIO_OUT2_SAI_INSTANCE;
      adr_tmp = STA350BW_ADDRESS_2;
      break;
    }
  }  
  
  if(CODEC_Handle[tmp].isInitialized == 1)
  {
    /* We have reached the max num of instance for this component */
    return COMPONENT_ERROR;
  }
  
  if (Sensor_IO_Init() == COMPONENT_ERROR )
  {
    return COMPONENT_ERROR;
  }

  
  /* Setup handle. */
  CODEC_Handle[tmp].who_am_i      = 0;
  CODEC_Handle[tmp].ifType        = 0; // I2C interface
  CODEC_Handle[tmp].address       = adr_tmp;
  CODEC_Handle[tmp].instance      = tmp;
  CODEC_Handle[tmp].isInitialized = 0;
  CODEC_Handle[tmp].isEnabled     = 0;
  CODEC_Handle[tmp].isCombo       = 1;
  CODEC_Handle[tmp].pData         = ( void * )NULL;
  CODEC_Handle[tmp].pVTable       = ( void * )&STA350BW_Drv;
  CODEC_Handle[tmp].pExtVTable    = 0;
  
  *handle = (void *)&CODEC_Handle[tmp];  
  driver = ( SOUNDTERMINAL_Drv_t * )((DrvContextTypeDef *)(*handle))->pVTable;
  
  /* PLL clock is set depending by the AudioFreq (44.1khz vs 48khz groups) */ 
  BSP_AUDIO_OUT_ClockConfig(&hAudioOutSAI[tmp], AudioFreq, NULL);  
  
  if(HAL_SAI_GetState(&hAudioOutSAI[tmp]) == HAL_SAI_STATE_RESET)
  {
    /* Init the I2S MSP: this __weak function can be redefined by the application*/
    BSP_AUDIO_OUT_MspInit(*handle,&hAudioOutSAI[tmp], NULL);
  }
  
  SAIx_Init(&hAudioOutSAI[tmp], AudioFreq);  
     
  uint16_t dummy[16]={0};
  
  HAL_SAI_Transmit_DMA(&hAudioOutSAI[tmp], (uint8_t *)dummy, 32);
  
  BSP_AUDIO_OUT_Reset(handle);
  
  if ( driver->Init == NULL )
  {
    memset((*handle), 0, sizeof(DrvContextTypeDef));
    *handle = NULL;
    return COMPONENT_ERROR;
  }
  
  if (driver->Init((DrvContextTypeDef *)(*handle), Volume, AudioFreq, NULL ) == COMPONENT_ERROR )
  {
    memset((*handle), 0, sizeof(DrvContextTypeDef));
    *handle = NULL;
    return COMPONENT_ERROR;
  }
  
  HAL_SAI_DMAStop(&hAudioOutSAI[tmp]);

  if(tmp == STA350BW_0)
  {    
    HAL_NVIC_SetPriority(AUDIO_OUT1_SAI_DMAx_IRQ, AUDIO_OUT_IRQ_PREPRIO, 0);
    HAL_NVIC_EnableIRQ(AUDIO_OUT1_SAI_DMAx_IRQ);
  }
  else if (tmp == STA350BW_1)
  {
    HAL_NVIC_SetPriority(AUDIO_OUT2_SAI_DMAx_IRQ, AUDIO_OUT_IRQ_PREPRIO, 0);
    HAL_NVIC_EnableIRQ(AUDIO_OUT2_SAI_DMAx_IRQ);
  }  
  
  return COMPONENT_OK;
}

/**
* @brief  De-initialization of AUDIO Device.
* @param  handle: device handle 
* @retval COMPONENT_OK if no problem during execution, COMPONENT_ERROR otherwise
*/
uint8_t BSP_AUDIO_OUT_DeInit(void *handle)
{ 
  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  
  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }
  
  HAL_SAI_DMAStop(&hAudioOutSAI[ctx->instance]);
  /* Init the I2S MSP: this __weak function can be redefined by the application*/
  BSP_AUDIO_OUT_MspDeInit(handle,&hAudioOutSAI[ctx->instance], NULL); 
  return COMPONENT_OK; 
}


/**
* @brief  Starts audio streaming to the AUDIO Device.
* @param  handle: device handle
* @param  *pBuffer: pointer to the data to be streamed
* @param  Size: data size
* @retval COMPONENT_OK if no problem during execution, COMPONENT_ERROR otherwise
*/
uint8_t BSP_AUDIO_OUT_Play(void *handle, uint16_t *pBuffer, uint32_t Size)
{
  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  SOUNDTERMINAL_Drv_t *driver = NULL;  
  
  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }
  
  driver = ( SOUNDTERMINAL_Drv_t * )ctx->pVTable;  
  
  /* Call the audio Codec Play function */
  if(driver->Play(ctx, pBuffer, Size, NULL) != 0)
  {  
    return COMPONENT_ERROR;
  }
  else
  {      
    HAL_SAI_Transmit_DMA(&hAudioOutSAI[ctx->instance], (uint8_t *)pBuffer, DMA_MAX(Size)); 
    return COMPONENT_OK;
  }
}



/**
* @brief  This function Pauses the audio stream. In case
*         of using DMA, the DMA Pause feature is used.
* @param  handle: device handle
* @WARNING When calling BSP_AUDIO_OUT_Pause() function for pause, only
*          BSP_AUDIO_OUT_Resume() function should be called for resume (use of BSP_AUDIO_OUT_Play() 
*          function for resume could lead to unexpected behavior).
* @retval COMPONENT_OK if no problem during execution, COMPONENT_ERROR otherwise
*/
uint8_t BSP_AUDIO_OUT_Pause(void *handle)
{    
  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  SOUNDTERMINAL_Drv_t *driver = NULL;  
  
  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }
  
  driver = ( SOUNDTERMINAL_Drv_t * )ctx->pVTable;  
  
  /* Call the Audio Codec Pause/Resume function */
  if(driver->Pause(ctx,NULL) != 0)
  {
    return COMPONENT_ERROR;
  }
  else
  {
    /* Call the Media layer pause function */
    HAL_SAI_DMAPause(&hAudioOutSAI[ctx->instance]);
    
    /* Return COMPONENT_OK when all operations are correctly done */
    return COMPONENT_OK;
  }
}

/**
* @brief  Resumes the audio stream.
* @param  handle: device handle
* @retval COMPONENT_OK if no problem during execution, COMPONENT_ERROR otherwise
*/
uint8_t BSP_AUDIO_OUT_Resume(void *handle)
{   
  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  SOUNDTERMINAL_Drv_t *driver = NULL;  
  
  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }
  
  driver = ( SOUNDTERMINAL_Drv_t * )ctx->pVTable;  
  
  /* Call the Audio Codec Pause/Resume function */
  if(driver->Resume(ctx,NULL) != 0)
  {
    return COMPONENT_ERROR;
  }
  else
  {
    /* Call the Media layer pause function */
    HAL_SAI_DMAResume(&hAudioOutSAI[ctx->instance]);
    
    /* Return COMPONENT_OK when all operations are correctly done */
    return COMPONENT_OK;
  }
}

/**
* @brief  Stop the audio stream.
* @param  handle: device handle
* @retval COMPONENT_OK if no problem during execution, COMPONENT_ERROR otherwise
*/
uint8_t BSP_AUDIO_OUT_Stop(void *handle, uint32_t Option)
{
  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  SOUNDTERMINAL_Drv_t *driver = NULL;  
  
  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }
  
  driver = ( SOUNDTERMINAL_Drv_t * )ctx->pVTable;  
  
  /* Call the Audio Codec Pause/Resume function */
  if(driver->Stop(ctx,NULL) != 0)
  {
    return COMPONENT_ERROR;
  }
  else
  {
    /* Call the Media layer pause function */
    HAL_SAI_DMAStop(&hAudioOutSAI[ctx->instance]);
    
    /* Return COMPONENT_OK when all operations are correctly done */
    return COMPONENT_OK;
  }
}

/**
* @brief  Set volume.
* @param  handle: device handle
* @param  channel: channel to be configured
* @retval COMPONENT_OK if no problem during execution, COMPONENT_ERROR otherwise
*/
uint8_t BSP_AUDIO_OUT_SetVolume(void *handle, uint8_t channel, uint8_t value)
{
  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  SOUNDTERMINAL_Drv_t *driver = NULL;  
  
  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }
  
  driver = ( SOUNDTERMINAL_Drv_t * )ctx->pVTable;  
  
  if(driver->SetVolume(ctx, channel, value, NULL) != 0)
  {
    return COMPONENT_ERROR;
  }
  return COMPONENT_OK;  
}

/**
* @brief  Set Equalization.
* @param  handle: device handle
* @param  ramBlock: device RAM block to be written
* @param  filterNumber: filter to be used
* @param  filterValues: pointer to filter values    
* @retval COMPONENT_OK if no problem during execution, COMPONENT_ERROR otherwise
* @note   for specific information about biquadratic filters setup, please refer 
*         to the STA350BW component datasheet, available at 
*         http://www.st.com/web/catalog/sense_power/FM125/SC1756/PF251568?s_searchtype=partnumber
*/
uint8_t BSP_AUDIO_OUT_SetEq(void *handle, uint8_t ramBlock, uint8_t filterNumber, uint32_t * filterValues)
{    
  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  SOUNDTERMINAL_Drv_t *driver = NULL;  
  
  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }
  
  driver = ( SOUNDTERMINAL_Drv_t * )ctx->pVTable;  
  
  if(driver->SetEq(ctx, ramBlock, filterNumber, filterValues, NULL) != 0)
  {
    return COMPONENT_ERROR;
  }
  return COMPONENT_OK;    
}

/**
* @brief  Set Tone.
* @param  handle: device handle
* @retval COMPONENT_OK if no problem during execution, COMPONENT_ERROR otherwise
*/
uint8_t BSP_AUDIO_OUT_SetTone(void *handle, uint8_t toneGain)
{
  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  SOUNDTERMINAL_Drv_t *driver = NULL;  
  
  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }
  
  driver = ( SOUNDTERMINAL_Drv_t * )ctx->pVTable;  
  
  if(driver->SetTone(ctx, toneGain, NULL) != 0)
  {
    return COMPONENT_ERROR;
  }
  return COMPONENT_OK;  
}

/**
* @brief  Set mute.
* @param  handle: device handle
* @param  channel: channel to be muted
* @param  state: enable or disable value
*         This parameter can be a value of @ref STA350BW_state_define 
* @param  filterValues: pointer to filter values    
* @retval COMPONENT_OK if no problem during execution, COMPONENT_ERROR otherwise
*/
uint8_t BSP_AUDIO_OUT_SetMute(void *handle, uint8_t channel, uint8_t state)
{  
  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  SOUNDTERMINAL_Drv_t *driver = NULL;  
  
  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }
  
  driver = ( SOUNDTERMINAL_Drv_t * )ctx->pVTable;  
  
  if(driver->SetMute(ctx, channel, state, NULL) != 0)
  {
    return COMPONENT_ERROR;
  }
  return COMPONENT_OK;      
}

/**
* @brief  Set frequency of the I2S bus.
* @param  handle: device handle
* @param  AudioFreq: sampling frequency
* @retval COMPONENT_OK if no problem during execution, COMPONENT_ERROR otherwise
*/
uint8_t BSP_AUDIO_OUT_SetFrequency(void *handle, uint32_t AudioFreq)
{ 
  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  SOUNDTERMINAL_Drv_t *driver = NULL;  
  
  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }
  
  driver = ( SOUNDTERMINAL_Drv_t * )ctx->pVTable;  
  
  if(driver->SetFrequency(ctx, AudioFreq, NULL) != 0)
  {
    return COMPONENT_ERROR;
  } 
  
  /* PLL clock is set depending by the AudioFreq (44.1khz vs 48khz groups) */   
  BSP_AUDIO_OUT_ClockConfig(&hAudioOutSAI[ctx->instance], AudioFreq, NULL);  
  SAIx_Init(&hAudioOutSAI[ctx->instance], AudioFreq);  
  
  return COMPONENT_OK; 
}

/**
* @brief  Power off the device.
* @param  handle: device handle
* @retval COMPONENT_OK if no problem during execution, COMPONENT_ERROR otherwise
*/
uint8_t BSP_AUDIO_OUT_PowerOff(void *handle)
{
  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  SOUNDTERMINAL_Drv_t *driver = NULL;  
  
  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }
  
  driver = ( SOUNDTERMINAL_Drv_t * )ctx->pVTable;
  
  if(driver->PowerOff(ctx, NULL) != 0)
  {
    return COMPONENT_ERROR;
  }  
  return COMPONENT_OK; 
}

/**
* @brief  Power on the device.
* @param  handle: device handle
* @retval COMPONENT_OK if no problem during execution, COMPONENT_ERROR otherwise
*/
uint8_t BSP_AUDIO_OUT_PowerOn(void *handle)
{
  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  SOUNDTERMINAL_Drv_t *driver = NULL;  
  
  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }
  
  driver = ( SOUNDTERMINAL_Drv_t * )ctx->pVTable;
  
  if(driver->PowerOn(ctx, NULL) != 0)
  {
    return COMPONENT_ERROR;
  }
  return COMPONENT_OK; 
  
}

/**
* @brief  Stop playing audio stream 
* @param  handle: device handle
* @retval COMPONENT_OK if correct communication, else wrong communication
*/
uint8_t BSP_AUDIO_OUT_EnterLowPowerMode(void *handle)
{    
  return COMPONENT_OK; 
}

/**
* @brief  Stop playing audio stream 
* @param  handle: device handle
* @retval COMPONENT_OK if correct communication, else wrong communication
*/
uint8_t BSP_AUDIO_OUT_ExitLowPowerMode(void *handle)
{  
  return COMPONENT_OK;   
}

/**
* @brief  This function can be used to set advanced DSP options in order to 
*         use advanced features on the STA350BW device.
* @param  handle: device handle
* @param  option: specific option to be setted up
*         This parameter can be a value of @ref STA350BW_DSP_option_selection 
* @param  state: state of the option to be controlled. Depending on the selected 
*         DSP feature to be controlled, this value can be either ENABLE/DISABLE 
*         or a specific numerical parameter related to the specific DSP function. 
*         This parameter can be a value of @ref STA350BW_state_define   
* @retval COMPONENT_OK if no problem during execution, COMPONENT_ERROR otherwise
*/
uint8_t BSP_AUDIO_OUT_SetDSPOption(void *handle, uint8_t option, uint8_t state)
{  
  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  SOUNDTERMINAL_Drv_t *driver = NULL;  
  
  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }
  
  driver = ( SOUNDTERMINAL_Drv_t * )ctx->pVTable;
  
  if(driver->SetDSPOption(ctx, option, state, NULL) != 0)
  {
    return COMPONENT_ERROR;
  }
  return COMPONENT_OK;    
}

/**
* @brief  Initializes BSP_AUDIO_OUT MSP.
* @param  handle: device handle
* @param  hi2s: I2S handle if required
* @param  Params: additional parameters where required
* @retval COMPONENT_OK if no problem during execution, COMPONENT_ERROR otherwise
*/
__weak uint8_t BSP_AUDIO_OUT_MspInit(void *handle, SAI_HandleTypeDef *hSai, void *Params)
{ 

DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;

  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }
  
  GPIO_InitTypeDef  gpio_init_structure;    
  
  if(ctx->instance == STA350BW_0)
  {    
    static DMA_HandleTypeDef hdma_SaiTx_1;   
    
    AUDIO_OUT1_SAI_CLK_ENABLE();
    AUDIO_OUT1_SAI_SD_CLK_ENABLE(); 
    AUDIO_OUT1_SAI_SCK_CLK_ENABLE();
    AUDIO_OUT1_SAI_MCK_CLK_ENABLE();
    AUDIO_OUT1_SAI_FS_CLK_ENABLE();
    
    /* I2S3 pins configuration: WS, SCK and SD pins -----------------------------*/
    gpio_init_structure.Pin         = AUDIO_OUT1_SAI_SCK_PIN ; 
    gpio_init_structure.Mode        = GPIO_MODE_AF_PP;
    gpio_init_structure.Pull        = GPIO_PULLDOWN;
    gpio_init_structure.Speed       = GPIO_SPEED_FAST;
    gpio_init_structure.Alternate   = AUDIO_OUT1_SAI_SCK_SD_WS_MCK_AF;
    HAL_GPIO_Init(AUDIO_OUT1_SAI_SCK_GPIO_PORT, &gpio_init_structure);
    
    gpio_init_structure.Pin         = AUDIO_OUT1_SAI_SD_PIN ;
    HAL_GPIO_Init(AUDIO_OUT1_SAI_SD_GPIO_PORT, &gpio_init_structure);     
    
    gpio_init_structure.Pin         = AUDIO_OUT1_SAI_FS_PIN ;
    HAL_GPIO_Init(AUDIO_OUT1_SAI_FS_GPIO_PORT, &gpio_init_structure); 
    
    gpio_init_structure.Pin         = AUDIO_OUT1_SAI_MCK_PIN; 
    HAL_GPIO_Init(AUDIO_OUT1_SAI_MCK_GPIO_PORT, &gpio_init_structure);   
    
    /* Enable the I2S DMA clock */
    AUDIO_OUT1_SAI_DMAx_CLK_ENABLE(); 
    
    if(hSai->Instance == AUDIO_OUT1_SAI_INSTANCE)
    {
      /* Configure the hdma_i2sTx handle parameters */  
      
      hdma_SaiTx_1.Instance             = AUDIO_OUT1_SAI_DMAx_INSTANCE;  
      hdma_SaiTx_1.Init.Request             = AUDIO_OUT1_SAI_DMAx_REQUEST;  
      hdma_SaiTx_1.Init.Direction           = DMA_MEMORY_TO_PERIPH;
      hdma_SaiTx_1.Init.PeriphInc           = DMA_PINC_DISABLE;
      hdma_SaiTx_1.Init.MemInc              = DMA_MINC_ENABLE;
      hdma_SaiTx_1.Init.PeriphDataAlignment = AUDIO_OUT1_SAI_DMAx_PERIPH_DATA_SIZE;
      hdma_SaiTx_1.Init.MemDataAlignment    = AUDIO_OUT1_SAI_DMAx_MEM_DATA_SIZE;
      hdma_SaiTx_1.Init.Mode                = DMA_CIRCULAR;
      hdma_SaiTx_1.Init.Priority            = DMA_PRIORITY_HIGH;      

      
      /* Associate the DMA handle */
      __HAL_LINKDMA(hSai, hdmatx, hdma_SaiTx_1);
      
      /* Deinitialize the Stream for new transfer */
      HAL_DMA_DeInit(&hdma_SaiTx_1);
      
      /* Configure the DMA Stream */
      HAL_DMA_Init(&hdma_SaiTx_1);
      

      HAL_NVIC_DisableIRQ(AUDIO_OUT1_SAI_DMAx_IRQ); 
    }
    
    /*Reset pin configuration*/
    AUDIO_OUT1_RST_GPIO_CLK_ENABLE();
    
    /* Configure GPIO PINs to detect Interrupts */
    gpio_init_structure.Pin = AUDIO_OUT1_RST_PIN;
    gpio_init_structure.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_structure.Speed = GPIO_SPEED_FAST;
    gpio_init_structure.Pull  = GPIO_NOPULL;
    HAL_GPIO_Init(AUDIO_OUT1_RST_GPIO_PORT, &gpio_init_structure);
    HAL_GPIO_WritePin(AUDIO_OUT1_RST_GPIO_PORT, AUDIO_OUT1_RST_PIN, GPIO_PIN_SET);     
    
  }
  else if (ctx->instance == STA350BW_1)
  {   
    static DMA_HandleTypeDef hdma_SaiTx_2;
    
    AUDIO_OUT2_SAI_CLK_ENABLE();
    AUDIO_OUT2_SAI_SD_CLK_ENABLE(); 
    AUDIO_OUT2_SAI_SCK_CLK_ENABLE();
    AUDIO_OUT2_SAI_MCK_CLK_ENABLE();
    AUDIO_OUT2_SAI_FS_CLK_ENABLE();
    
    /* I2S3 pins configuration: WS, SCK and SD pins -----------------------------*/
    gpio_init_structure.Pin         = AUDIO_OUT2_SAI_SCK_PIN ; 
    gpio_init_structure.Mode        = GPIO_MODE_AF_PP;
    gpio_init_structure.Pull        = GPIO_PULLDOWN;
    gpio_init_structure.Speed       = GPIO_SPEED_FAST;
    gpio_init_structure.Alternate   = AUDIO_OUT2_SAI_SCK_SD_WS_MCK_AF;
    HAL_GPIO_Init(AUDIO_OUT2_SAI_SCK_GPIO_PORT, &gpio_init_structure);
    
    gpio_init_structure.Pin         = AUDIO_OUT2_SAI_SD_PIN ;
    HAL_GPIO_Init(AUDIO_OUT2_SAI_SD_GPIO_PORT, &gpio_init_structure);     
    
    gpio_init_structure.Pin         = AUDIO_OUT2_SAI_FS_PIN ;
    HAL_GPIO_Init(AUDIO_OUT2_SAI_FS_GPIO_PORT, &gpio_init_structure); 
    
    gpio_init_structure.Pin         = AUDIO_OUT2_SAI_MCK_PIN; 
    HAL_GPIO_Init(AUDIO_OUT2_SAI_MCK_GPIO_PORT, &gpio_init_structure);   
    
    /* Enable the I2S DMA clock */
    AUDIO_OUT2_SAI_DMAx_CLK_ENABLE(); 
    
    if(hSai->Instance == AUDIO_OUT2_SAI_INSTANCE)
    {
      /* Configure the hdma_i2sTx handle parameters */   
      hdma_SaiTx_2.Instance             = AUDIO_OUT2_SAI_DMAx_INSTANCE;  
      hdma_SaiTx_2.Init.Request             = AUDIO_OUT2_SAI_DMAx_REQUEST;  
      hdma_SaiTx_2.Init.Direction           = DMA_MEMORY_TO_PERIPH;
      hdma_SaiTx_2.Init.PeriphInc           = DMA_PINC_DISABLE;
      hdma_SaiTx_2.Init.MemInc              = DMA_MINC_ENABLE;
      hdma_SaiTx_2.Init.PeriphDataAlignment = AUDIO_OUT2_SAI_DMAx_PERIPH_DATA_SIZE;
      hdma_SaiTx_2.Init.MemDataAlignment    = AUDIO_OUT2_SAI_DMAx_MEM_DATA_SIZE;
      hdma_SaiTx_2.Init.Mode                = DMA_CIRCULAR;
      hdma_SaiTx_2.Init.Priority            = DMA_PRIORITY_HIGH;

      
      /* Associate the DMA handle */
      __HAL_LINKDMA(hSai, hdmatx, hdma_SaiTx_2);
      
      /* Deinitialize the Stream for new transfer */
      HAL_DMA_DeInit(&hdma_SaiTx_2);
      
      /* Configure the DMA Stream */
      HAL_DMA_Init(&hdma_SaiTx_2); 
      
      HAL_NVIC_DisableIRQ(AUDIO_OUT2_SAI_DMAx_IRQ);       
    }
    
    /*Reset pin configuration*/
    AUDIO_OUT2_RST_GPIO_CLK_ENABLE();
    
    /* Configure GPIO PINs to detect Interrupts */
    gpio_init_structure.Pin = AUDIO_OUT2_RST_PIN;
    gpio_init_structure.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_structure.Speed = GPIO_SPEED_FAST;
    gpio_init_structure.Pull  = GPIO_NOPULL;
    HAL_GPIO_Init(AUDIO_OUT2_RST_GPIO_PORT, &gpio_init_structure);
    HAL_GPIO_WritePin(AUDIO_OUT2_RST_GPIO_PORT, AUDIO_OUT2_RST_PIN, GPIO_PIN_SET);     
  }
  else
  {
    return COMPONENT_ERROR;
  }  
  return COMPONENT_OK;
}

/**
* @brief  Deinitializes I2S MSP.
* @param  handle: device handle
* @param  hi2s: I2S handle if required
* @param  Params: additional parameters where required
* @retval COMPONENT_OK if no problem during execution, COMPONENT_ERROR otherwise
*/
__weak uint8_t BSP_AUDIO_OUT_MspDeInit(void *handle,SAI_HandleTypeDef *hSai, void *Params)
{
  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  
  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }  
  
  if(ctx->instance == STA350BW_0)
  {       
    AUDIO_OUT1_SAI_CLK_DISABLE();  
    
    /* I2S3 pins de initialization: WS, SCK and SD pins -----------------------------*/

    HAL_GPIO_DeInit(AUDIO_OUT1_SAI_SCK_GPIO_PORT, AUDIO_OUT1_SAI_SCK_PIN);  
    HAL_GPIO_DeInit(AUDIO_OUT1_SAI_SD_GPIO_PORT, AUDIO_OUT1_SAI_SD_PIN);    
    HAL_GPIO_DeInit(AUDIO_OUT2_SAI_FS_GPIO_PORT, AUDIO_OUT1_SAI_FS_PIN);
    HAL_GPIO_DeInit(AUDIO_OUT1_SAI_MCK_GPIO_PORT, AUDIO_OUT1_SAI_MCK_PIN);   
    HAL_GPIO_DeInit(AUDIO_OUT1_RST_GPIO_PORT, AUDIO_OUT1_RST_PIN); 
    
    if(hSai->Instance == AUDIO_OUT1_SAI_INSTANCE)
    {     
      /* Deinitialize the Stream for new transfer */
      HAL_DMA_DeInit(hSai->hdmatx); 

      /* I2S DMA IRQ Channel configuration */
      HAL_NVIC_DisableIRQ(AUDIO_OUT1_SAI_DMAx_IRQ); 
    }    
  }
  else if (ctx->instance == STA350BW_1)
  {   
    AUDIO_OUT1_SAI_CLK_DISABLE();  
    
    /* I2S3 pins de initialization: WS, SCK and SD pins -----------------------------*/

    HAL_GPIO_DeInit(AUDIO_OUT2_SAI_SCK_GPIO_PORT, AUDIO_OUT2_SAI_SCK_PIN);  
    HAL_GPIO_DeInit(AUDIO_OUT2_SAI_SD_GPIO_PORT, AUDIO_OUT2_SAI_SD_PIN);    
    HAL_GPIO_DeInit(AUDIO_OUT2_SAI_FS_GPIO_PORT, AUDIO_OUT2_SAI_FS_PIN);
    HAL_GPIO_DeInit(AUDIO_OUT2_SAI_MCK_GPIO_PORT, AUDIO_OUT2_SAI_MCK_PIN);   
    HAL_GPIO_DeInit(AUDIO_OUT2_RST_GPIO_PORT, AUDIO_OUT2_RST_PIN); 
    
    if(hSai->Instance == AUDIO_OUT2_SAI_INSTANCE)
    {     
      /* Deinitialize the Stream for new transfer */
      HAL_DMA_DeInit(hSai->hdmatx); 

      /* I2S DMA IRQ Channel configuration */
      HAL_NVIC_DisableIRQ(AUDIO_OUT2_SAI_DMAx_IRQ); 
    }    
  } 
  else 
  {
    return COMPONENT_ERROR;
  }  	
  
  return COMPONENT_OK;
  /* GPIO pins clock and DMA clock can be shut down in the application 
  by surcharging this __weak function */ 
}

/**
* @brief  Clock Config.
* @param  handle: device handle
* @param  hi2s: I2S handle if required
* @param  Params: additional parameters where required
* @param  AudioFreq: Audio frequency used to play the audio stream.
* @note   This API is called by BSP_AUDIO_OUT_Init() and BSP_AUDIO_OUT_SetFrequency()
*         Being __weak it can be overwritten by the application     
* @retval COMPONENT_OK if no problem during execution, COMPONENT_ERROR otherwise
*/
__weak uint8_t BSP_AUDIO_OUT_ClockConfig(SAI_HandleTypeDef *hSai, uint32_t AudioFreq, void *Params)
{ 
  RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct;
  HAL_RCCEx_GetPeriphCLKConfig(&RCC_PeriphCLKInitStruct);
  
  if (!(AudioFreq % 8))
  {
    /* Audio frequency multiple of 8 (8/16/32/48/96/192)*/    
    RCC_PeriphCLKInitStruct.PLLSAI2.PLLSAI2N        = 43;
    RCC_PeriphCLKInitStruct.PLLSAI2.PLLSAI2P        = 7;    
  }
  else
  {
    RCC_PeriphCLKInitStruct.PLLSAI2.PLLSAI2N        = 24;
    RCC_PeriphCLKInitStruct.PLLSAI2.PLLSAI2P        = 17;
  }
  
  RCC_PeriphCLKInitStruct.PeriphClockSelection    = RCC_PERIPHCLK_SAI2;
  RCC_PeriphCLKInitStruct.PLLSAI2.PLLSAI2ClockOut = RCC_PLLSAI2_SAI2CLK;
  RCC_PeriphCLKInitStruct.Sai2ClockSelection      = RCC_SAI2CLKSOURCE_PLLSAI2;
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);
  
  return COMPONENT_OK;
}

/**
* @brief  Reset the device.
* @param  handle: device handle
* @retval COMPONENT_OK if no problem during execution, COMPONENT_ERROR otherwise 
* @retval None
*/
uint8_t BSP_AUDIO_OUT_Reset(void *handle)
{  
  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  
  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }
  
  if(ctx->instance == STA350BW_0)
  {
    HAL_GPIO_WritePin(AUDIO_OUT1_RST_GPIO_PORT, AUDIO_OUT1_RST_PIN, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(AUDIO_OUT1_RST_GPIO_PORT, AUDIO_OUT1_RST_PIN, GPIO_PIN_SET);  
    return COMPONENT_OK; 
  }
  else if (ctx->instance == STA350BW_1)
  {
    HAL_GPIO_WritePin(AUDIO_OUT2_RST_GPIO_PORT, AUDIO_OUT2_RST_PIN, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(AUDIO_OUT2_RST_GPIO_PORT, AUDIO_OUT2_RST_PIN, GPIO_PIN_SET);   
    return COMPONENT_OK;     
  }
  return COMPONENT_ERROR; 
}

/**
* @brief Tx Transfer completed callbacks
* @param hSai: I2S handle
* @retval None
*/
void HAL_SAI_TxCpltCallback(SAI_HandleTypeDef *hSai)
{
  /* Call the record update function to get the next buffer to fill and its size (size is ignored) */
  if(hSai->Instance == AUDIO_OUT1_SAI_INSTANCE)
  {
    BSP_AUDIO_OUT_TransferComplete_CallBack(STA350BW_0);
  }
  if(hSai->Instance == AUDIO_OUT2_SAI_INSTANCE)
  {
    BSP_AUDIO_OUT_TransferComplete_CallBack(STA350BW_1);
  }    
}

/**
* @brief Tx Transfer Half completed callbacks
* @param hSai: I2S handle
* @retval None
*/
void HAL_SAI_TxHalfCpltCallback(SAI_HandleTypeDef *hSai)
{
  /* Manage the remaining file size and new address offset: This function 
  should be coded by user (its prototype is already declared in stm324xg_eval_audio.h) */  
  if(hSai->Instance == AUDIO_OUT1_SAI_INSTANCE)
  {
    BSP_AUDIO_OUT_HalfTransfer_CallBack(STA350BW_0);
  }
  if(hSai->Instance == AUDIO_OUT2_SAI_INSTANCE)
  {
    BSP_AUDIO_OUT_HalfTransfer_CallBack(STA350BW_1);
  }
}

/**
* @brief  Manages the DMA full Transfer complete event.
* @param  OutputDevice: device to be used
*         This parameter can be a value of @ref SOUNDTERMINAL_devices 
* @retval None
*/
__weak void BSP_AUDIO_OUT_TransferComplete_CallBack(uint16_t OutputDevice)
{
}

/**
* @brief  Manages the DMA Half Transfer complete event.
* @param  OutputDevice: device to be used
*         This parameter can be a value of @ref SOUNDTERMINAL_devices 
* @retval None
*/
__weak void BSP_AUDIO_OUT_HalfTransfer_CallBack(uint16_t OutputDevice)
{ 
}

/**
* @brief  Initializes the Audio Codec audio interface (I2S)
* @note   This function assumes that the I2S input clock (through PLL_R in 
*         Devices RevA/Z and through dedicated PLLI2S_R in Devices RevB/Y)
*         is already configured and ready to be used.    
* @param  AudioFreq: Audio frequency to be configured for the I2S peripheral. 
* @retval None
*/
static void SAIx_Init(SAI_HandleTypeDef *hSai, uint32_t AudioFreq)
{
  
    __HAL_SAI_DISABLE(hSai);
  
  hSai->Init.MonoStereoMode = SAI_STEREOMODE;
  hSai->Init.Mckdiv = SAIClockDivider(AudioFreq);
  hSai->Init.AudioFrequency =  SAI_AUDIO_FREQUENCY_MCKDIV;
  hSai->Init.AudioMode = SAI_MODEMASTER_TX;
  hSai->Init.NoDivider = SAI_MASTERDIVIDER_ENABLE;
  hSai->Init.Protocol = SAI_FREE_PROTOCOL;
  hSai->Init.DataSize = SAI_DATASIZE_16;
  hSai->Init.FirstBit = SAI_FIRSTBIT_MSB;
  hSai->Init.ClockStrobing = SAI_CLOCKSTROBING_RISINGEDGE;
  hSai->Init.Synchro = SAI_ASYNCHRONOUS;
  hSai->Init.OutputDrive = SAI_OUTPUTDRIVE_ENABLE;
  hSai->Init.FIFOThreshold = SAI_FIFOTHRESHOLD_1QF;

  hSai->FrameInit.FrameLength       = 32; 
  hSai->FrameInit.ActiveFrameLength = 16;
  hSai->FrameInit.FSDefinition      = SAI_FS_CHANNEL_IDENTIFICATION;
  hSai->FrameInit.FSPolarity        = SAI_FS_ACTIVE_LOW;
  hSai->FrameInit.FSOffset          = SAI_FS_BEFOREFIRSTBIT;

  hSai->SlotInit.FirstBitOffset = 0;
  hSai->SlotInit.SlotSize       = SAI_SLOTSIZE_16B;
  hSai->SlotInit.SlotNumber     = 2; 
  hSai->SlotInit.SlotActive     = (SAI_SLOTACTIVE_0 | SAI_SLOTACTIVE_1);

HAL_SAI_Init(hSai);
  
  /* Enable SAI to generate clock used by audio driver */
  __HAL_SAI_ENABLE(hSai);
}

/**
* @}
*/

/**
* @}
*/

/**
* @}
*/



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/     

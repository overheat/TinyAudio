/**
******************************************************************************
* @file    x_nucleo_cca01m1_audio_f0.h
* @author  Central Labs
* @version V2.0.0
* @date    2-March-2017
* @brief   This file contains definitions for x_nucleo_cca01m1_audio_f0.c
*          firmware driver.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __X_NUCLEO_CCA01M1_AUDIO_H
#define __X_NUCLEO_CCA01M1_AUDIO_H

#ifdef __cplusplus
extern "C" {
#endif
  
  /* Includes ------------------------------------------------------------------*/
  
#include "..\Components\sta350bw\STA350BW_Driver.h"
#include "x_nucleo_cca01m1.h"
  
  /** @addtogroup BSP
  * @{
  */
  
  /** @addtogroup X_NUCLEO_CCA01M1 X_NUCLEO_CCA01M1
  * @{
  */
  
  /** @addtogroup X_NUCLEO_CCA01M1_AUDIO X_NUCLEO_CCA01M1_AUDIO
  * @{
  */  
  
  /** @addtogroup X_NUCLEO_CCA01M1_AUDIO_Exported_Types Exported types
  * @{
  */
  typedef enum
  {
    CODEC_SENSORS_AUTO = -1,    /* Always first element and equal to -1 */
    STA350BW_0 = 0,                   
    STA350BW_1 = 1
  } CODEC_ID_t;
  /**
  * @}
  */ 
  
  /** @defgroup  X_NUCLEO_CCA01M1_AUDIO_Public_Constants Public Constants 
  * @{
  */
  
  extern I2S_HandleTypeDef hAudioOutI2s[];  
#define SOUNDTERMINAL_DEVICE_NBR   2
  
  /* DEVICE 1 */ 
  /* I2S peripheral configuration defines */
#define I2S_STANDARD                  			I2S_STANDARD_PHILLIPS
#define AUDIO_OUT1_I2S_INSTANCE                 SPI2
#define AUDIO_OUT1_I2S_CLK_ENABLE()             __SPI2_CLK_ENABLE()
#define AUDIO_OUT1_I2S_CLK_DISABLE()            __SPI2_CLK_DISABLE()
#define AUDIO_OUT1_I2S_SCK_SD_WS_AF         	GPIO_AF0_SPI2
#define AUDIO_OUT1_I2S_MCK_AF         		GPIO_AF5_SPI2
#define AUDIO_OUT1_I2S_SD_CLK_ENABLE()          __GPIOB_CLK_ENABLE()
#define AUDIO_OUT1_I2S_SCK_CLK_ENABLE()         __GPIOB_CLK_ENABLE()
#define AUDIO_OUT1_I2S_MCK_CLK_ENABLE()         __GPIOC_CLK_ENABLE()
#define AUDIO_OUT1_I2S_WS_CLK_ENABLE()          __GPIOB_CLK_ENABLE()
#define AUDIO_OUT1_I2S_WS_PIN                   GPIO_PIN_12
#define AUDIO_OUT1_I2S_SCK_PIN                  GPIO_PIN_13
#define AUDIO_OUT1_I2S_SD_PIN                   GPIO_PIN_15
#define AUDIO_OUT1_I2S_MCK_PIN                  GPIO_PIN_2
#define AUDIO_OUT1_I2S_SCK_GPIO_PORT            GPIOB
#define AUDIO_OUT1_I2S_SD_GPIO_PORT             GPIOB
#define AUDIO_OUT1_I2S_WS_GPIO_PORT             GPIOB
#define AUDIO_OUT1_I2S_MCK_GPIO_PORT            GPIOC   
  /* I2S DMA Stream Tx definitions */
#define AUDIO_OUT1_I2S_DMAx_CLK_ENABLE()        __DMA1_CLK_ENABLE()
#define AUDIO_OUT1_I2S_DMAx_STREAM              DMA1_Channel5
#define AUDIO_OUT1_I2S_DMAx_IRQ                 DMA1_Channel4_5_6_7_IRQn
#define AUDIO_OUT1_I2S_DMAx_PERIPH_DATA_SIZE    DMA_PDATAALIGN_HALFWORD
#define AUDIO_OUT1_I2S_DMAx_MEM_DATA_SIZE       DMA_MDATAALIGN_HALFWORD 
#define AUDIO_OUT1_IRQHandler                 	DMA1_Channel4_5_6_7_IRQHandler 
  /* Reset Pin definitions */
#define AUDIO_OUT1_RST_GPIO_PORT                GPIOA
#define AUDIO_OUT1_RST_GPIO_CLK_ENABLE()        __GPIOA_CLK_ENABLE()
#define AUDIO_OUT1_RST_GPIO_CLK_DISABLE()       __GPIOA_CLK_DISABLE()
#define AUDIO_OUT1_RST_PIN                      GPIO_PIN_10  
  /* PD Pin definitions */
#define AUDIO_OUT1_PD_GPIO_PORT           		GPIOA
#define AUDIO_OUT1_PD_GPIO_CLK_ENABLE()   		__GPIOA_CLK_ENABLE()
#define AUDIO_OUT1_PD_GPIO_CLK_DISABLE()  		__GPIOA_CLK_DISABLE()
#define AUDIO_OUT1_PD_PIN                		GPIO_PIN_0

  /* DEVICE 2 */ 
  /* I2S peripheral configuration defines */
#define AUDIO_OUT2_I2S_INSTANCE                  SPI1
#define AUDIO_OUT2_I2S_CLK_ENABLE()             __SPI1_CLK_ENABLE()
#define AUDIO_OUT2_I2S_CLK_DISABLE()            __SPI1_CLK_DISABLE()
#define AUDIO_OUT2_I2S_SCK_SD_WS_AF         	GPIO_AF0_SPI1
#define AUDIO_OUT2_I2S_MCK_AF         		GPIO_AF0_SPI1
#define AUDIO_OUT2_I2S_SD_CLK_ENABLE()          __GPIOA_CLK_ENABLE()
#define AUDIO_OUT2_I2S_SCK_CLK_ENABLE()         __GPIOA_CLK_ENABLE()
#define AUDIO_OUT2_I2S_MCK_CLK_ENABLE()         __GPIOA_CLK_ENABLE()
#define AUDIO_OUT2_I2S_WS_CLK_ENABLE()          __GPIOA_CLK_ENABLE()
#define AUDIO_OUT2_I2S_WS_PIN                   GPIO_PIN_4
#define AUDIO_OUT2_I2S_SCK_PIN                  GPIO_PIN_5
#define AUDIO_OUT2_I2S_SD_PIN                   GPIO_PIN_7
#define AUDIO_OUT2_I2S_MCK_PIN                  GPIO_PIN_6
#define AUDIO_OUT2_I2S_SCK_GPIO_PORT            GPIOA
#define AUDIO_OUT2_I2S_SD_GPIO_PORT             GPIOA
#define AUDIO_OUT2_I2S_WS_GPIO_PORT             GPIOA
#define AUDIO_OUT2_I2S_MCK_GPIO_PORT            GPIOA    
  /* I2S DMA Stream Tx definitions */
#define AUDIO_OUT2_I2S_DMAx_CLK_ENABLE()        __DMA1_CLK_ENABLE()
#define AUDIO_OUT2_I2S_DMAx_STREAM              DMA1_Channel3
#define AUDIO_OUT2_I2S_DMAx_IRQ                 DMA1_Channel2_3_IRQn
#define AUDIO_OUT2_I2S_DMAx_PERIPH_DATA_SIZE    DMA_PDATAALIGN_HALFWORD
#define AUDIO_OUT2_I2S_DMAx_MEM_DATA_SIZE       DMA_MDATAALIGN_HALFWORD 
#define AUDIO_OUT2_IRQHandler                 	DMA1_Channel2_3_IRQHandler 
  /* Reset Pin definitions */
#define AUDIO_OUT2_RST_GPIO_PORT                GPIOA
#define AUDIO_OUT2_RST_GPIO_CLK_ENABLE()        __GPIOA_CLK_ENABLE()
#define AUDIO_OUT2_RST_GPIO_CLK_DISABLE()       __GPIOA_CLK_DISABLE()
#define AUDIO_OUT2_RST_PIN                      GPIO_PIN_8  
  /* PD Pin definitions */  
#define AUDIO_OUT2_PD_GPIO_PORT           		GPIOB
#define AUDIO_OUT2_PD_GPIO_CLK_ENABLE()   		__GPIOB_CLK_ENABLE()
#define AUDIO_OUT2_PD_GPIO_CLK_DISABLE()  		__GPIOB_CLK_DISABLE()
#define AUDIO_OUT2_PD_PIN                		GPIO_PIN_0
  
  /* Select the interrupt preemption priority and subpriority for the IT/DMA interrupt */
#define AUDIO_OUT_IRQ_PREPRIO            6   /* Select the preemption priority level(0 is the highest) */
#define DMA_MAX_SZE                     0xFFFF
#define DMA_MAX(_X_)                (((_X_) <= DMA_MAX_SZE)? (_X_):DMA_MAX_SZE)
  /* Audio status definition */     
#ifndef AUDIO_OK
#define AUDIO_OK                                ((uint8_t)0)
#endif    
#ifndef AUDIO_ERROR
#define AUDIO_ERROR                             ((uint8_t)1)
#endif   
#ifndef AUDIO_TIMEOUT
#define AUDIO_TIMEOUT                           ((uint8_t)2)
#endif   
  /**
  * @}
  */
  
  
  /** @defgroup STAMPINO_F401_AUDIO_Exported_Macros AUDIO_Exported_Macros
  * @{
  */
  
  /** @defgroup  X_NUCLEO_CCA01M1_AUDIO_Exported_Functions 
  * @{
  */
  
  /** @defgroup  X_NUCLEO_CCA01M1_AUDIO_Exported_Functions Exported Functions 
  * @{
  */
  
  uint8_t BSP_AUDIO_OUT_Init(CODEC_ID_t id, void **handle, uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq);  
  uint8_t BSP_AUDIO_OUT_DeInit(void *handle);
  uint8_t BSP_AUDIO_OUT_Play(void *handle, uint16_t* pBuffer, uint32_t Size);
  uint8_t BSP_AUDIO_OUT_Pause(void *handle);
  uint8_t BSP_AUDIO_OUT_Resume(void *handle);
  uint8_t BSP_AUDIO_OUT_Stop(void *handle, uint32_t Option);;
  uint8_t BSP_AUDIO_OUT_SetVolume(void *handle, uint8_t channel, uint8_t value);
  uint8_t BSP_AUDIO_OUT_SetEq(void *handle, uint8_t ramBlock, uint8_t filterNumber, uint32_t * filterValues);
  uint8_t BSP_AUDIO_OUT_SetTone(void *handle, uint8_t toneGain);
  uint8_t BSP_AUDIO_OUT_SetMute(void *handle, uint8_t channel, uint8_t state);
  uint8_t BSP_AUDIO_OUT_SetFrequency(void *handle, uint32_t AudioFreq);
  uint8_t BSP_AUDIO_OUT_EnterLowPowerMode(void *handle);
  uint8_t BSP_AUDIO_OUT_ExitLowPowerMode(void *handle);
  uint8_t BSP_AUDIO_OUT_MspInit(void *handle,I2S_HandleTypeDef *hi2s, void *Params);
  uint8_t BSP_AUDIO_OUT_MspDeInit(void *handle,I2S_HandleTypeDef *hi2s, void *Params);
  uint8_t BSP_AUDIO_OUT_ClockConfig(I2S_HandleTypeDef *hi2s, uint32_t AudioFreq, void *Params);
  uint8_t BSP_AUDIO_OUT_Reset(void *handle);
  uint8_t BSP_AUDIO_OUT_PowerOn(void *handle);
  uint8_t BSP_AUDIO_OUT_PowerOff(void *handle);
  uint8_t BSP_AUDIO_OUT_isInitialized(void *handle);
  uint8_t BSP_AUDIO_OUT_I2S_Init(uint32_t AudioFreq);
  uint8_t BSP_AUDIO_OUT_SetDSPOption(void *handle, uint8_t option, uint8_t state);
  
  /* User Callbacks: user has to implement these functions in his code if they are needed. */
  /* This function is called when the requested data has been completely transferred.*/
  void    BSP_AUDIO_OUT_TransferComplete_CallBack(uint16_t OutputDevice);
  
  /* This function is called when half of the requested buffer has been transferred. */
  void    BSP_AUDIO_OUT_HalfTransfer_CallBack(uint16_t OutputDevice);
  
  /* This function is called when an Interrupt due to transfer error on or peripheral
  error occurs. */
  void    BSP_AUDIO_OUT_Error_CallBack(void);
  
  /**
  * @}
  */
  
  /**
  * @}
  */
  
  /**
  * @}
  */
  
  /**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif /* __X_NUCLEO_CCA01M1_AUDIO_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

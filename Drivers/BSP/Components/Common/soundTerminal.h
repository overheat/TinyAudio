/**
 ******************************************************************************
 * @file    soundTerminal.h
 * @author  Central Labs
 * @version V1.0.0
 * @date    18-August-2015
 * @brief  This file contains all the functions prototypes for the Sound Terminal driver.
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
#ifndef __SOUNDTERMINAL_H
#define __SOUNDTERMINAL_H

#ifdef __cplusplus
extern "C" {
#endif
  
  /* Includes ------------------------------------------------------------------*/
#include <stdint.h> 
#include "component.h"
  
/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */
    
/** @addtogroup SOUND_TERMINAL
  * @{
  */

/** @defgroup SOUND_TERMINAL_Exported_Types
  * @{
  */

  
  /** 
  * @brief  SoundTerminal driver structure definition  
  */ 
  typedef struct
  {  
    int32_t        (*Init)(DrvContextTypeDef *, uint16_t,  uint32_t, void *p);
    int32_t        (*DeInit)(DrvContextTypeDef *, void *p);
    int32_t        (*ReadID)(DrvContextTypeDef *, void *p);
    int32_t        (*Play)(DrvContextTypeDef *, uint16_t *, uint16_t, void *p);
    int32_t        (*Pause)(DrvContextTypeDef *, void *p);
    int32_t        (*Resume)(DrvContextTypeDef *, void *p);
    int32_t        (*Stop)(DrvContextTypeDef *, void *p);
    int32_t        (*PowerOn)(DrvContextTypeDef *, void *p);
    int32_t        (*PowerOff)(DrvContextTypeDef *, void *p);
    int32_t        (*Reset)(DrvContextTypeDef *, void *p);
    int32_t        (*SetEq)(DrvContextTypeDef *, uint8_t, uint8_t, uint32_t *, void *p);
    int32_t        (*SetTone)(DrvContextTypeDef *, uint8_t, void *p);
    int32_t        (*SetMute)(DrvContextTypeDef *, uint8_t, uint8_t, void *p); 
    int32_t        (*SetVolume)(DrvContextTypeDef *, uint8_t, uint8_t, void *p);
    int32_t        (*SetFrequency)(DrvContextTypeDef *, uint32_t, void *p);
    int32_t        (*SetDSPOption)(DrvContextTypeDef *, uint8_t, uint8_t, void *p);
  }SOUNDTERMINAL_Drv_t;
  

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

#endif /* __SOUNDTERMINAL_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/ 

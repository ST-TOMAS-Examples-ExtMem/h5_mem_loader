/**
  ******************************************************************************
  * @file           : extmem_user_driver.c
  * @version        : 1.0.0
  * @brief          : This file implements the extmem configuration
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32_extmem.h"
#include "stm32_extmem_conf.h"
#if EXTMEM_DRIVER_USER == 1
#include "stm32_user_driver_api.h"
#include "stm32_user_driver_type.h"

#include "stm32_extmemloader_conf.h"
/** @defgroup USER USER driver
  * @ingroup EXTMEM_DRIVER
  * @{
  */

/* Private Macro ------------------------------------------------------------*/
/** @defgroup USER_Private_Macro Private Macro
  * @{
  */

/**
  * @}
  */
/* Private typedefs ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t* buffer;
/* Private functions ---------------------------------------------------------*/

/** @defgroup USER_Exported_Functions Exported Functions
  * @{
  */

EXTMEM_DRIVER_USER_StatusTypeDef EXTMEM_DRIVER_USER_Init(uint32_t MemoryId,
                                                         EXTMEM_DRIVER_USER_ObjectTypeDef* UserObject)
{
  EXTMEM_DRIVER_USER_StatusTypeDef retr = EXTMEM_DRIVER_USER_OK;
  UserObject->MemID = MemoryId;     /* Keep the memory id, could be used to control more than one user memory */
  UserObject->PtrUserDriver = NULL; /* could be used to link data with the memory id */
  buffer=(uint8_t*)0x2009E000;
  return retr;
}

EXTMEM_DRIVER_USER_StatusTypeDef EXTMEM_DRIVER_USER_DeInit(EXTMEM_DRIVER_USER_ObjectTypeDef* UserObject)
{
  EXTMEM_DRIVER_USER_StatusTypeDef retr = EXTMEM_DRIVER_USER_OK;
  (void)UserObject;
  return retr;
}

EXTMEM_DRIVER_USER_StatusTypeDef EXTMEM_DRIVER_USER_Read(EXTMEM_DRIVER_USER_ObjectTypeDef* UserObject,
                                                         uint32_t Address, uint8_t* Data, uint32_t Size)
{
  EXTMEM_DRIVER_USER_StatusTypeDef retr = EXTMEM_DRIVER_USER_OK;
  (void)*UserObject;
  (void)Address;
  (void)Data;
  (void)Size;
  uint32_t masked_address = Address & 0xFFFFFF;
  memcpy(Data,(void*)(buffer+masked_address),Size);
  return retr;
}

EXTMEM_DRIVER_USER_StatusTypeDef EXTMEM_DRIVER_USER_Write(EXTMEM_DRIVER_USER_ObjectTypeDef* UserObject,
                                                          uint32_t Address, const uint8_t* Data, uint32_t Size)
{
  EXTMEM_DRIVER_USER_StatusTypeDef retr = EXTMEM_DRIVER_USER_OK;
  (void)*UserObject;
  (void)Address;
  (void)Data;
  (void)Size;
  uint32_t masked_address = Address & 0xFFFFFF;
  memcpy((void*)(buffer+masked_address),Data,Size);
  return retr;
}

EXTMEM_DRIVER_USER_StatusTypeDef EXTMEM_DRIVER_USER_EraseSector(EXTMEM_DRIVER_USER_ObjectTypeDef* UserObject,
                                                                uint32_t Address, uint32_t Size)
{
  EXTMEM_DRIVER_USER_StatusTypeDef retr = EXTMEM_DRIVER_USER_OK;
  (void)*UserObject;
  (void)Address;
  (void)Size;
  uint32_t masked_address = Address & 0xFFFFFF;
  memset((void*)(buffer+masked_address),0xFF,Size);
  return retr;
}

EXTMEM_DRIVER_USER_StatusTypeDef EXTMEM_DRIVER_USER_MassErase(EXTMEM_DRIVER_USER_ObjectTypeDef* UserObject)
{
  EXTMEM_DRIVER_USER_StatusTypeDef retr = EXTMEM_DRIVER_USER_NOTSUPPORTED;
  (void)*UserObject;
  return retr;
}

EXTMEM_DRIVER_USER_StatusTypeDef EXTMEM_DRIVER_USER_Enable_MemoryMappedMode(EXTMEM_DRIVER_USER_ObjectTypeDef* UserObject)
{
  EXTMEM_DRIVER_USER_StatusTypeDef retr = EXTMEM_DRIVER_USER_OK;
  (void)*UserObject;
  return retr;
}

EXTMEM_DRIVER_USER_StatusTypeDef EXTMEM_DRIVER_USER_Disable_MemoryMappedMode(EXTMEM_DRIVER_USER_ObjectTypeDef* UserObject)
{
  EXTMEM_DRIVER_USER_StatusTypeDef retr = EXTMEM_DRIVER_USER_OK;
  (void)*UserObject;
  return retr;
}

EXTMEM_DRIVER_USER_StatusTypeDef EXTMEM_DRIVER_USER_GetMapAddress(EXTMEM_DRIVER_USER_ObjectTypeDef* UserObject, uint32_t* BaseAddress)
{
  EXTMEM_DRIVER_USER_StatusTypeDef retr = EXTMEM_DRIVER_USER_NOTSUPPORTED;
  (void)*UserObject;
  (void)BaseAddress;
  return retr;
}

EXTMEM_DRIVER_USER_StatusTypeDef EXTMEM_DRIVER_USER_GetInfo(EXTMEM_DRIVER_USER_ObjectTypeDef* UserObject, EXTMEM_USER_MemInfoTypeDef* MemInfo)
{
  EXTMEM_DRIVER_USER_StatusTypeDef retr = EXTMEM_DRIVER_USER_NOTSUPPORTED;
  (void)*UserObject;
  (void)MemInfo;
  return retr;
}

/**
  * @}
  */

/** @addtogroup USER_Private_Functions
  * @{
  */

/**
  * @}
  */

/**
  * @}
  */
#endif /* EXTMEM_DRIVER_USER == 1 */

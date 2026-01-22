# Guide to use external_loader and memory_manager on different device than H7SR

External memory manager/loader is useful tool to start creating your own custom loader. But unfirtunately it is only for some devices like N6/H7RS

Purpose of this example is to show how to use memory_manager and external_loader from H7RS on random STM32.

Demonstrated on H562.

For the i'll use user mmeory configuration which allow me to use any memory. In this case i'll demonstrate it on RAM buffer. 

Currenlty used only with CubeIDE to create loader for STM32CubeProgrammer
Only USER memory used. Other were not tested


# Create independent extmem_loader_manager



# Create MX project for H562

1. Create project for H5 or any other device
2. Because we will use ram as storage no peripherals are needed. 
3. Store generate project for cubeIDE(other IDE is also possible)

# Modify H562 project for memory loader/manager

## Add needed files
1. Copy extmem_loader_manager to H5 project
2. Open H5 project in CubeIDE
3. Allow to compile ext_mem_loader_manager

## Add this paths to include

```
../extmem_loader_manager/STM32_ExtMem_Manager
```

```
../extmem_loader_manager/STM32_ExtMem_Manager/user
```

```
../extmem_loader_manager/STM32_ExtMem_Loader/core
```

```
../extmem_loader_manager/STM32_ExtMem_Loader/STM32Cube
```

```
../extmem_loader_manager/user
```

## Add this to defines
  
  For CubeIDE/CubeProgrammer
  
  ```
STM32_EXTMEMLOADER_STM32CUBETARGET
  ```

To use read function on memory not mapped to address space

```c
READ_FUNCTION
```

## Set linker

to `${workspace_loc:/${ProjName}/extmem_loader_manager/loader_linker.ld}`

You need to opend the linker and check memory size to fit for your device.

```gcc
/* Specify the memory areas */
MEMORY
{
  RAM_D1 (xrw)      : ORIGIN = 0x20000004, LENGTH = 64K-4
}
```

The `ORIGIN` is usually ok for STM32 but `LENGTH` need to fit to the device. On H562 length is ok

## Remove this files from project
  Loader not need them or they are already in other files
  From `Core/src`
    - main.c
  From `Core/src`
    - startup/startup_stm32h563zitx.s (on different device the file will have different name)
  

## Describe our memory

  Open `stm32_extmemloader_conf.h` in `extmem_loader_manager\user`

  We can describe our memory for CubeProgrammer. 

```c
#define	STM32EXTLOADER_DEVICE_NAME            "h5_memo_loader"
```

I recommend keep the name of the loader same as your project

```c
#define	STM32EXTLOADER_DEVICE_ADDR            0x60000000
```

Location of our memory. It should be in cortex External Memory aread 0x60000000-0xDFFFFFFF
In our case we dont have any memory physically so we can use `0x64000000`

```c
#define	STM32EXTLOADER_DEVICE_SIZE            0x2000
```

Our memory size. We will sotre it in ram so it is size of our RAM buffer

```c
#define	STM32EXTLOADER_DEVICE_SECTOR_NUMBERS  0x20
```

How many sectors we have (sector defie erasable part of memory in some memories called blocks)

```c
#define	STM32EXTLOADER_DEVICE_SECTOR_SIZE     0x100
#define	STM32EXTLOADER_DEVICE_PAGE_SIZE       0x100
```

Size of secort and size or page. Page is size needed to program at one time. 

So now our loader will erase and program only 256B nothing smaller. 


```c
#define	STM32EXTLOADER_DEVICE_NAME            "h5_memo_loader"
#define STM32EXTLOADER_DEVICE_TYPE            NOR_FLASH
#define	STM32EXTLOADER_DEVICE_ADDR            0x64000000
#define	STM32EXTLOADER_DEVICE_SIZE            0x2000
#define	STM32EXTLOADER_DEVICE_SECTOR_NUMBERS  0x20
#define	STM32EXTLOADER_DEVICE_SECTOR_SIZE     0x100
#define	STM32EXTLOADER_DEVICE_PAGE_SIZE       0x100
#define	STM32EXTLOADER_DEVICE_INITIAL_CONTENT 0xFF
#define STM32EXTLOADER_DEVICE_PAGE_TIMEOUT    10000
#define STM32EXTLOADER_DEVICE_SECTOR_TIMEOUT  6000
```

## Implement memory

Add include to `stm32_extmemloader_conf` that we can use the seze defines

```c
#include "stm32_extmemloader_conf.h"
```

it will looks like

```c
/* Includes ------------------------------------------------------------------*/
#include "stm32_extmem.h"
#include "stm32_extmem_conf.h"
#if EXTMEM_DRIVER_USER == 1
#include "stm32_user_driver_api.h"
#include "stm32_user_driver_type.h"

#include "stm32_extmemloader_conf.h"
/** @defgroup USER USER driver
```

we define our memory buffer only as pointer we define location in int

```c
uint8_t* buffer;
```

```c
/* Private variables ---------------------------------------------------------*/
uint8_t* buffer;
```

### Init implementation

```c
EXTMEM_DRIVER_USER_StatusTypeDef EXTMEM_DRIVER_USER_Init(uint32_t MemoryId,
                                                         EXTMEM_DRIVER_USER_ObjectTypeDef* UserObject)
{
  EXTMEM_DRIVER_USER_StatusTypeDef retr = EXTMEM_DRIVER_USER_OK;
  UserObject->MemID = MemoryId;     /* Keep the memory id, could be used to control more than one user memory */
  UserObject->PtrUserDriver = NULL; /* could be used to link data with the memory id */
  buffer=(uint8_t*)0x2009E000;
  return retr;
}
```

we need to set correct return value `EXTMEM_DRIVER_USER_OK`

We point our buffer to RAM. Best is take end of RAM for test. Or different rm on devices with more RAM memories. 
buffer=(uint8_t*)0x2009E000;



### Deinit 

```c
EXTMEM_DRIVER_USER_StatusTypeDef EXTMEM_DRIVER_USER_DeInit(EXTMEM_DRIVER_USER_ObjectTypeDef* UserObject)
{
  EXTMEM_DRIVER_USER_StatusTypeDef retr = EXTMEM_DRIVER_USER_OK;
  (void)UserObject;
  return retr;
}
```

Mainly change return value


### Read

```c
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
```

For sure mask the address because we are starting 0 in our buffer.


### Write

```c
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
```

Write similar to read. 

### Sector erase
```c
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
```

Sector erase. We will get the address and size to erase. The manager+loader will give only sizes dividable by STM32EXTLOADER_DEVICE_SECTOR_SIZE

### Mass erase

```c
EXTMEM_DRIVER_USER_StatusTypeDef EXTMEM_DRIVER_USER_MassErase(EXTMEM_DRIVER_USER_ObjectTypeDef* UserObject)
{
  EXTMEM_DRIVER_USER_StatusTypeDef retr = EXTMEM_DRIVER_USER_NOTSUPPORTED;
  (void)*UserObject;
  return retr;
}
```

We will not implement it now to kee i simple. 

### Memory mapped mode

```c
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
```

We only change return value because it is called in memory manager/loader

### Rest of functions

```c
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
```

for external laoder we dont need to implment this


### Compile and try

1. Compile project
2. Rename elf to stldr. It is in `Debug` folder `STM32CubeProgrammer\bin\ExternalLoader\`
3. Copy stldr file to STM32Cubeprogrammer ``
4. Start STM32CubeProgrammer
5. Select our external memory loader
6. Try program external memory


# Conclusion

We used a extmem_loader_manager to add it to different STM32

This can bring simplier external laoder creation or porting to different device
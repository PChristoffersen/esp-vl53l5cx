/**
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  * 
  * Copyright (c) 2024 Peter Christoffersen
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */


#ifndef _VL53L5CX_PLATFORM_H_
#define _VL53L5CX_PLATFORM_H_
#pragma once

#include <stdint.h>
#include <string.h>

#include <driver/i2c_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Structure VL53L5CX_Platform needs to be filled by the customer,
 * depending on his platform. At least, it contains the VL53L5CX I2C address.
 * Some additional fields can be added, as descriptors, or platform
 * dependencies. Anything added into this structure is visible into the platform
 * layer.
 */

typedef struct
{
	/* To be filled with customer's platform. At least an I2C address/descriptor
	 * needs to be added */
	/* Example for most standard platform : I2C address of sensor */
    uint16_t  			address;

	// ESP i2c device handle
	i2c_master_dev_handle_t handle;
} VL53L5CX_Platform;

/*
 * Supported speeds
 *
 * Note: It is untested whether Fast-mode-plus (Fm+) that can be used reliably on any ESP32 variant
 */
#define VL53L5CX_MIN_SCL_SPEED_HZ  		100000U // Standard-mode (Sm)
#define VL53L5CX_DEFAULT_SCL_SPEED_HZ 	400000U // Fase-mode (Fm)
#define VL53L5CX_MAX_SCL_SPEED_HZ 	   1000000U // Fast-mode-plus (Fm+)


#define VL53L5CX_I2C_TIMEOUT_MS 50



/*
 * @brief The macro below is used to define the number of target per zone sent
 * through I2C. This value can be changed by user, in order to tune I2C
 * transaction, and also the total memory size (a lower number of target per
 * zone means a lower RAM). The value must be between 1 and 4.
 */

#ifndef CONFIG_VL53L5CX_NB_TARGET_PER_ZONE
#define VL53L5CX_NB_TARGET_PER_ZONE		1U
#else
#define VL53L5CX_NB_TARGET_PER_ZONE		CONFIG_VL53L5CX_NB_TARGET_PER_ZONE
#endif

/*
 * @brief The macro below can be used to avoid data conversion into the driver.
 * By default there is a conversion between firmware and user data. Using this macro
 * allows to use the firmware format instead of user format. The firmware format allows
 * an increased precision.
 */

// #define 	VL53L5CX_USE_RAW_FORMAT

/*
 * @brief All macro below are used to configure the sensor output. User can
 * define some macros if he wants to disable selected output, in order to reduce
 * I2C access.
 */

#if CONFIG_VL53L5CX_ENABLE_AMBIENT_PER_SPAD == 0
#define VL53L5CX_DISABLE_AMBIENT_PER_SPAD
#endif
#if CONFIG_VL53L5CX_ENABLE_NB_SPADS_ENABLED == 0
#define VL53L5CX_DISABLE_NB_SPADS_ENABLED
#endif
#if CONFIG_VL53L5CX_ENABLE_NB_TARGET_DETECTED == 0
#define VL53L5CX_DISABLE_NB_TARGET_DETECTED
#endif
#if CONFIG_VL53L5CX_ENABLE_SIGNAL_PER_SPAD == 0
#define VL53L5CX_DISABLE_SIGNAL_PER_SPAD
#endif
#if CONFIG_VL53L5CX_ENABLE_RANGE_SIGMA_MM == 0
#define VL53L5CX_DISABLE_RANGE_SIGMA_MM
#endif
#if CONFIG_VL53L5CX_ENABLE_DISTANCE_MM == 0
#define VL53L5CX_DISABLE_DISTANCE_MM
#endif
#if CONFIG_VL53L5CX_ENABLE_REFLECTANCE_PERCENT == 0
#define VL53L5CX_DISABLE_REFLECTANCE_PERCENT
#endif
#if CONFIG_VL53L5CX_ENABLE_TARGET_STATUS == 0
#define VL53L5CX_DISABLE_TARGET_STATUS
#endif
#if CONFIG_VL53L5CX_ENABLE_MOTION_INDICATOR == 0
#define VL53L5CX_DISABLE_MOTION_INDICATOR
#endif

/**
 * @param (VL53L5CX_Platform*) p_platform : Pointer of VL53L5CX platform
 * structure.
 * @param (uint16_t) Address : I2C location of value to read.
 * @param (uint8_t) *p_values : Pointer of value to read.
 * @return (uint8_t) status : 0 if OK
 */

uint8_t VL53L5CX_RdByte(
		VL53L5CX_Platform *p_platform,
		uint16_t RegisterAdress,
		uint8_t *p_value);

/**
 * @brief Mandatory function used to write one single byte.
 * @param (VL53L5CX_Platform*) p_platform : Pointer of VL53L5CX platform
 * structure.
 * @param (uint16_t) Address : I2C location of value to read.
 * @param (uint8_t) value : Pointer of value to write.
 * @return (uint8_t) status : 0 if OK
 */

uint8_t VL53L5CX_WrByte(
		VL53L5CX_Platform *p_platform,
		uint16_t RegisterAdress,
		uint8_t value);

/**
 * @brief Mandatory function used to read multiples bytes.
 * @param (VL53L5CX_Platform*) p_platform : Pointer of VL53L5CX platform
 * structure.
 * @param (uint16_t) Address : I2C location of values to read.
 * @param (uint8_t) *p_values : Buffer of bytes to read.
 * @param (uint32_t) size : Size of *p_values buffer.
 * @return (uint8_t) status : 0 if OK
 */

uint8_t VL53L5CX_RdMulti(
		VL53L5CX_Platform *p_platform,
		uint16_t RegisterAdress,
		uint8_t *p_values,
		uint32_t size);

/**
 * @brief Mandatory function used to write multiples bytes.
 * @param (VL53L5CX_Platform*) p_platform : Pointer of VL53L5CX platform
 * structure.
 * @param (uint16_t) Address : I2C location of values to write.
 * @param (uint8_t) *p_values : Buffer of bytes to write.
 * @param (uint32_t) size : Size of *p_values buffer.
 * @return (uint8_t) status : 0 if OK
 */

uint8_t VL53L5CX_WrMulti(
		VL53L5CX_Platform *p_platform,
		uint16_t RegisterAdress,
		uint8_t *p_values,
		uint32_t size);

/**
 * @brief Optional function, only used to perform an hardware reset of the
 * sensor. This function is not used in the API, but it can be used by the host.
 * This function is not mandatory to fill if user don't want to reset the
 * sensor.
 * @param (VL53L5CX_Platform*) p_platform : Pointer of VL53L5CX platform
 * structure.
 * @return (uint8_t) status : 0 if OK
 */

uint8_t VL53L5CX_Reset_Sensor(
		VL53L5CX_Platform *p_platform);

/**
 * @brief Mandatory function, used to swap a buffer. The buffer size is always a
 * multiple of 4 (4, 8, 12, 16, ...).
 * @param (uint8_t*) buffer : Buffer to swap, generally uint32_t
 * @param (uint16_t) size : Buffer size to swap
 */

void VL53L5CX_SwapBuffer(
		uint8_t 		*buffer,
		uint16_t 	 	 size);
/**
 * @brief Mandatory function, used to wait during an amount of time. It must be
 * filled as it's used into the API.
 * @param (VL53L5CX_Platform*) p_platform : Pointer of VL53L5CX platform
 * structure.
 * @param (uint32_t) TimeMs : Time to wait in ms.
 * @return (uint8_t) status : 0 if wait is finished.
 */

uint8_t VL53L5CX_WaitMs(
		VL53L5CX_Platform *p_platform,
		uint32_t TimeMs);


#ifdef __cplusplus
}
#endif

#endif	// _PLATFORM_H_

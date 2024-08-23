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


#include "vl53l5cx_platform.h"

#include <freertos/FreeRTOS.h>
#include <esp_err.h>
#include <esp_check.h>
#include <esp_log.h>
#include <driver/i2c_master.h>

static const char TAG[] = "vl53l5cx";

#define VL53L5CX_STACK_MAX 		1024
#define VL53L5CX_MEM_ALLOC_CAPS MALLOC_CAP_DEFAULT

#ifdef NDEBUG
#define VL53L5CX_RETURN_ON_ERROR(x, format, ...) do { 	\
		esp_err_t err_rc_ = (x);						\
		if (unlikely(err_rc_ != ESP_OK)) {				\
			return 1;									\
		}												\
	} while (0)
#else
#define VL53L5CX_RETURN_ON_ERROR(x, format, ...) do { 	\
		esp_err_t err_rc_ = (x);						\
		if (unlikely(err_rc_ != ESP_OK)) {				\
			ESP_LOGE(TAG, "%s(%d): " format, __FUNCTION__, __LINE__, ##__VA_ARGS__);        \
			return 1;									\
		}												\
	} while (0)
#endif

static inline void _fill_write_buffer(uint8_t *buffer, uint16_t RegisterAdress, const uint8_t *p_value, size_t size)
{
	buffer[0] = RegisterAdress>>8;
	buffer[1] = RegisterAdress & 0xFF;
	memcpy(buffer+sizeof(RegisterAdress), p_value, size);
}


uint8_t VL53L5CX_RdByte(
		VL53L5CX_Platform *p_platform,
		uint16_t RegisterAdress,
		uint8_t *p_value)
{
	return VL53L5CX_RdMulti(p_platform, RegisterAdress, p_value, 1);
}

uint8_t VL53L5CX_WrByte(
		VL53L5CX_Platform *p_platform,
		uint16_t RegisterAdress,
		uint8_t value)
{
	return VL53L5CX_WrMulti(p_platform, RegisterAdress, &value, 1);
}

uint8_t VL53L5CX_WrMulti(
		VL53L5CX_Platform *p_platform,
		uint16_t RegisterAdress,
		uint8_t *p_values,
		uint32_t size)
{
	esp_err_t ret;
	size_t buf_sz = sizeof(RegisterAdress)+size;

	ESP_LOGV(TAG, "VL53L5CX_WrMulti >%04x  %p sz=%lu", RegisterAdress, p_values, size);

	if (buf_sz <= VL53L5CX_STACK_MAX) {
		// Allocate small writes on the stack
		uint8_t buf[buf_sz];

		_fill_write_buffer(buf, RegisterAdress, p_values, size);
		ret = i2c_master_transmit(p_platform->handle, buf, buf_sz, VL53L5CX_I2C_TIMEOUT_MS);
	}
	else {
		// Use heap_caps_calloc for larger buffers
	    uint8_t *buf = heap_caps_calloc(1, buf_sz, VL53L5CX_MEM_ALLOC_CAPS);
    	ESP_RETURN_ON_FALSE(buf, 1, TAG, "Write buffer allocation failed");

		_fill_write_buffer(buf, RegisterAdress, p_values, size);

		// Use longer timeout for big writes
		int timeout = VL53L5CX_I2C_TIMEOUT_MS + (size*1000U/VL53L5CX_MIN_SCL_SPEED_HZ);

		ret = i2c_master_transmit(p_platform->handle, buf, buf_sz, timeout);

		heap_caps_free(buf);
	}
	VL53L5CX_RETURN_ON_ERROR(ret, "transmit failed with error %d", ret);

	return 0;
}

uint8_t VL53L5CX_RdMulti(
		VL53L5CX_Platform *p_platform,
		uint16_t RegisterAdress,
		uint8_t *p_values,
		uint32_t size)
{
	esp_err_t ret;
	uint8_t buffer[2];

	ESP_LOGV(TAG, "VL53L5CX_RdMulti <%04x  %p sz=%lu", RegisterAdress, p_values, size);

	buffer[0] = RegisterAdress>>8;
	buffer[1] = RegisterAdress & 0xFF;

	ret = i2c_master_transmit_receive(p_platform->handle, buffer, sizeof(buffer), p_values, size, VL53L5CX_I2C_TIMEOUT_MS);
	if (ret<0)
		return -ret;
	return ret;
}

uint8_t VL53L5CX_Reset_Sensor(
		VL53L5CX_Platform *p_platform)
{
	uint8_t status = 0;
	
	/* (Optional) Need to be implemented by customer. This function returns 0 if OK */
	
	/* Set pin LPN to LOW */
	/* Set pin AVDD to LOW */
	/* Set pin VDDIO  to LOW */
	VL53L5CX_WaitMs(p_platform, 100);

	/* Set pin LPN of to HIGH */
	/* Set pin AVDD of to HIGH */
	/* Set pin VDDIO of  to HIGH */
	VL53L5CX_WaitMs(p_platform, 100);

	return status;
}

void VL53L5CX_SwapBuffer(
		uint8_t 		*buffer,
		uint16_t 	 	 size)
{
	uint32_t i, tmp;
	
	/* Example of possible implementation using <string.h> */
	for(i = 0; i < size; i = i + 4) 
	{
		tmp = (
		  buffer[i]<<24)
		|(buffer[i+1]<<16)
		|(buffer[i+2]<<8)
		|(buffer[i+3]);
		
		memcpy(&(buffer[i]), &tmp, 4);
	}
}	

uint8_t VL53L5CX_WaitMs(
		VL53L5CX_Platform *p_platform,
		uint32_t TimeMs)
{
	vTaskDelay(pdMS_TO_TICKS(TimeMs));
	return 0;
}

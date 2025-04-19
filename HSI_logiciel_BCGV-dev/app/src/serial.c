/**
 * \file serial.c
 * \brief
 * \author Raphael CAUSSE
 */

/***** Includes **************************************************************/

#include "serial.h"

/***** Extern Variables ******************************************************/

serial_frame_t serial_buffer_read[DRV_MAX_FRAMES] = {0};
serial_frame_t serial_buffer_write[DRV_MAX_FRAMES] = {0};
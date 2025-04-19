/**
 * \file serial.h
 * \brief
 * \author Raphael CAUSSE
 */

#ifndef SERIAL_H
#define SERIAL_H

/***** Includes **************************************************************/

#include "drv_api.h"

/***** Extern Variables ******************************************************/

extern serial_frame_t serial_buffer_read[DRV_MAX_FRAMES];
extern serial_frame_t serial_buffer_write[DRV_MAX_FRAMES];

#endif /* SERIAL_H */
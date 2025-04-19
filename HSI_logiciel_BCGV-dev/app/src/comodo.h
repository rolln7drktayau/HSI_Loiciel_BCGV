/**
 * \file comodo.h
 * \brief Interface of COMODO system.
 * \details Read and write serial frames OF COMODO from driver, decode COMODO frames.
 * \author Raphael CAUSSE - Melvyn MUNOZ - Roland Cedric TAYO
 */

#ifndef COMODO_H
#define COMODO_H

/***** Includes **************************************************************/

#include "bcgv_api.h"

/***** Functions *************************************************************/

bool comodo_read_frame_500ms(int32_t drv_fd);

/**
 * \brief Decode the COMODO serial frame and update application data.
 * \return bool true if the frame was successfully decoded, false otherwise.
 */
bool comodo_decode_frame(void);

/**
 * \brief Print decoded data extracted from COMODO serial frame.
 */
void comodo_print_decoded(void);

#endif /* COMODO_H */

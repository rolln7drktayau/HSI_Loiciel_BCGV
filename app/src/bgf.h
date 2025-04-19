/**
 * \file bgf.h
 * \brief Interface of BGF system.
 * \details Read and write serial frames of BGF from driver.
 * \author Melvyn MUNOZ
 */

#ifndef BGF_H
#define BGF_H

/***** Includes **************************************************************/

#include "bcgv_api.h"

/***** Functions *************************************************************/

/**
 * \brief Read all available serial frames for BGF.
 * \param drv_fd : Driver file descriptor.
 * \return int32_t : Number of serial read, or -1 if an error occured
 */
int32_t bgf_read_frames(int32_t drv_fd);

/**
 * \brief Write all necessary messages as serial frames.
 * \param drv_fd : Driver file descriptor.
 * \return int32_t : Number of errors while writing
 */
int32_t bgf_write_frames(int32_t drv_fd);

#endif /* BGF_H */

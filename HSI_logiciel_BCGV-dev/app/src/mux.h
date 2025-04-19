/**
 * \file mux.h
 * \brief Interface of MUX system.
 * \details Read UDP frames from driver, write UDP frames to driver, decode and encode MUX frames.
 * \author Raphael CAUSSE - Melvyn MUNOZ - Roland Cedric TAYO
 */

#ifndef MUX_H
#define MUX_H

/***** Includes **************************************************************/

#include "drv_api.h"
#include "bcgv_api.h"

/***** Functions *************************************************************/

/**
 * \brief Read MUX 100ms UDP frame from driver (blocking call).
 * \details Wait for next UDP 100ms frame and returns it.
 * \param drv_fd : Driver file descriptor
 * \return bool : true if successfully read from driver, false otherwise
 */
bool mux_read_frame_100ms(int32_t drv_fd);

/**
 * \brief Write MUX 200ms UDP frame to driver.
 * \param drv_fd : Driver file descriptor
 * \return bool : true if successfully write from driver, false otherwise
 */
bool mux_write_frame_200ms(int32_t drv_fd);

/**
 * \brief Check for a valid frame number in the frame.
 * \details If there the frame number is not equal to the expected frame number (incremental and looping at 100), a warning is logged.
 */
void mux_check_frame_number(void);

/**
 * \brief Increment current frame number for next frame checks.
 */
void mux_incr_frame_number(void);

/**
 * \brief Decode MUX 100ms UDP frame and update application data.
 * \details Frame format (15 bytes, Big endian):
 *          - 1 byte (MSB): Frame number (1-100)
 *          - 4 bytes : Distance (km)
 *          - 1 byte : Speed (km/h)
 *          - 1 byte : Chassis issues
 *          - 1 byte : Motor issues
 *          - 1 byte : Fuel level (litres)
 *          - 4 bytes: Engine RPM
 *          - 1 byte (LSB): Battery issues
 *          - 1 byte : CRC8
 * \return bool : true if the frame CRC8 is valid and frame has been decoded, false otherwise
 */
bool mux_decode_frame_100ms(void);

/**
 * \brief Encode UDP 200ms MUX frame with application.
 */
void mux_encode_frame_200ms(void);

/**
 * \brief Print raw bytes of a MUX frame.
 * \param frame : Pointer to the MUX frame buffer
 * \param length : Number of bytes in MUX frame
 */
void mux_print_raw(const uint8_t *frame, const size_t length);

/**
 * \brief Print decoded data extracted from MUX 100ms UDP frame.
 */
void mux_print_decoded(void);

#endif /* MUX_H */
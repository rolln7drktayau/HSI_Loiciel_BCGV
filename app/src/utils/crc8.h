/**
 * \file crc8.h
 * \brief CRC8 checksum computation using a lookup table.
 * \author Raphael CAUSSE
 */

#ifndef CRC8_H
#define CRC8_H

/***** Includes **************************************************************/

#include <stdint.h>
#include <stddef.h>

/***** Definitions ***********************************************************/

#define CRC8_INIT (0x00)

/***** Functions *************************************************************/

/**
 * \brief Compute CRC8 checksum for a given data buffer.
 * \param data : Pointer to the input data buffer.
 * \param length : Number of bytes in the data buffer.
 * \return uint8_t : The computed CRC8 checksum.
 */
uint8_t crc8_compute(const uint8_t *data, size_t length);

/**
 * \brief Update an existing CRC8 checksum with a new byte.
 * \param current_crc : Current CRC8 checksum value.
 * \param new_byte : New byte to include in the checksum.
 * \return uint8_t : Updated CRC8 checksum.
 */
uint8_t crc8_update(uint8_t current_crc, uint8_t new_byte);

#endif /* CRC8_H */
/**
 * \file bit_utils.h
 * \brief Bit manipulation utilities.
 * \author Raphael CAUSSE
 */

#ifndef BIT_UTILS_H
#define BIT_UTILS_H

/***** Definitions ***********************************************************/

#define XOR ^

/***** Macros ****************************************************************/

#define GET_BIT(x, bit) (((x) >> (bit)) & 1)
#define CLEAR_BIT(x, bit) (x &= ~bit)

#endif /* BIT_UTILS_H */
/**
 * \file comodo.c
 * \brief Implementation of COMODO system.
 * \details Read and write serial frames OF COMODO from driver, decode COMODO frames.
 * \author Raphael CAUSSE - Melvyn MUNOZ - Roland Cedric TAYO
 */

/***** Includes **************************************************************/

#include "comodo.h"
#include "serial.h"
#include "bit_utils.h"
#include "log.h"

/***** Definitions ***********************************************************/

#define COMODO_SERIAL_CHANNEL (12 - 1) /* Serial channel 12 */
#define COMODO_SERIAL_FRAME_SIZE (1)   /* byte */

/***** Static Variables ******************************************************/

static uint8_t comodo_frame = 0;

/***** Functions *************************************************************/

bool comodo_read_frame_500ms(int32_t drv_fd)
{
    int32_t ret = 0;
    uint32_t serial_data_len = 0;

    ret = drv_read_ser(drv_fd, serial_buffer_read, &serial_data_len);
    if (ret == DRV_ERROR)
    {
        log_error("error while reading from driver", NULL);
        return false;
    }

    if ((serial_data_len > 0) && (serial_buffer_read[COMODO_SERIAL_CHANNEL].frameSize == COMODO_SERIAL_FRAME_SIZE))
    {
        comodo_frame = serial_buffer_read[COMODO_SERIAL_CHANNEL].frame[0];
    }

#ifdef DEBUG
    printf("==================== COMODO READ ===================\n");
    printf("COMODO [ %02X ]\n", comodo_frame);
    printf("====================================================\n");
#endif

    return true;
}

bool comodo_decode_frame(void)
{
    /* Extract data from frame */
    cmd_t cmd_indic_hazard = GET_BIT(comodo_frame, 7);
    cmd_t cmd_position_light = GET_BIT(comodo_frame, 6);
    cmd_t cmd_crossing_light = GET_BIT(comodo_frame, 5);
    cmd_t cmd_highbeam_light = GET_BIT(comodo_frame, 4);
    cmd_t cmd_indic_right = GET_BIT(comodo_frame, 3);
    cmd_t cmd_indif_left = GET_BIT(comodo_frame, 2);
    cmd_t cmd_wiper = GET_BIT(comodo_frame, 1);
    cmd_t cmd_washer = GET_BIT(comodo_frame, 0);

    /* Store data in app context */
    set_cmd_indic_hazard(cmd_indic_hazard);
    set_cmd_position_light(cmd_position_light);
    set_cmd_crossing_light(cmd_crossing_light);
    set_cmd_highbeam_light(cmd_highbeam_light);
    set_cmd_indic_right(cmd_indic_right);
    set_cmd_indic_left(cmd_indif_left);
    set_cmd_wiper(cmd_wiper);
    set_cmd_washer(cmd_washer);

#ifdef DEBUG
    printf("=================== COMODO DECODE ==================\n");
    comodo_print_decoded();
    printf("====================================================\n");
#endif

    return true;
}

void comodo_print_decoded(void)
{
    cmd_t cmd_indic_hazard = get_cmd_indic_hazard();
    cmd_t cmd_position_light = get_cmd_position_light();
    cmd_t cmd_crossing_light = get_cmd_crossing_light();
    cmd_t cmd_highbeam_light = get_cmd_highbeam_light();
    cmd_t cmd_indic_right = get_cmd_indic_right();
    cmd_t cmd_indif_left = get_cmd_indic_left();
    cmd_t cmd_wiper = get_cmd_wiper();
    cmd_t cmd_washer = get_cmd_washer();

    printf("Indic hazard: %u\n", cmd_indic_hazard);
    printf("Position light: %u\n", cmd_position_light);
    printf("Crossing light: %u\n", cmd_crossing_light);
    printf("Highbeam light: %u\n", cmd_highbeam_light);
    printf("Indic right: %u\n", cmd_indic_right);
    printf("Indif left: %u\n", cmd_indif_left);
    printf("Wiper: %u\n", cmd_wiper);
    printf("Washer: %u\n", cmd_washer);
}
/**
 * \file app.c
 * \brief Entry point of the application.
 * \author Raphael CAUSSE - Melvyn MUNOZ - Roland Cedric TAYO
 */

/***** Includes **************************************************************/

#include "drv_api.h"
#include "bcgv_api.h"
#include "mux.h"
#include "bgf.h"
#include "comodo.h"
#include "log.h"
#include "fsm_lights.h"
#include "fsm_indicators.h"
#include "fsm_windshield_washer.h"

/***** Main function *********************************************************/

int main(void)
{
    bool quit = false;
    bool success = false;
    int32_t ret = 0;
    int32_t driver_fd = 0;

    /***** Starting application *****/

    driver_fd = drv_open();
    if (driver_fd == DRV_ERROR)
    {
        log_error("error while opening driver", NULL);
        return EXIT_FAILURE;
    }
    else if (driver_fd == DRV_VER_MISMATCH)
    {
        log_error("driver version mismatch", NULL);
        return EXIT_FAILURE;
    }
    log_info("driver opened", NULL);

    bcgv_ctx_init();

    /***** Main loop *****/

    (void)success;
    while (quit == false)
    {
        /* Receive and decode MUX frame (UDP) */
        success = mux_read_frame_100ms(driver_fd);
        mux_check_frame_number();
        success = mux_decode_frame_100ms();

        /* Receive serial frames */
        ret = bgf_read_frames(driver_fd);
        success = comodo_read_frame_500ms(driver_fd);

        /* Decode COMODO frame (serial) */
        success = comodo_decode_frame();

        /* FSM executions */
        ret = fsm_lights_run();
        ret = fsm_indicators_run();
        ret = fsm_windshield_washer_run();

        /* Encode and send MUX frame (UDP) */
        mux_encode_frame_200ms();
        success = mux_write_frame_200ms(driver_fd);

        /* Encode and write serial frames */
        ret = bgf_write_frames(driver_fd);

        /* Prepare next MUX frame number check */
        mux_incr_frame_number();
    }

    /***** Closing application *****/

    ret = drv_close(driver_fd);
    if (ret == DRV_ERROR)
    {
        log_error("error while closing driver", NULL);
        return EXIT_FAILURE;
    }
    log_info("driver closed", NULL);

    return EXIT_SUCCESS;
}

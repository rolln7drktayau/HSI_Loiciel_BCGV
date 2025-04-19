/**
 * \file log.h
 * \brief Implementation of logging module.
 * \author Raphael CAUSSE
 */

/***** Includes **************************************************************/

#include <stdarg.h>
#include <time.h>
#include "log.h"

/***** Functions *************************************************************/

void log_write(const char *level, const char *func, int line, const char *fmt, ...)
{
    /* Get the current time */
    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);

    /* Format the time */
    char time_buf[20];
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", local_time);

    /* Write the log message */
    fprintf(stdout, "[%s] [%s] %s:%d: ", time_buf, level, func, line);

    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    va_end(args);

    fprintf(stdout, "\n");
    fflush(stdout);
}
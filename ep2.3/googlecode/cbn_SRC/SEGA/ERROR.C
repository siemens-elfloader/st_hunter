
#include "osd.h"

int error_log;

struct {
    int enabled;
    int verbose;
//    FILE *log;
} t_error;

void error_init(void)
{
//    error_log = fopen("error.log","w");
}

void error_shutdown(void)
{
//    if(error_log) fclose(error_log);
}

void error(char *format, ...)
{
//    va_list ap;
//    va_start(ap, format);
//    if(error_log) vfprintf(error_log, format, ap);
//    va_end(ap);
}


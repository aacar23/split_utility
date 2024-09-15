#include <time.h>

#include "time_to_str.h"

char *time_to_str(void)
{
    static char time_str[MAX_TIME_STR_LENGTH + 1lu] = {0};

    strftime(time_str, MAX_TIME_STR_LENGTH, "%d %B %Y %H:%M:%S", localtime(&(time_t){time(NULL)}));

    return time_str;
}

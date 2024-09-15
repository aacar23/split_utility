#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "split2.h"
#include "split.h"
#include "error_exit.h"
#include "time_to_str.h"

char **split2(const char *str, const char *delim)
{
    char *str_duplicate = strdup(str);

    const char * const * result = (const char * const *)split(str_duplicate, delim);

    free(str_duplicate);

    return (char **)result;
}

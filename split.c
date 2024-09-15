#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "split.h"
#include "error_exit.h"
#include "time_to_str.h"

typedef struct dynamic_char_ptr_array{
    char **contents;
    size_t size;
    size_t capacity;
}dynamic_char_ptr_array;

#define DEFAULT_CAPACITY (10lu)

static dynamic_char_ptr_array *create_dynamic_char_ptr_array_empty(void)
{

    const char *faulty_call = "malloc";

    dynamic_char_ptr_array *new = malloc(sizeof(dynamic_char_ptr_array));
    if (!new)
        goto FAIL_1;

    if (!(new -> contents = calloc((new -> capacity = DEFAULT_CAPACITY) + 1, sizeof(char *))))
        goto FAIL_2;

    new -> size = 0lu;

    return new;

    FAIL_2:
    faulty_call = "calloc";
    free(new);
    FAIL_1:
    error_exit("%s:%s:%s:%s", __func__, faulty_call, strerror(errno), time_to_str());

    return NULL;
}

static void expand_dynamic_char_ptr_array(dynamic_char_ptr_array *obj)
{
    if (!(obj -> contents = realloc(obj -> contents, ((obj -> capacity *= 2lu) + 1lu) * sizeof(char *))))
        error_exit("%s:%s:%s:%s", __func__, "realloc", strerror(errno), time_to_str());

    memset(obj -> contents + obj -> capacity / 2lu, 0, obj -> capacity / 2lu + 1lu);
}

static void append_ptr_to_dynamic_char_ptr_array(dynamic_char_ptr_array *obj, const char *ptr)
{
    if (obj -> size + 1lu == obj -> capacity)
        expand_dynamic_char_ptr_array(obj);

    *(obj -> contents + obj -> size++) = (char *)ptr;
}

static void append_ptr_content_copy_dynamic_char_ptr_array(dynamic_char_ptr_array *obj, const char *ptr)
{
    const char *str_duplicate = strdup(ptr);
    if (!str_duplicate)
        error_exit("%s:%s:%s:%s", __func__, "strdup", strerror(errno), time_to_str());

    append_ptr_to_dynamic_char_ptr_array(obj, str_duplicate);
}

static void fill_dynamic_char_ptr_array_with_token_copies(dynamic_char_ptr_array *obj, char *str, const char *delim)
{
    for (const char *current_token = strtok(str, delim);current_token;current_token = strtok(NULL, delim))
        append_ptr_content_copy_dynamic_char_ptr_array(obj, current_token);
}


char **split(char *str, const char *delim)
{
    dynamic_char_ptr_array *tokens = create_dynamic_char_ptr_array_empty();
    fill_dynamic_char_ptr_array_with_token_copies(tokens, str, delim);

    const char * const *result = (const char * const *)tokens -> contents;
    free(tokens);

    return (char **)result;
}

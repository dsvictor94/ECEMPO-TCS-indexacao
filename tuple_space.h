#ifndef TUPLE_SPACE_H   // guardas de cabeçalho, impedem inclusões cíclicas
#define TUPLE_SPACE_H

typedef struct _tuple_space_t {
    const char *path;
} tuple_space_t;

int tuple_space_init(tuple_space_t *tuple_space, const char *path);
int tuple_space_destroy(tuple_space_t *tuple_space);

// tuple : (a, b, ?, d) -> "a/b/*/d"
int tuple_space_take(tuple_space_t *tuple_space, const char *tuple);
int tuple_space_read(tuple_space_t *tuple_space, const char *tuple);
int tuple_space_write(tuple_space_t *tuple_space, const char *tuple);
int tuple_space_release(int fd);

#endif
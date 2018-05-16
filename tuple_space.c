#include <stddef.h>
#include <stdio.h>
#include <glob.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include <sys/stat.h>
#include <sys/file.h>

#include "tuple_space.h"

int is_regular_file(const char *path) {
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

int has_no_link(int fd) {
    struct stat path_stat;
    fstat(fd, &path_stat);
    return path_stat.st_nlink == 0;
}

int tuple_space_init(tuple_space_t *tuple_space, const char *path) {
    tuple_space -> path = path;
}

int tuple_space_destroy(tuple_space_t *tuple_space) {
    tuple_space -> path = NULL;
}

int tuple_space_take(tuple_space_t *tuple_space, const char *tuple) {
    glob_t globbuf;
    int r, i, j, seed;
    int fd;
    size_t pattern_size = strlen(tuple_space->path) + strlen(tuple) + 2;
    char *pattern = malloc(pattern_size);
    strcat(pattern, tuple_space->path);
    strcat(pattern, "/");
    strcat(pattern, tuple);
    
    r = GLOB_NOMATCH;
    srand(time(NULL));
    while (1) {
        r = glob(pattern, 0, NULL, &globbuf);
        if (r == GLOB_NOMATCH) goto retry;
        if (r != 0) goto endfail;

        seed = rand();
        for (int i=0; i <globbuf.gl_pathc; i++) { 
            j = (i + seed) % globbuf.gl_pathc;
            if (is_regular_file(globbuf.gl_pathv[j])) {
                fd = open(globbuf.gl_pathv[j], O_RDONLY);
                if (fd == -1) continue;

                if (flock(fd, LOCK_EX | LOCK_NB) == 0 && !has_no_link(fd)) {
                    if (remove(globbuf.gl_pathv[j]) != 0) goto endfail;
                    goto endok;
                }
            }
        }

        retry:
        usleep(50000);
    }

    endok:
    printf("DEBUG %s \n", globbuf.gl_pathv[j]);
    globfree(&globbuf);
    return fd;
    endfail:
    globfree(&globbuf);
    return -1;
}


int tuple_space_read(tuple_space_t *tuple_space, const char *tuple) {
    glob_t globbuf;
    int r, i, j, seed;
    int fd;
    size_t pattern_size = strlen(tuple_space->path) + strlen(tuple) + 2;
    char *pattern = malloc(pattern_size);
    strcat(pattern, tuple_space->path);
    strcat(pattern, "/");
    strcat(pattern, tuple);
    
    r = GLOB_NOMATCH;
    srand(time(NULL));
    while (1) {
        r = glob(pattern, 0, NULL, &globbuf);
        if (r == GLOB_NOMATCH) goto retry;
        if (r != 0) goto endfail;

        seed = rand();
        for (int i=0; i <globbuf.gl_pathc; i++) { 
            j = (i + seed) % globbuf.gl_pathc;
            if (is_regular_file(globbuf.gl_pathv[j])) {
                fd = open(globbuf.gl_pathv[j], O_RDONLY);
                if (fd == -1) continue;

                if (flock(fd, LOCK_EX | LOCK_NB) == 0  && !has_no_link(fd)) {
                    goto endok;
                }
            }
        }

        retry:
        usleep(50000);
    }

    endok:
    printf("DEBUG %s \n", globbuf.gl_pathv[j]);
    globfree(&globbuf);
    return fd;
    endfail:
    globfree(&globbuf);
    return -1;
}

int tuple_space_write(tuple_space_t *tuple_space, const char *tuple) {
    int fd;
    size_t pattern_size = strlen(tuple_space->path) + strlen(tuple) + 2;
    char *pattern = malloc(pattern_size);
    strcat(pattern, tuple_space->path);
    strcat(pattern, "/");
    strcat(pattern, tuple);
    
    while (1) {
        fd = open(pattern, O_RDWR | O_CREAT, 0664);
        if (fd == -1) goto endfail;
        if (flock(fd, LOCK_EX) == 0) {
            if (!has_no_link(fd)){ 
                goto endok;
            }
            close(fd);
        }
    }

    endok:
    printf("DEBUG %s \n", pattern);
    return fd;
    endfail:
    return -1;
}

int tuple_space_release(int fd) {
    close(fd);
}

tuple_space_t ts;

int main(int argc, char const *argv[]){
    tuple_space_init(&ts, "/tmp/ts");

    int fd = tuple_space_write(&ts, "a/b");

    if (fd != -1) printf("Teste %i \n", fd);

    sleep(20);

    tuple_space_destroy(&ts);
    return 0;
}

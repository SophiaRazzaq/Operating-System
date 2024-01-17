#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <ctype.h>
#include <pthread.h>

#define FILE_SIZE 100

void replaceIntegers(char *start, char *end)
 {
    for (char *ptr = start; ptr < end; ++ptr) 
    {
        if (isdigit(*ptr))
        {
            *ptr = ' ';
        }
    }
}

void *threadFunction(void *arg) 
{
    char *start = (char *)arg;
    char *end = start + FILE_SIZE / 2;
    replaceIntegers(start, end);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("wrong input");
        return 1;
    }

    const char *file_name = argv[1];

    int fd = open(file_name, O_RDWR);
    if (fd == -1) {
        printf("failed to open the file\n");
        return 1;
    }

    char *file_data = (char *)mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (file_data == MAP_FAILED) {
        printf("failed to create a memory map\n");
        close(fd);
        return 1;
    }

    close(fd);

    pthread_t thread;
    pthread_create(&thread, NULL, threadFunction, file_data + FILE_SIZE / 2);

    replaceIntegers(file_data, file_data + FILE_SIZE / 2);

    pthread_join(thread, NULL);

    munmap(file_data, FILE_SIZE);

    return 0;
}

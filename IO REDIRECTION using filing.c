#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {


    char buffer[4096];
    ssize_t bytes_read;   
    int fd1;
    
    if (argc != 2) {
        printf("error in the input :(");
        return 1;
    }


    fd1= open("input.txt", O_WRONLY | O_CREAT);
    if (fd1 == -1) {
        perror("Error opening input file");
        return 1;
    }
    


    int fd2 = open("output.txt", O_WRONLY | O_CREAT);
    if (fd2 == -1) {
        perror("Error in the output file craetion :(");
        return 1;
    }
    

   dup2(fd1, fd2);


    return 0;
}

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

void replaceWordInFile(const char *fileName, const char *wordToReplace, const char *replacementWord) {
    int fd = open(fileName, O_RDWR);
    if (fd == -1) {
        perror("open");
        return;
    }

    off_t fileSize = lseek(fd, 0, SEEK_END);
    if (fileSize == -1) {
        perror("lseek");
        close(fd);
        return;
    }

    char *map = (char *) mmap(NULL, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return;
    }

    char *wordPtr;
    while ((wordPtr = strstr(map, wordToReplace)) != nullptr) {
        std::memcpy(wordPtr, replacementWord, strlen(replacementWord));
    }

    if (munmap(map, fileSize) == -1) {
        perror("munmap");
    }

    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        std::cout << "wrong sequence, filename->word to be replaced with->word to replace" << std::endl;
        return 1;
    }

    const char *fileName = argv[1];
    const char *wordToReplace = argv[2];
    const char *replacementWord = argv[3];

    replaceWordInFile(fileName, wordToReplace, replacementWord);

    return 0;
}

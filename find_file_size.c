#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

double getTime() {
    struct timeval time;
    gettimeofday(&time, NULL);
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}

void measureReadTime(const char* filename, int block_size) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file for reading");
        exit(EXIT_FAILURE);
    }

    char* buffer = malloc(block_size);
    double start, end;

    start = getTime();
    while (read(fd, buffer, block_size) > 0) {
        // Continue reading until the end
    }
    end = getTime();

    free(buffer);
    close(fd);

    printf("Time taken to read the file with block size %d bytes: %.6f seconds\n", block_size, end - start);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <filename> <block_size>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *filename = argv[1];
    int block_size = atoi(argv[2]);

    measureReadTime(filename, block_size);

    return 0;
}

// Basic file read/write operations in C.
// Use: gcc -o basic_io basic_io.c
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void writeToFile(const char* filename, int block_size, int block_count) {
    int fd = open(filename, O_WRONLY | O_CREAT, 0666);
    if (fd == -1) {
        perror("Error opening file for writing");
        exit(EXIT_FAILURE);
    }

    char* buffer = malloc(block_size);
    memset(buffer, 'A', block_size); // Fill with 'A's

    for (int i = 0; i < block_count; ++i) {
        if (write(fd, buffer, block_size) == -1) {
            perror("Error writing to file");
            close(fd);
            exit(EXIT_FAILURE);
        }
    }
    printf("Successfully wrote %d blocks of size %d bytes to %s\n", block_count, block_size, filename);

    free(buffer);
    close(fd);
}

void readFromFile(const char* filename, int block_size) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file for reading");
        exit(EXIT_FAILURE);
    }

    char* buffer = malloc(block_size);
    if (buffer == NULL) {
        perror("Failed to allocate memory");
        close(fd);
        exit(EXIT_FAILURE);
    }

    int blocks_read = 0;
    ssize_t bytes_read;

    while ((bytes_read = read(fd, buffer, block_size)) > 0) {
        blocks_read++;
        // Optionally process the data read from the file
    }

    if (bytes_read == -1) {
        perror("Error occurred during file read");
        free(buffer);
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("Successfully read %d blocks of size %d bytes from %s\n", blocks_read, block_size, filename);

    free(buffer);
    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <filename> <mode: r/w> <block_size> <block_count>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *filename = argv[1];
    char mode = argv[2][0];
    int block_size = atoi(argv[3]);
    int block_count = atoi(argv[4]);

    if (mode == 'w') {
        writeToFile(filename, block_size, block_count);
    } else if (mode == 'r') {
        readFromFile(filename, block_size);
    } else {
        fprintf(stderr, "Invalid mode. Use 'r' for read or 'w' for write.\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}

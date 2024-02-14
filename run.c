#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Usage: %s <filename> [-r|-w] <block_size> <block_count>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    char mode = argv[2][1]; // assuming -r or -w
    int block_size = atoi(argv[3]);
    int block_count = atoi(argv[4]);
    int fd, i;
    char *buffer = malloc(block_size);

    if (mode == 'w') {
        // Write to file
        for (int j = 0; j < block_size; j++) {
        buffer[j] = 'A';
        }
        fd = open(filename, O_WRONLY | O_CREAT, 0666);
        if (fd < 0) {
            perror("Error opening file");
            return 1;
        }

        for (i = 0; i < block_count; i++) {
            if (write(fd, buffer, block_size) != block_size) {
                perror("Error writing to file");
                close(fd);
                return 1;
            }
        }
    } else if (mode == 'r') {
        // Read from file
        fd = open(filename, O_RDONLY);
        if (fd < 0) {
            perror("Error opening file");
            return 1;
        }

        for (i = 0; i < block_count; i++) {
            ssize_t bytes_read = read(fd, buffer, block_size);
            if (bytes_read < 0) {
                perror("Error reading from file");
                close(fd);
                return 1;
            }else if (bytes_read == 0) {
            // End of file reached
            break;
            }
            write(STDOUT_FILENO, buffer, bytes_read);
        }
    } else {
        printf("Invalid mode. Use -r for read or -w for write.\n");
        return 1;
    }

    close(fd);
    free(buffer);
    return 0;
}

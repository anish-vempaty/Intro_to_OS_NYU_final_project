#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>

double getTime() {
    struct timeval time;
    gettimeofday(&time, NULL);
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}

void performIO(const char* filename, int block_size, int block_count, char mode) {
    int fd;
    if (mode == 'w') {
        fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    } else {
        fd = open(filename, O_RDONLY);
    }
    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char* buffer = malloc(block_size);
    if (mode == 'w') {
        memset(buffer, 0, block_size);  // Fill with zeros for writing
    }

    double start, end;
    ssize_t io_result;
    int syscall_count = 0;

    start = getTime();
    for (int i = 0; i < block_count; ++i) {
        if (mode == 'w') {
            io_result = write(fd, buffer, block_size);
        } else {
            io_result = read(fd, buffer, block_size);
        }

        if (io_result <= 0) {
            break;
        }
        syscall_count++;
    }
    end = getTime();

    free(buffer);
    close(fd);

    double time_taken = end - start;
    double data_transferred = block_size * (double)syscall_count;
    double performance = (data_transferred / (1024 * 1024)) / time_taken;
    double syscalls_per_second = syscall_count / time_taken;

    printf("Time taken: %.6f seconds\n", time_taken);
    printf("Performance: %.2f MiB/s\n", performance);
    printf("System calls per second: %.2f\n", syscalls_per_second);
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

    if (mode != 'r' && mode != 'w') {
        fprintf(stderr, "Invalid mode. Use 'r' for read or 'w' for write.\n");
        exit(EXIT_FAILURE);
    }

    performIO(filename, block_size, block_count, mode);

    return 0;
}

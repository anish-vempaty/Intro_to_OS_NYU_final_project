#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <time.h>

#define BLOCK_SIZE 65536 // Adjust based on your system and testing
#define NUM_THREADS 30   // Adjust based on your system

typedef struct {
    char *filename;
    off_t start;
    off_t end;
    unsigned int xor_result;
} ThreadData;

void *thread_func(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    //printf("Thread starting: start=%lld, end=%lld\n", (long long)data->start, (long long)data->end);

    int fd = open(data->filename, O_RDONLY);
    if (fd < 0) {
        perror("Error opening file");
        pthread_exit(NULL);
    }

    lseek(fd, data->start, SEEK_SET);
    unsigned char buffer[BLOCK_SIZE];
    unsigned int xor_val = 0;
    ssize_t bytes_read;

    off_t total_bytes_to_read = data->end - data->start;
    off_t bytes_to_read = total_bytes_to_read;

    while ((bytes_read = read(fd, buffer, bytes_to_read > BLOCK_SIZE ? BLOCK_SIZE : bytes_to_read)) > 0) {
        for (ssize_t j = 0; j < bytes_read; ++j) {
            xor_val ^= buffer[j];
        }
        bytes_to_read -= bytes_read;
    }

    // If the file size was not a multiple of 4, pad with '0x00' bytes
    if (bytes_to_read > 0) {
        while (bytes_to_read % 4 != 0) {
            xor_val ^= 0x00; // XOR with zero has no effect, so this is for clarity.
            bytes_to_read++;
        }
    }

    if (bytes_read < 0) {
        perror("Error reading file");
    }

    data->xor_result = xor_val;
    //printf("Thread completed: XOR value = %08x\n", xor_val);

    close(fd);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    struct stat statbuf;
    if (stat(filename, &statbuf) == -1) {
        perror("Failed to get file size");
        return 1;
    }
    off_t file_size = statbuf.st_size;
    //printf("File size: %lld\n", (long long)file_size);

    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];

    off_t part_size = file_size / NUM_THREADS;
    off_t remaining = file_size % NUM_THREADS;
    off_t current_start = 0;

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start); // Start timing

    for (int i = 0; i < NUM_THREADS; ++i) {
        thread_data[i].filename = filename;
        thread_data[i].start = current_start;
        thread_data[i].end = current_start + part_size + (remaining > 0 ? 1 : 0);
        current_start = thread_data[i].end;
        remaining--;

        pthread_create(&threads[i], NULL, thread_func, &thread_data[i]);
    }

    unsigned int final_xor = 0;
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
        final_xor ^= thread_data[i].xor_result;
        //printf("Intermediate XOR after thread %d: %08x\n", i, final_xor);
    }
    clock_gettime(CLOCK_MONOTONIC, &end); // End timing
    double elapsed_time = end.tv_sec - start.tv_sec;
    elapsed_time += (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Final XOR value: %08x\n", final_xor);
    printf("Time taken: %.5f seconds\n", elapsed_time);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_BLOCKS 100

typedef struct {
    void *ptr;
    size_t size;
} MemBlock;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s '<delay>:<unit>:<+N+M-N...>'\n", argv[0]);
        return 1;
    }

    char *input = argv[1];
    char *delay_str = strtok(input, ":");
    char *unit_str = strtok(NULL, ":");
    char *seq_str = strtok(NULL, ":");

    if (!delay_str || !unit_str || !seq_str) {
        fprintf(stderr, "Invalid format. Expected format: delay:unit:+N+M-N...\n");
        return 1;
    }

    int delay = atoi(delay_str);
    size_t unit_size = 1;

    if (strcasecmp(unit_str, "MB") == 0) {
        unit_size = 1024 * 1024;
    } else if (strcasecmp(unit_str, "kB") == 0 || strcasecmp(unit_str, "KB") == 0) {
        unit_size = 1024;
    } else if (strcasecmp(unit_str, "B") == 0) {
        unit_size = 1;
    } else {
        fprintf(stderr, "Unsupported unit: %s\n", unit_str);
        return 1;
    }

    MemBlock blocks[MAX_BLOCKS] = {0};
    int block_count = 0;

    printf("Starting memory test with delay = %d sec, unit = %s\n", delay, unit_str);

    char *cursor = seq_str;
    while (*cursor) {
        if (*cursor == '+' || *cursor == '-') {
            char op = *cursor++;
            char num_buf[16] = {0};
            int i = 0;

            while (*cursor >= '0' && *cursor <= '9' && i < 15) {
                num_buf[i++] = *cursor++;
            }
            num_buf[i] = '\0';

            int amount = atoi(num_buf);
            if (amount <= 0) continue;

            if (op == '+') {
                if (block_count >= MAX_BLOCKS) {
                    fprintf(stderr, "Too many allocations\n");
                    break;
                }
                size_t bytes = amount * unit_size;
                void *mem = malloc(bytes);
                if (!mem) {
                    perror("malloc failed");
                    break;
                }
                memset(mem, 0xA5, bytes);
                blocks[block_count++] = (MemBlock){mem, bytes};
                printf("Allocated %d %s (%zu bytes)\n", amount, unit_str, bytes);
            } else if (op == '-') {
                size_t bytes = amount * unit_size;
                int freed = 0;
                for (int j = block_count - 1; j >= 0; --j) {
                    if (blocks[j].size == bytes && blocks[j].ptr) {
                        free(blocks[j].ptr);
                        blocks[j].ptr = NULL;
                        printf("Freed %d %s (%zu bytes)\n", amount, unit_str, bytes);
                        freed = 1;
                        break;
                    }
                }
                if (!freed) {
                    printf("No matching block to free: %d %s\n", amount, unit_str);
                }
            }

            sleep(delay);
        } else {
            cursor++;
        }
    }

    for (int i = 0; i < block_count; ++i) {
        if (blocks[i].ptr) {
            free(blocks[i].ptr);
        }
    }

    printf("Test complete.\n");
    return 0;
}

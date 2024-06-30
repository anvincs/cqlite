#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_BUFFER_SIZE 128

typedef struct {
  char* buffer;
  size_t buffer_length;
  ssize_t input_length;
} InputBuffer;

InputBuffer* new_input_buffer() {
  InputBuffer* input_buffer = malloc(sizeof(InputBuffer));
  input_buffer->buffer = NULL;
  input_buffer->buffer_length = 0;
  input_buffer->input_length = 0;

  return input_buffer;
}

void print_prompt() { printf("db > "); }

// void read_input(InputBuffer* input_buffer) {
//   ssize_t bytes_read =
//       getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

//   if (bytes_read <= 0) {
//     printf("Error reading input\n");
//     exit(EXIT_FAILURE);
//   }

//   // Ignore trailing newline
//   input_buffer->input_length = bytes_read - 1;
//   input_buffer->buffer[bytes_read - 1] = 0;
// }

void read_input(InputBuffer* input_buffer) {
    if (input_buffer->buffer == NULL) {
        input_buffer->buffer = (char *)malloc(INITIAL_BUFFER_SIZE);
        if (input_buffer->buffer == NULL) {
            printf("Error allocating memory\n");
            exit(EXIT_FAILURE);
        }
        input_buffer->buffer_length = INITIAL_BUFFER_SIZE;
    }

    size_t current_size = input_buffer->buffer_length;
    char *current_pos = input_buffer->buffer;
    size_t total_read = 0;

    while (1) {
        if (fgets(current_pos, current_size - total_read, stdin) == NULL) {
            if (total_read == 0) {
                printf("Error reading input\n");
                exit(EXIT_FAILURE);
            }
            break;
        }

        size_t len = strlen(current_pos);
        total_read += len;

        if (current_pos[len - 1] == '\n') {
            break;
        }

        if (total_read + 1 >= current_size) {
            current_size *= 2;
            char *new_buffer = (char *)realloc(input_buffer->buffer, current_size);
            if (new_buffer == NULL) {
                printf("Error reallocating memory\n");
                exit(EXIT_FAILURE);
            }
            input_buffer->buffer = new_buffer;
            current_pos = input_buffer->buffer + total_read;
        } else {
            current_pos += len;
        }
    }

    input_buffer->input_length = total_read - 1;
    input_buffer->buffer[total_read - 1] = 0;
}

void close_input_buffer(InputBuffer* input_buffer) {
    free(input_buffer->buffer);
    free(input_buffer);
}

int main(int argc, char* argv[]) {
  InputBuffer* input_buffer = new_input_buffer();
  while (true) {
    print_prompt();
    read_input(input_buffer);

    if (strcmp(input_buffer->buffer, ".exit") == 0) {
      close_input_buffer(input_buffer);
      exit(EXIT_SUCCESS);
    } else {
      printf("Unrecognized command '%s'.\n", input_buffer->buffer);
    }
  }
}
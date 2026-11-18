#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 4096

static void write_all(int fd, const char *buffer, ssize_t size)
{
    ssize_t written = 0;

    while (written < size) {
        ssize_t result = write(fd, buffer + written, size - written);

        if (result <= 0) {
            break;
        }

        written += result;
    }
}

int main(int argc, char *argv[])
{
    int num_files = argc - 1;
    int fds[num_files > 0 ? num_files : 1];
    char buffer[BUFFER_SIZE];
    ssize_t bytes;

    // Open all files for writing
    for (int i = 0; i < num_files; i++) {
        fds[i] = open(argv[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    }

    // Read input and write it to stdout and files
    while ((bytes = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0) {
        write_all(STDOUT_FILENO, buffer, bytes);

        for (int i = 0; i < num_files; i++) {
            write_all(fds[i], buffer, bytes);
        }
    }

    // Close all opened files
    for (int i = 0; i < num_files; i++) {
        close(fds[i]);
    }

    return 0;
}

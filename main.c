#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define MAX_LINE 4096

void search_fd(int fd, const char *pattern) {
    char line[MAX_LINE];
    char ch;
    int pos = 0;
    ssize_t bytes_read;

    while ((bytes_read = read(fd, &ch, 1)) > 0) {
        if (pos < MAX_LINE - 1) {
            line[pos++] = ch;
        }
        if (ch == '\n') {
            line[pos] = '\0';
            if (strstr(line, pattern) != NULL) {
                write(STDOUT_FILENO, line, pos);
            }
            pos = 0;
        }
    }

    if (pos > 0) {
        line[pos] = '\0';
        if (strstr(line, pattern) != NULL) {
            write(STDOUT_FILENO, line, pos);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        const char *err = "Usage: ./grep <pattern> [file...]\n";
        write(STDERR_FILENO, err, strlen(err));
        return 1;
    }

    const char *pattern = argv[1];

    if (argc == 2) {
        search_fd(STDIN_FILENO, pattern);
        return 0;
    }

    for (int i = 2; i < argc; i++) {
        int fd = open(argv[i], O_RDONLY);
        if (fd < 0) {
            const char *err = "Error opening file\n";
            write(STDERR_FILENO, err, strlen(err));
            continue;
        }
        search_fd(fd, pattern);
        close(fd);
    }

    return 0;
}

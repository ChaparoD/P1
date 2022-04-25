/*#include <stdio.h>

int main(int argc, char const *argv[])
{
  printf("Hello P1!\n");
}*/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

const uint8_t data[] =
        { 0x54, 0x65, 0x6d, 0x70, 0x6f, 0x72, 0x61, 0x72, 0x79,
          0x20, 0x73, 0x74, 0x72, 0x69, 0x6e, 0x67, 0x20, 0x74,
          0x6f, 0x20, 0x62, 0x65, 0x20, 0x77, 0x72, 0x69, 0x74,
          0x74, 0x65, 0x6e, 0x20, 0x74, 0x6f, 0x20, 0x66, 0x69,
          0x6c, 0x65 };

const char* filename = "memformat.bin";

int main(void) {

    FILE* output_file = fopen(filename, "wb+");
    if (!output_file) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    fwrite(data, 1, sizeof data, output_file);
    printf("Done Writing!\n");
    fclose(output_file);

    FILE* in_file = fopen(filename, "rb");
    if (!in_file) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    struct stat sb;
    if (stat(filename, &sb) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    char* file_contents = malloc(sb.st_size);
    fread(file_contents, sb.st_size, 1, in_file);

    printf("read data: %s\n", file_contents);
    fclose(in_file);

    free(file_contents);
    exit(EXIT_SUCCESS);
}

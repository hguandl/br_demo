#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <brotli/encode.h>
#include <brotli/decode.h>

const int BUF_SIZE = 2048;

int main(int argc, const char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>.\n", argv[0]);
        return 1;
    }
    const char *filename = argv[1];

    uint8_t *input = NULL;
    size_t length;
    FILE *fp = fopen(filename, "rb");

    if (fp != NULL) {
        fseek(fp, 0, SEEK_END);
        length = ftell(fp) > BUF_SIZE ? BUF_SIZE : ftell(fp);
        fseek(fp, 0, SEEK_SET);
        input = (uint8_t *)malloc(length);
        if (input != NULL) {
            fread(input, 1, length, fp);
        }
    }

    fclose(fp);
    printf("File size: %ld byte(s).\n", length);

    size_t output_size = length;
    uint8_t *output = NULL;
    if (input != NULL) {
        output = (uint8_t *)malloc(output_size);
        BROTLI_BOOL success = BrotliEncoderCompress(BROTLI_DEFAULT_QUALITY, 
                                                    BROTLI_DEFAULT_WINDOW,
                                                    BROTLI_DEFAULT_MODE,
                                                    length,
                                                    input,
                                                    &output_size,
                                                    output);
        if (success) {
            printf("Compressed size: %ld byte(s).\n", output_size);
        } else {
            printf("Compression failed.\n");
        }
    }

    size_t decoded_size = BUF_SIZE;
    uint8_t *decoded = NULL;
    if (output != NULL) {
        decoded = (uint8_t *)malloc(decoded_size + 1);
        BROTLI_BOOL success = BrotliDecoderDecompress(output_size,
                                                      output,
                                                      &decoded_size,
                                                      decoded);
        if (success) {
            decoded[decoded_size] = '\0';
            printf("Decompression result:\n%s", (char *)decoded);
        } else {
            printf("Decompression failed.\n");
        }
    }

    return 0;
}

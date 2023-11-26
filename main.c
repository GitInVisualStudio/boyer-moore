#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "boyer_more.h"
#include "naive.h"

#define TIME() (clock() / (CLOCKS_PER_SEC / 1000.0f))
#define RAND_CHAR() ((uint8_t)(rand() / ((double)RAND_MAX) * (1 << 8)))
#define ITERATIONS 500

typedef bool (*filter)(uint8_t);
typedef float (*search)(uint8_t *, size_t, uint8_t *, size_t);

const char *const files[] = {
    "data/bible.txt", "data/E.coli", "data/world192.txt"};

uint8_t *load_data(size_t *string_length)
{
    uint8_t *data = NULL;
    size_t length = 0;
    for (size_t i = 0; i < sizeof(files) / sizeof(char *); ++i)
    {
        FILE *fd = fopen(files[i], "r");
        fseek(fd, 0, SEEK_END);
        size_t file_length = ftell(fd);
        rewind(fd);

        data = realloc(data, sizeof(uint8_t) * (length + file_length));
        int value = fread(data + length, sizeof(uint8_t), file_length, fd);
        if (value < 0)
            exit(-1);
        length += file_length;
        fclose(fd);
    }
    *string_length = length;
    return data;
}

uint8_t *random_pattern_filter(size_t length, filter filter)
{
    uint8_t *generated_pattern = (uint8_t *)calloc(length + 1, sizeof(uint8_t));
    for (size_t i = 0; i < length; ++i)
    {
        do
        {
            generated_pattern[i] = RAND_CHAR();
        } while (!filter(generated_pattern[i]));
    }
    return generated_pattern;
}

bool is_alphabet(uint8_t c)
{
    return (c >= 65 && c <= 90) || (c >= 97 && c <= 122);
}

float test_boyer_moore(uint8_t *data, size_t data_length, uint8_t *pattern, size_t pattern_length)
{
    float start_time = TIME();
    for (size_t i = 0; i < ITERATIONS; ++i)
        boyer_moore(data, data_length, pattern, pattern_length);
    float end_time = TIME();
    return end_time - start_time;
}

float test_naive(uint8_t *data, size_t data_length, uint8_t *pattern, size_t pattern_length)
{
    float start_time = TIME();
    for (size_t i = 0; i < ITERATIONS; ++i)
    {
        uint8_t *value = naive(data, data_length, pattern, pattern_length);
        // if we dont do that, the compiler optimizes naive out
        if (value)
        {
            printf("%s\n", value);
        }
    }
    float end_time = TIME();
    return end_time - start_time;
}

void test(search search)
{
    size_t data_length;
    uint8_t *data = load_data(&data_length);
    size_t slice = data_length / 4;

    for (size_t i = 0; i < 4; ++i)
    {
        size_t data_length = slice * (i + 1);
        for (size_t pattern_length = 8; pattern_length <= 64; pattern_length *= 2)
        {
            uint8_t *pattern = random_pattern_filter(pattern_length, is_alphabet);
            float time = search(data, data_length, pattern, pattern_length);
            printf("%li;%li;%f\n", data_length, pattern_length, time);
            // don't forget to free your memory!
            free(pattern);
        }
    }
}

int main(int argc, const char **argv)
{
    if (argc != 2)
        return 1;
    if (!strcmp(argv[1], "naive"))
        test(test_naive);
    if (!strcmp(argv[1], "boyermoore"))
        test(test_boyer_moore);
    return 0;
}
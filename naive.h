#include <stdint.h>
#include <stddef.h>

uint8_t *naive(uint8_t *string, size_t stringlen, uint8_t *pat, size_t patlen)
{
    for (size_t i = 0; i < stringlen - patlen; ++i)
    {
        for (size_t j = 0; j < patlen; ++j)
        {
            if (string[i + j] != pat[j])
                break;
            if (j + 1 == patlen)
                return &string[i];
        }
    }
    return NULL;
}

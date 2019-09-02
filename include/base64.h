#ifndef BASE64_H
#define BASE64_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

size_t base64_encode_len(size_t input_length);
void base64_encode(const void *data, size_t input_length, char *encoded_data);

size_t base64_decode_len(const char *data, size_t input_length);
bool base64_decode(const char *data, size_t input_length, void *decoded_data);

void base64_init(void);

#endif

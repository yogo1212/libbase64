#include <stdlib.h>

#include "base64.h"

static char base64_alphabet[] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
	'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
	'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
	'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
	'w', 'x', 'y', 'z', '0', '1', '2', '3',
	'4', '5', '6', '7', '8', '9', '+', '/',
};

static uint8_t decoding_table[256];
static int mod_table[] = {0, 2, 1};

size_t base64_encode_len(size_t input_length)
{
	return 4 * ((input_length + 2) / 3) + 1;
}

void base64_encode(const void *_data, size_t input_length, char *encoded_data)
{
	const uint8_t *data = _data;
	size_t output_length = base64_encode_len(input_length);

	for (size_t i = 0, j = 0; i < input_length;) {
		uint8_t octet_a = (i < input_length) ? (uint8_t) data[i++] : 0;
		uint8_t octet_b = (i < input_length) ? (uint8_t) data[i++] : 0;
		uint8_t octet_c = (i < input_length) ? (uint8_t) data[i++] : 0;

		uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

		encoded_data[j++] = base64_alphabet[(triple >> 3 * 6) & 0x3F];
		encoded_data[j++] = base64_alphabet[(triple >> 2 * 6) & 0x3F];
		encoded_data[j++] = base64_alphabet[(triple >> 1 * 6) & 0x3F];
		encoded_data[j++] = base64_alphabet[(triple >> 0 * 6) & 0x3F];
	}

	for (size_t i = 0; i < mod_table[input_length % 3]; i++)
		encoded_data[output_length - 2 - i] = '=';

	encoded_data[output_length - 1] = '\0';
}

size_t base64_decode_len(const char *data, size_t input_length)
{
	size_t res = input_length / 4 * 3;

	if (data[input_length - 1] == '=')
		res--;

	if (data[input_length - 2] == '=')
		res--;

	return res;
}

bool base64_decode(const char *data, size_t input_length, void *_out)
{
	uint8_t *out = _out;
	if (input_length % 4 != 0)
		return false;

	size_t output_length = base64_decode_len(data, input_length);

	for (size_t i = 0, j = 0; i < input_length;) {
		uint32_t sextet_a = (data[i] == '=') ? (0 & i++) : decoding_table[(int) data[i++]];
		uint32_t sextet_b = (data[i] == '=') ? (0 & i++) : decoding_table[(int) data[i++]];
		uint32_t sextet_c = (data[i] == '=') ? (0 & i++) : decoding_table[(int) data[i++]];
		uint32_t sextet_d = (data[i] == '=') ? (0 & i++) : decoding_table[(int) data[i++]];

		uint32_t triple = (sextet_a << 3 * 6)
			+ (sextet_b << 2 * 6)
			+ (sextet_c << 1 * 6)
			+ (sextet_d << 0 * 6);

		if (j < output_length)
			out[j++] = (triple >> 2 * 8) & 0xFF;

		if (j < output_length)
			out[j++] = (triple >> 1 * 8) & 0xFF;

		if (j < output_length)
			out[j++] = (triple >> 0 * 8) & 0xFF;
	}

	return true;
}


void base64_init(void)
{
	for (int i = 0; i < 64; i++)
		decoding_table[(uint8_t) base64_alphabet[i]] = i;
}

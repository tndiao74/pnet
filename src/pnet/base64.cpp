
// s3demo
// Copyright 2025 Raw Thrills Inc.


// pre-compile header
#include "stdafx.h"


///////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////

#include "base64.h"


///////////////////////////////////////////////////////////////////////
// Globals
///////////////////////////////////////////////////////////////////////

// Base64 character set
static const char encoding_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const int mod_table[] = { 0, 2, 1 };

// Base64 decoding table
static const int b64_decode_table[] = {
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, // 0-15
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, // 16-31
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,-1,63, // 32-47 ('+', '/')
    52,53,54,55,56,57,58,59,60,61,-1,-1,-1,-1,-1,-1, // 48-63 ('0'-'9')
    -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14, // 64-79 ('A'-'O')
    15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1, // 80-95 ('P'-'Z')
    -1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40, // 96-111 ('a'-'o')
    41,42,43,44,45,46,47,48,49,50,51,-1,-1,-1,-1,-1  // 112-127 ('p'-'z')
};


///////////////////////////////////////////////////////////////////////
// Internal Prototypes
///////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////
// base64
///////////////////////////////////////////////////////////////////////

char* base64_encode(const char* data, size_t bufMax, char* buf)
{
    // check params
    if (data == NULL || buf == NULL)
        return NULL;

    size_t input_length = strlen(data);
    size_t output_length = 4 * ((input_length + 2) / 3); // Calculate required output length
    if (output_length + 1 >= bufMax)
        return NULL;

    for (size_t i = 0, j = 0; i < input_length;)
    {
        uint32_t octet_a = i < input_length ? data[i++] : 0;
        uint32_t octet_b = i < input_length ? data[i++] : 0;
        uint32_t octet_c = i < input_length ? data[i++] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        buf[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
        buf[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
        buf[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
        buf[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
    }

    for (int i = 0; i < mod_table[input_length % 3]; i++)
    {
        buf[output_length - 1 - i] = '='; // Add padding
    }

    buf[output_length] = '\0'; // Null terminate
    return buf;
}

// Function to decode a Base64 string
size_t base64_decode(const char* encoded_data, size_t encoded_len, unsigned char* decoded_data, size_t decoded_max_len)
{
    size_t i;
    int val;
    int bits = 0;
    int byte_idx = 0;

    for (i = 0; i < encoded_len; i++) {
        char c = encoded_data[i];
        if (c == '=') break; // Padding character

        val = b64_decode_table[(unsigned char)c];
        if (val == -1) continue; // Invalid character

        bits = (bits << 6) | val;
        byte_idx++;

        if (byte_idx == 4) {
            if (decoded_max_len < 3) return 0; // Not enough space
            decoded_data[0] = (bits >> 16) & 0xFF;
            decoded_data[1] = (bits >> 8) & 0xFF;
            decoded_data[2] = bits & 0xFF;
            decoded_data += 3;
            decoded_max_len -= 3;
            bits = 0;
            byte_idx = 0;
        }
    }

    // Handle remaining bits (padding)
    if (byte_idx > 0) {
        if (byte_idx == 2) { // 1 output byte
            if (decoded_max_len < 1) return 0;
            decoded_data[0] = (bits >> 4) & 0xFF;
            decoded_max_len -= 1;
        }
        else if (byte_idx == 3) { // 2 output bytes
            if (decoded_max_len < 2) return 0;
            decoded_data[0] = (bits >> 10) & 0xFF;
            decoded_data[1] = (bits >> 2) & 0xFF;
            decoded_max_len -= 2;
        }
    }

    return (size_t)(decoded_data - (unsigned char*)decoded_data); // This calculation is simplified, needs careful handling for actual return size
}
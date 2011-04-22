#ifndef STORMMQ_AMQP_TYPE_TYPE_UNIONS_H
#define STORMMQ_AMQP_TYPE_TYPE_UNIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <wchar.h>

typedef union amqp_four_byte_t
{
    unsigned char b[4];
    uint32_t _uint;
    int32_t _int;
    float _float;
    wchar_t _wchar;
} amqp_four_byte_t;

typedef union amqp_eight_byte_t
{
    unsigned char b[8];
    uint64_t _ulong;
    int64_t _long;
    double _double;
    amqp_timestamp_t _timestamp;
} amqp_eight_byte_t;

typedef union amqp_two_byte_t
{
    unsigned char b[2];
    uint16_t _ushort;
    int16_t _short;
} amqp_two_byte_t;

/* TODO - review - This is kind of silly but it's use prevents sign bit extension errors */
typedef union amqp_one_byte_t
{
    unsigned char b[1];
    uint8_t _unsigned;
    int8_t _signed;
} amqp_one_byte_t;

#ifdef __cplusplus
}
#endif
#endif
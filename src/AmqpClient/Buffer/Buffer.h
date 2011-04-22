#ifndef STORMMQ_AMQP_BUFFER_H
#define STORMMQ_AMQP_BUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>
#include "amqp.h"
#include "Memory/Memory.h"
#include "Type/TypeUnions.h"

#ifndef AMQP_BUFFER_T_TYPE
#define AMQP_BUFFER_T_TYPE
typedef struct amqp_buffer_t amqp_buffer_t;
#endif

#ifndef AMQP_STREAM_T_TYPE
#define AMQP_STREAM_T_TYPE
typedef struct amqp_stream_t amqp_stream_t;
#endif

#ifndef AMQP_CONTEXT_T_TYPE
#define AMQP_CONTEXT_T_TYPE
typedef struct amqp_context_t amqp_context_t;
#endif

typedef struct amqp_buffer_limit_t
{
    size_t index;
    size_t size;
} amqp_buffer_limit_t;

struct amqp_buffer_t
{
    unsigned char *bytes;
    amqp_buffer_limit_t limit;
    size_t actual_size;
};

extern amqp_buffer_t *amqp_alloc_amqp_buffer_t(amqp_context_t *context);
extern void amqp_dealloc_amqp_buffer_t(amqp_context_t *context, amqp_buffer_t *);

extern int amqp_buffer_putc(amqp_buffer_t *, int);

extern int amqp_buffer_puts(amqp_buffer_t *, const unsigned char *source, size_t n);
extern int amqp_buffer_put_buffer_contents(amqp_buffer_t *desc, amqp_buffer_t *source);
extern uint32_t amqp_buffer_read_size(amqp_buffer_t *buffer, size_t width);

extern int amqp_buffer_check_free_space(amqp_buffer_t *buffer, size_t needed);

extern amqp_buffer_limit_t amqp_buffer_set_limit(amqp_buffer_t *, size_t);
extern void amqp_buffer_restore_limit(amqp_buffer_t *buffer, amqp_buffer_limit_t *old_limit);

extern int amqp_buffer_read_from_stream(amqp_buffer_t *buffer, amqp_stream_t *stream, int n);
extern int amqp_buffer_write_to_stream(amqp_buffer_t *buffer, amqp_stream_t *stream);

inline static
int amqp_buffer_check_available(amqp_buffer_t *buffer, size_t desired)
{
    return buffer->limit.index <= buffer->limit.size && (buffer->limit.size - buffer->limit.index) >= desired;
}

inline static
void amqp_unchecked_putc(amqp_buffer_t *buffer, int c)
{
    buffer->bytes[buffer->limit.size++] = c;
}

inline static
void amqp_buffer_advance_read_index(amqp_buffer_t *buffer, size_t amount)
{
    buffer->limit.index += amount;
}

inline static
void amqp_buffer_advance_write_point(amqp_buffer_t *buffer, size_t amount)
{
    buffer->limit.size += amount;
}

inline static
size_t amqp_buffer_set_write_point(amqp_buffer_t *buffer, size_t new_value)
{
    size_t old_value = buffer->limit.size;
    buffer->limit.size = new_value;
    return old_value;
}

inline static
size_t amqp_buffer_index(amqp_buffer_t *buffer)
{
    return buffer->limit.index;
}

// TODO - remove
inline static
size_t amqp_buffer_size(amqp_buffer_t *buffer)
{
    return buffer->limit.size;
}

inline static
size_t amqp_buffer_write_point(amqp_buffer_t *buffer)
{
    return amqp_buffer_size(buffer);
}

// at end check is relatice to the current limit
inline static
int amqp_buffer_at_end(amqp_buffer_t *buffer)
{
    return buffer->limit.index == buffer->limit.size;
}

inline static
void amqp_buffer_reset(amqp_buffer_t *buffer)
{
    buffer->limit.index = buffer->limit.size = 0;
}

inline static
const unsigned char *amqp_buffer_pointer(amqp_buffer_t *buffer, size_t offset)
{
    return buffer->bytes + offset;
}

inline static
int amqp_buffer_unchecked_getc(amqp_buffer_t *buffer)
{
    return buffer->bytes[buffer->limit.index++];
}

// getc is checked against the current limit
inline static
int amqp_buffer_getc(amqp_buffer_t *buffer)
{
    return buffer->limit.index < buffer->limit.size ? buffer->bytes[buffer->limit.index++] : -1;
}

// Unchecked get is relative to the front of the buffer, not the current limit.
inline static
int amqp_unchecked_getc_at(amqp_buffer_t *buffer, size_t index)
{
    return buffer->bytes[index];
}

#ifdef WORDS_BIGENDIAN
#error BIG ENDIAN not supported
#else
static inline amqp_one_byte_t amqp_ntoh_8(amqp_buffer_t *buffer, size_t offset) __attribute__((always_inline));
static inline amqp_two_byte_t amqp_ntoh_16(amqp_buffer_t *buffer, size_t offset) __attribute__((always_inline));
static inline amqp_four_byte_t amqp_ntoh_32(amqp_buffer_t *buffer, size_t offset) __attribute__((always_inline));
static inline amqp_eight_byte_t amqp_ntoh_64(amqp_buffer_t *buffer, size_t offset) __attribute__((always_inline));
static inline void amqp_hton_16(amqp_buffer_t *buffer, amqp_two_byte_t value) __attribute__((always_inline));
static inline void amqp_hton_32(amqp_buffer_t *buffer, amqp_four_byte_t value) __attribute__((always_inline));
static inline void amqp_hton_64(amqp_buffer_t *buffer, amqp_eight_byte_t value) __attribute__((always_inline));

static inline amqp_one_byte_t amqp_ntoh_8(amqp_buffer_t *buffer, size_t offset)
{
    amqp_one_byte_t result;
    result._unsigned = amqp_unchecked_getc_at(buffer, offset);
    return result;
}

static inline amqp_two_byte_t amqp_ntoh_16(amqp_buffer_t *buffer, size_t offset)
{
    amqp_two_byte_t result;
    result.b[1] = amqp_unchecked_getc_at(buffer, offset++);
    result.b[0] = amqp_unchecked_getc_at(buffer, offset);
    return result;
}

static inline amqp_four_byte_t amqp_ntoh_32(amqp_buffer_t *buffer, size_t offset)
{
    amqp_four_byte_t result;
    result.b[3] = amqp_unchecked_getc_at(buffer, offset++);
    result.b[2] = amqp_unchecked_getc_at(buffer, offset++);
    result.b[1] = amqp_unchecked_getc_at(buffer, offset++);
    result.b[0] = amqp_unchecked_getc_at(buffer, offset);
    return result;
}

// static inline
uint32_t amqp_buffer_read_size(amqp_buffer_t *buffer, size_t width);

static inline amqp_eight_byte_t amqp_ntoh_64(amqp_buffer_t *buffer, size_t offset)
{
    amqp_eight_byte_t result;
    result.b[7] = amqp_unchecked_getc_at(buffer, offset++);
    result.b[6] = amqp_unchecked_getc_at(buffer, offset++);
    result.b[5] = amqp_unchecked_getc_at(buffer, offset++);
    result.b[4] = amqp_unchecked_getc_at(buffer, offset++);
    result.b[3] = amqp_unchecked_getc_at(buffer, offset++);
    result.b[2] = amqp_unchecked_getc_at(buffer, offset++);
    result.b[1] = amqp_unchecked_getc_at(buffer, offset++);
    result.b[0] = amqp_unchecked_getc_at(buffer, offset);
    return result;
}

static inline void amqp_hton_16(amqp_buffer_t *buffer, amqp_two_byte_t value)
{
    amqp_unchecked_putc(buffer, value.b[1]);
    amqp_unchecked_putc(buffer, value.b[0]);
}

static inline void amqp_hton_32(amqp_buffer_t *buffer, amqp_four_byte_t value)
{
    amqp_unchecked_putc(buffer, value.b[3]);
    amqp_unchecked_putc(buffer, value.b[2]);
    amqp_unchecked_putc(buffer, value.b[1]);
    amqp_unchecked_putc(buffer, value.b[0]);
}

static inline void amqp_hton_64(amqp_buffer_t *buffer, amqp_eight_byte_t value)
{
    amqp_unchecked_putc(buffer, value.b[7]);
    amqp_unchecked_putc(buffer, value.b[6]);
    amqp_unchecked_putc(buffer, value.b[5]);
    amqp_unchecked_putc(buffer, value.b[4]);
    amqp_unchecked_putc(buffer, value.b[3]);
    amqp_unchecked_putc(buffer, value.b[2]);
    amqp_unchecked_putc(buffer, value.b[1]);
    amqp_unchecked_putc(buffer, value.b[0]);
}

#endif


/*
The next three functions are intended for use by the frame header decoding and
assume that at least 8 bytes are in the buffer.
*/
inline static uint32_t amqp_buffer_read_uint32(amqp_buffer_t *buffer, size_t offset)
{
    return amqp_ntoh_32(buffer, offset)._uint;
}

inline static uint16_t amqp_buffer_read_uint16(amqp_buffer_t *buffer, size_t offset)
{
    return amqp_ntoh_16(buffer, offset)._ushort;
}

inline static uint8_t amqp_buffer_read_uint8(amqp_buffer_t *buffer, size_t offset)
{
    return buffer->bytes[offset];
}

/*
Write functions intended for use by the frame header writers
The assume they are writing to the start of a buffer and do no bounds checking.
*/
inline static void amqp_buffer_write_uint32(amqp_buffer_t *buffer, uint32_t value)
{
    amqp_four_byte_t v;
    v._uint = value;
    amqp_hton_32(buffer, v);
}
inline static void amqp_buffer_write_uint16(amqp_buffer_t *buffer, uint16_t value)
{
    amqp_two_byte_t v;
    v._ushort = value;
    amqp_hton_16(buffer, v);
}
inline static void amqp_buffer_write_uint8(amqp_buffer_t *buffer, uint8_t value)
{
    amqp_unchecked_putc(buffer, value);
}


#ifdef __cplusplus
}
#endif
#endif
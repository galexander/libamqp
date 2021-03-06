/*
   Copyright 2011-2012 StormMQ Limited

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 */

#ifndef LIBAMQP_CODEC_TYPE_TYPE_H
#define LIBAMQP_CODEC_TYPE_TYPE_H

#include <assert.h>

#include "libamqp_common.h"
#include "Codec/Type/EncodingMetaData.h"
#include "Context/Context.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LIBAMQP_AMQP_TYPE_T
#define LIBAMQP_AMQP_TYPE_T
typedef struct amqp_type_t amqp_type_t;
#endif

#ifndef LIBAMQP_AMQP_CONTEXT_TYPE_T
#define LIBAMQP_AMQP_CONTEXT_TYPE_T
typedef struct amqp_context_t amqp_context_t;
#endif

typedef struct amqp_buffer_position_t
{
    size_t index;
    size_t size;
} amqp_buffer_position_t;

typedef struct amqp_type_constructor_t
{
    amqp_encoding_meta_data_t *meta_data;
    uint32_t typedef_flags;
    uint8_t format_code;
    uint8_t extension_type_code;
} amqp_type_constructor_t;

struct amqp_type_t
{
    int invalid_cause ;
    amqp_type_constructor_t constructor;
    amqp_buffer_position_t position;
    union {
        amqp_eight_byte_t b8;
        amqp_four_byte_t b4;
        amqp_two_byte_t b2;
        amqp_one_byte_t b1;
        unsigned char uuid[16];
        struct {
            size_t count;
            amqp_type_t **elements;
        } compound;
        struct {
            size_t count;
            amqp_type_t **elements;
            amqp_type_t *constructor;
        } array;
        struct {
            size_t count;
            amqp_type_t **elements;
        } list;
        struct {
            size_t count;
            amqp_type_t **entries;
        } map;
        struct {
            size_t count;
            amqp_type_t **elements;
        } described;
        struct {
            amqp_buffer_t *buffer;
        } variable;
    } value;
    amqp_type_t *saved_container;
};

extern void amqp_type_initialize_pool(amqp_memory_pool_t *pool);

extern amqp_type_t **amqp_allocate_amqp_type_t_array(amqp_context_t *c, size_t count);
extern void amqp_deallocate_amqp_type_t_array(amqp_context_t *c, amqp_memory_pool_t *pool, amqp_type_t **array, size_t count);
extern amqp_type_t **amqp_realloc_amqp_type_t_array(amqp_context_t *c, amqp_type_t **elements, size_t count);

extern int amqp_type_match(amqp_type_t *lhs, amqp_type_t *rhs);
extern void amqp_mark_type_invalid(amqp_type_t *type, int cause);
extern void amqp_describe_type(char *buffer, size_t size, amqp_type_t *type);

static inline
void amqp_typedef_flags_set(amqp_type_t *type, uint32_t flags)
{
    assert(type);
    type->constructor.typedef_flags |= flags;
}

static inline
void amqp_typedef_flags_clear(amqp_type_t *type, uint32_t bits)
{
    assert(type);
    type->constructor.typedef_flags &= ~bits;
}

static inline
int amqp_typedef_flags_test(amqp_type_t *type, uint32_t bits)
{
    assert(type);
    return type->constructor.typedef_flags & bits;
}

static inline
int amqp_type_is_null(amqp_type_t *type)
{
    return type == 0 || type->constructor.typedef_flags & amqp_is_null;
}

static inline
int amqp_type_is_not_null(amqp_type_t *type)
{
    return type && (type->constructor.typedef_flags & amqp_is_null) == 0;
}

static inline
int amqp_type_is_boolean(amqp_type_t *type)
{
    assert(type);
    return type->constructor.typedef_flags & amqp_is_boolean;
}

static inline
int amqp_type_is_unsigned(amqp_type_t *type)
{
    assert(type);
    return type->constructor.typedef_flags & amqp_is_unsigned;
}

static inline
int amqp_type_is_ubyte(amqp_type_t *type)
{
    assert(type);
    return (type->constructor.typedef_flags & (amqp_is_byte | amqp_is_unsigned)) == (amqp_is_byte | amqp_is_unsigned);
}

static inline
int amqp_type_is_ushort(amqp_type_t *type)
{
    assert(type);
    return (type->constructor.typedef_flags & (amqp_is_short | amqp_is_unsigned)) == (amqp_is_short | amqp_is_unsigned);
}

static inline
int amqp_type_is_uint(amqp_type_t *type)
{
    assert(type);
    return (type->constructor.typedef_flags & (amqp_is_int | amqp_is_unsigned)) == (amqp_is_int | amqp_is_unsigned);
}

static inline
int amqp_type_is_ulong(amqp_type_t *type)
{
    assert(type);
    return (type->constructor.typedef_flags & (amqp_is_long | amqp_is_unsigned)) == (amqp_is_long | amqp_is_unsigned);
}

static inline
int amqp_type_is_signed(amqp_type_t *type)
{
    assert(type);
    return (type->constructor.typedef_flags & amqp_is_unsigned) == 0;
}

static inline
int amqp_type_is_byte(amqp_type_t *type)
{
    assert(type);
    return ((type->constructor.typedef_flags & amqp_is_byte) != 0 ) && amqp_type_is_signed(type);
}

static inline
int amqp_type_is_short(amqp_type_t *type)
{
    assert(type);
    return (type->constructor.typedef_flags & amqp_is_short) && amqp_type_is_signed(type);
}

static inline
int amqp_type_is_int(amqp_type_t *type)
{
    assert(type);
    return (type->constructor.typedef_flags & amqp_is_int) && amqp_type_is_signed(type);
}

static inline
int amqp_type_is_long(amqp_type_t *type)
{
    assert(type);
    return (type->constructor.typedef_flags & amqp_is_long) && amqp_type_is_signed(type);
}

static inline
int amqp_type_is_float(amqp_type_t *type)
{
    assert(type);
    return type->constructor.typedef_flags & amqp_is_float;
}

static inline
int amqp_type_is_double(amqp_type_t *type)
{
    assert(type);
    return type->constructor.typedef_flags & amqp_is_double;
}

static inline
int amqp_type_is_timestamp(amqp_type_t *type)
{
    assert(type);
    return type->constructor.typedef_flags & amqp_is_timestamp;
}

static inline
int amqp_type_is_uuid(amqp_type_t *type)
{
    assert(type);
    return type->constructor.typedef_flags & amqp_is_uuid;
}

static inline
int amqp_type_is_decimal32(amqp_type_t *type)
{
    assert(type);
    return type->constructor.typedef_flags & amqp_is_decimal32;
}

static inline
int amqp_type_is_decimal64(amqp_type_t *type)
{
    assert(type);
    return type->constructor.typedef_flags & amqp_is_decimal64;
}

static inline
int amqp_type_is_decimal128(amqp_type_t *type)
{
    assert(type);
    return type->constructor.typedef_flags & amqp_is_decimal128;
}

static inline
int amqp_type_is_char(amqp_type_t *type)
{
    assert(type);
    return type->constructor.typedef_flags & amqp_is_char;
}

static inline
int amqp_type_is_binary(amqp_type_t *type)
{
    assert(type);
    return type->constructor.typedef_flags & amqp_is_binary;
}

static inline
int amqp_type_is_string(amqp_type_t *type)
{
    assert(type);
    return type->constructor.typedef_flags & amqp_is_string;
}

static inline
int amqp_type_is_symbol(amqp_type_t *type)
{
    assert(type);
    return type->constructor.typedef_flags & amqp_is_symbol;
}

static inline
int amqp_type_is_variable(amqp_type_t *type)
{
    assert(type);
    return type->constructor.typedef_flags & amqp_is_variable_mask;
}

static inline
int amqp_type_is_list(amqp_type_t *type)
{
    assert(type);
    return type->constructor.typedef_flags & amqp_is_list;
}

static inline
int amqp_type_is_map(amqp_type_t *type)
{
    assert(type);
    return type->constructor.typedef_flags & amqp_is_map;
}

static inline
int amqp_type_is_array(amqp_type_t *type)
{
    assert(type);
    return type->constructor.typedef_flags & amqp_is_array;
}

static inline
int amqp_type_is_container(amqp_type_t *type)
{
    assert(type);
    return type->constructor.typedef_flags & amqp_is_container_mask;
}

static inline
int amqp_type_is_composite(amqp_type_t *type)
{
    assert(type);
    return type->constructor.typedef_flags & amqp_is_composite;
}

static inline
int amqp_type_is_descriptor(amqp_type_t *type)
{
    assert(type);
    return type->constructor.typedef_flags & amqp_is_descriptor;
}

static inline
int amqp_type_is_described(amqp_type_t *type)
{
    assert(type);
    return type->constructor.typedef_flags & amqp_is_described;
}

static inline
int amqp_type_is_encoded(amqp_type_t *type)
{
    assert(type);
    return type->constructor.typedef_flags & amqp_is_encoded;
}

static inline
int amqp_type_is_incomplete(amqp_type_t *type)
{
    assert(type);
    return type->constructor.typedef_flags & amqp_is_incomplete;
}

static inline
int amqp_type_is_empty_list(amqp_type_t *type)
{
    assert(type);
    return amqp_type_is_list(type) != 0 && type->value.list.count == 0;
}

static inline
int amqp_type_is_valid(amqp_type_t *type)
{
    assert(type);
    return (type->constructor.typedef_flags & (amqp_is_invalid | amqp_is_incomplete)) == 0;
}

static inline
int amqp_type_is_invalid(amqp_type_t *type)
{
    assert(type);
    return type->constructor.typedef_flags & (amqp_is_invalid | amqp_is_incomplete);
}

static inline
int amqp_type_is_contained(amqp_type_t *type)
{
    assert(type);
    return type->constructor.typedef_flags & amqp_is_contained;
}

static inline
amqp_type_t *amqp_type_list_element(amqp_type_t *type, size_t index)
{
    assert(amqp_type_is_list(type) && type->value.list.count > index);
    return type->value.list.elements[index];
}

static inline
amqp_type_t *amqp_type_map_element(amqp_type_t *type, size_t index)
{
    assert(amqp_type_is_map(type) && type->value.map.count > index);
    return type->value.map.entries[index];
}

// ACCESS FUNCTIONS
static inline
amqp_type_t *amqp_type_get_descriptor(amqp_type_t *type)
{
    assert(amqp_type_is_composite(type));
    return type->value.described.elements[0];
}

static inline
amqp_type_t *amqp_type_get_described(amqp_type_t *type)
{
    assert(amqp_type_is_composite(type));
    return type->value.described.elements[1];
}

static inline
size_t amqp_type_copy_to(amqp_type_t *type, uint8_t *buffer, size_t amount)
{
    size_t size;

    assert(amqp_type_is_variable(type));

    size = type->position.size > amount ? amount : type->position.size;
    amqp_buffer_block_copy(type->value.variable.buffer, buffer, type->position.index, size);
    return size;
}

static inline
uint8_t amqp_type_get_byte_at(amqp_type_t *type, size_t index)
{
    assert(amqp_type_is_variable(type));
    return amqp_unchecked_getc_at(type->value.variable.buffer, type->position.index + index);
}

// CONVERSION FUNCTIONS
static inline
int amqp_type_to_boolean(amqp_type_t *type)
{
    return type->value.b1._signed;
}

static inline
uint8_t amqp_type_to_ubyte(amqp_type_t *type)
{
    assert(amqp_type_is_ubyte(type));
    return type->value.b1._unsigned;
}

static inline
int8_t amqp_type_to_byte(amqp_type_t *type)
{
    assert(amqp_type_is_byte(type));
    return type->value.b1._signed;
}

static inline
uint16_t amqp_type_to_ushort(amqp_type_t *type)
{
    assert(amqp_type_is_ushort(type));
    return type->value.b2._ushort;
}

static inline
int16_t amqp_type_to_short(amqp_type_t *type)
{
    assert(amqp_type_is_short(type));
    return type->value.b2._short;
}

static inline
uint32_t amqp_type_to_uint(amqp_type_t *type)
{
    assert(amqp_type_is_uint(type));
    return type->value.b4._uint;
}

static inline
int32_t amqp_type_to_int(amqp_type_t *type)
{
    assert(amqp_type_is_int(type));
    return type->value.b4._int;
}

static inline
wchar_t amqp_type_to_char(amqp_type_t *type)
{
    assert(amqp_type_is_char(type));
    return type->value.b4._wchar;
}

static inline
uint64_t amqp_type_to_ulong(amqp_type_t *type)
{
    assert(amqp_type_is_ulong(type));
    return type->value.b8._ulong;
}

static inline
int64_t amqp_type_to_long(amqp_type_t *type)
{
    assert(amqp_type_is_long(type));
    return type->value.b8._long;
}

static inline
amqp_timestamp_t amqp_type_to_timestamp(amqp_type_t *type)
{
    assert(amqp_type_is_timestamp(type));
    return type->value.b8._timestamp;
}

static inline
float amqp_type_to_float(amqp_type_t *type)
{
    assert(amqp_type_is_float(type));
    return type->value.b4._float;
}

static inline
double amqp_type_to_double(amqp_type_t *type)
{
    assert(amqp_type_is_double(type));
    return type->value.b8._double;
}

static inline
amqp_type_t *amqp_type_array_type(amqp_type_t *type)
{
    assert(amqp_type_is_array(type));
    return type->value.array.constructor;
}

#ifdef __cplusplus
}
#endif
#endif



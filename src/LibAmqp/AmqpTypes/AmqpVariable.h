/*
   Copyright 2011 StormMQ Limited

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
#ifndef LIBAMQP_AMQP_TYPES_AMQP_VARIABLE_H
#define LIBAMQP_AMQP_TYPES_AMQP_VARIABLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>

#include "AmqpTypes/AmqpLeader.h"
#include "Codec/Type/Type.h"
#include "Codec/Type/TypeExtension.h"

typedef struct amqp_variable_t
{
    amqp_type_t *type;
    size_t size;
    struct amqp_block_header block;
    const uint8_t *data;
} amqp_variable_t;

extern int amqp_variable_compare(amqp_variable_t *lhs, amqp_variable_t *rhs);
extern int amqp_variable_compare_with_cstr(amqp_variable_t *lhs, const char *rhs);
extern int amqp_variable_compare_with_bytes(amqp_variable_t *lhs, const uint8_t *rhs, size_t size);
extern uint32_t amqp_variable_hash(amqp_variable_t *variable);

static inline
int amqp_variable_to_char_bytes(amqp_variable_t *variable, uint8_t *buffer, size_t buffer_size)
{
    return amqp_type_copy_to(variable->type, buffer, buffer_size);
}

#ifdef __cplusplus
}
#endif
#endif
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

#ifndef LIBAMQP_MEMORY_POOL_H
#define LIBAMQP_MEMORY_POOL_H

#include <stdlib.h>
#include <limits.h>
#include "Memory/Memory.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct amqp_memory_pool_t amqp_memory_pool_t;
typedef void (*amqp_pool_callback_t)(amqp_memory_pool_t *pool, void *object);

typedef struct amqp_memory_block_t amqp_memory_block_t;

struct amqp_memory_pool_t
{
    amqp_memory_block_t *block_list;
    amqp_allocation_stats_t stats;

    int initialized; // TODO - delete this field as it's kinda pointless
    int allocations_per_block;
    unsigned long allocations_mask;
    size_t safe_alignment;       /* typically 4 or 16 */
    size_t allocation_data_padding; /* padding after header to ensure correct alignment of object */
    size_t block_data_padding;      /* padding before first allocation in block */
    size_t object_size;         /* the size of the object being allocated from the pool */
    size_t allocation_size;     /* size of an allocation block: header + padding + object */

    amqp_pool_callback_t initializer_callback;
    amqp_pool_callback_t destroyer_callback;
};

extern void amqp_initialize_pool(amqp_memory_pool_t *pool, size_t object_size);
extern void amqp_initialize_pool_specifing_block_limits(amqp_memory_pool_t *pool, size_t object_size, int allocations_per_block);
extern void amqp_initialize_pool_suggesting_block_size(amqp_memory_pool_t *pool, size_t object_size, size_t suggested_block_size);
extern void amqp_pool_specify_initialization_callbacks(amqp_memory_pool_t *pool, amqp_pool_callback_t initializer_callback, amqp_pool_callback_t destroyer_callback);

extern void *amqp_allocate(amqp_memory_pool_t *pool);
extern void amqp_deallocate(amqp_memory_pool_t *pool, void *);

#ifdef __cplusplus
}
#endif
#endif
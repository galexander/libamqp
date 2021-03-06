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

#include "Context/Context.h"
#include "Transport/Frame/Frame.h"

#include "debug_helper.h"

static
void amqp_initialize_amqp_frame_t(amqp_context_t *c, amqp_memory_pool_t *pool, amqp_frame_t *frame)
{
    assert(pool != 0);
    assert(frame != 0);
}

static
void amqp_cleanup_amqp_frame_t(amqp_context_t *c, amqp_memory_pool_t *pool, amqp_frame_t *frame)
{
    assert(pool != 0);
    assert(frame != 0);
}

void amqp_frame_initialize_pool(amqp_memory_pool_t *pool)
{
    amqp_initialize_pool_suggesting_block_size(pool, sizeof(amqp_frame_t), 2048, "frame");
    amqp_pool_specify_initialization_callbacks(pool, (amqp_pool_callback_t) amqp_initialize_amqp_frame_t, (amqp_pool_callback_t) amqp_cleanup_amqp_frame_t);
}


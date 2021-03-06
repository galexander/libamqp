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

#ifndef LIBAMQP_CONTEXT_CONTEXT_H
#define LIBAMQP_CONTEXT_CONTEXT_H

#include <stdarg.h>
#include <stdio.h>

#include "libamqp.h"
#include "Memory/Pool.h"
#include "Buffer/Buffer.h"
#include "Codec/Type/Type.h"
#include "Context/DebugParams.h"
#include "Context/ErrorHandling.h"
#include "Context/SaslIdentity.h"
#include "Messaging/MessagingPlugin.h"
#include "Thread/Thread.h"
#include "AmqpTypes/AmqpDescriptor.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef LIBAMQP_AMQP_TYPE_T
#define LIBAMQP_AMQP_TYPE_T
    typedef struct amqp_type_t amqp_type_t;
#endif

#ifndef LIBAMQP_AMQP_CONTEXT_TYPE_T
#define LIBAMQP_AMQP_CONTEXT_TYPE_T
    typedef struct amqp_context_t amqp_context_t;
#endif

#ifndef LIBAMQP_AMQP_SASL_IDENTITY_TYPE_T
#define LIBAMQP_AMQP_SASL_IDENTITY_TYPE_T
typedef struct amqp_sasl_identity_t amqp_sasl_identity_t;
#endif

#ifndef LIBAMQP_AMQP_FRAME_TYPE_T
#define LIBAMQP_AMQP_FRAME_TYPE_T
typedef struct amqp_frame_t amqp_frame_t;
#endif

#ifndef LIBAMQP_AMQP_EVENT_LOOP_TYPE_T
#define LIBAMQP_AMQP_EVENT_LOOP_TYPE_T
    typedef struct ev_loop amqp_event_loop_t;
#endif

#ifndef LIBAMQP_AMQP_CONNECTION_TYPE_T
#define LIBAMQP_AMQP_CONNECTION_TYPE_T
typedef struct amqp_connection_t amqp_connection_t;
#endif

#ifndef LIBAMQP_AMQP_HASH_TABLE_TYPE_T
#define LIBAMQP_AMQP_HASH_TABLE_TYPE_T
typedef struct amqp_hash_table_t amqp_hash_table_t;
#endif

#ifndef LIBAMQP_AMQP_MAP_TYPE_T
#define LIBAMQP_AMQP_MAP_TYPE_T
typedef struct amqp_map_t amqp_map_t;
#endif

#ifndef LIBAMQP_AMQP_SASL_PLUGIN_TYPE_T
#define LIBAMQP_AMQP_SASL_PLUGIN_TYPE_T
typedef struct amqp_sasl_plugin_t amqp_sasl_plugin_t;
#endif

#ifndef LIBAMQP_AMQP_SASL_PLUGIN_NODE_TYPE_T
#define LIBAMQP_AMQP_SASL_PLUGIN_NODE_TYPE_T
typedef struct amqp_sasl_plugin_node_t amqp_sasl_plugin_node_t;
#endif

#ifndef LIBAMQP_AMQP_SYMBOL_T
#define LIBAMQP_AMQP_SYMBOL_T
    typedef struct amqp_symbol_t amqp_symbol_t;
#endif

#define amqp_allocate_buffer(c)     amqp__allocate_buffer((c) AMQP_DEBUG_PARAMS)
#define amqp_allocate_frame(c)      amqp__allocate_frame((c) AMQP_DEBUG_PARAMS)
#define amqp_allocate_type(c)       amqp__allocate_type((c) AMQP_DEBUG_PARAMS)

    typedef int amqp_debug_print_c_t(int c);

    typedef union amqp_outputter_arg_t
    {
        FILE *stream;
        struct
        {
            char *buffer;
            size_t buffer_size;
        } buffered;
    } amqp_outputter_arg_t;

    typedef void (*amqp_outputter_t)(amqp_context_t *context, amqp_outputter_arg_t *desc, const char *filename, int line_number, const char *context_name, const char *label, const char *source, const char *extra, const char *format, va_list args);

    struct amqp_context_t
    {
        int error_code;

        struct
        {
            amqp_debug_print_c_t *putc;     // TODO - consider removing this and using outputter.
            int formatted_print;
            int use_name_for_composite_type;
            int continue_on_usage_error;
            int max_listen_queue_length;
        } config;

        struct
        {
            char name[32];
            amqp_outputter_arg_t arg;
            amqp_outputter_t outputter;
            int level;
            struct
            {
                int indent;
                int auto_indent;
                int last_char;
            } indent;
            int type_print_inside_array_flag;
        } debug;

        struct
        {
            amqp_allocation_stats_t allocation_stats;
            amqp_memory_pool_t amqp_buffer_t_pool;
            amqp_memory_pool_t amqp_type_t_pool;
            amqp_memory_pool_t amqp_frame_t_pool;
#ifdef LIBAMQP_DEBUG_ALLOCATIONS
            struct {
                amqp__memory_block_t *allocated_blocks;
            } debug;
#endif
        } memory;

        struct
        {
            amqp_buffer_t *buffer;
            amqp_type_t *container;
        } encode;

        struct
        {
            // TODO - move into config
            uint32_t max_frame_size;
            uint16_t channel_max;
            uint32_t idle_time_out;
        } limits;

        struct
        {
            int cloned;
            amqp_mutex_t *output_mutex;
        } shared;
        struct
        {
            amqp_hash_table_t *amqp_descriptors;
            struct
            {
                amqp_sasl_plugin_node_t *sasl_plugin_list;
                amqp_messaging_methods_t messaging;
            } plugins;
            const char *container_id;
        } reference;

        struct
        {
            amqp_sasl_identity_t identity_hooks;
        } sasl;

        amqp_event_loop_t *thread_event_loop;
    };

    extern amqp_context_t *amqp_create_context();
    extern amqp_context_t *amqp_context_clone(amqp_context_t *context);
    extern int amqp_context_destroy(amqp_context_t *context);

    extern amqp_type_t *amqp__allocate_type(amqp_context_t *context AMQP_DEBUG_PARAMS_DECL);
    extern void amqp_deallocate_type(amqp_context_t *context, amqp_type_t *type);

    extern amqp_buffer_t *amqp__allocate_buffer(amqp_context_t *context AMQP_DEBUG_PARAMS_DECL);
    extern void amqp_deallocate_buffer(amqp_context_t *context, amqp_buffer_t *type);

    extern amqp_frame_t *amqp__allocate_frame(amqp_context_t *context AMQP_DEBUG_PARAMS_DECL);
    extern void amqp_deallocate_frame(amqp_context_t *context, amqp_frame_t *type);

    extern amqp_debug_print_c_t *amqp_context_define_putc_function(amqp_context_t *context, amqp_debug_print_c_t *putc);
    extern int amqp_context_printf(amqp_context_t *context, int level, const char *format, ...);
    extern int amqp_context_putc(amqp_context_t *context, int c);
    extern int amqp_context_increase_print_indent(amqp_context_t *context, int delta);
    extern int amqp_context_set_print_indent(amqp_context_t *context, int indent);
    extern int amqp_context_set_auto_indent(amqp_context_t *context, int amount);

    extern void amqp_buffer_outputter(amqp_context_t *context, amqp_outputter_arg_t *dest, const char *filename, int line_number, const char *context_name, const char *label, const char *source, const char *extra, const char *format, va_list args);
    extern void amqp_stream_outputter(amqp_context_t *context, amqp_outputter_arg_t *dest, const char *filename, int line_number, const char *context_name, const char *label, const char *source, const char *extra, const char *format, va_list args);
    extern void amqp_restore_outputter(amqp_context_t *context);
    extern void amqp_output_to_buffer(amqp_context_t *context, char *buffer, size_t buffer_size);

    extern void amqp_context_set_name(amqp_context_t *context, const char *name);

    extern void amqp_context_register_sasl_plugin(amqp_context_t *context, amqp_sasl_plugin_t *sasl_plugin);
    extern amqp_sasl_plugin_t *amqp_context_lookup_sasl_plugin(amqp_context_t *context, amqp_symbol_t *mechanism);
    extern void amqp_context_free_sasl_plugins(amqp_context_t *context);
    extern amqp_sasl_plugin_t *amqp_context_first_sasl_plugin(amqp_context_t *context, amqp_sasl_plugin_node_t **sasl_plugin_node);
    extern amqp_sasl_plugin_t *amqp_context_next_sasl_plugin(amqp_context_t *context, amqp_sasl_plugin_node_t **sasl_plugin_node);
    extern int amqp_context_sasl_plugin_count(amqp_context_t *context);
    extern void amqp_context_register_identity_hooks(amqp_context_t *context, amqp_identity_hook_method_t provide_login, amqp_identity_hook_method_t provide_password, amqp_identity_hook_method_t provide_email);

    extern uint8_t *amqp_allocate_print_buffer(amqp_context_t *context, size_t n);
    extern void amqp_deallocate_print_buffer(amqp_context_t *context, uint8_t *buffer);

    extern void amqp_outputter_lock(amqp_context_t *context);
    extern void amqp_outputter_unlock(amqp_context_t *context);

    static inline
    amqp_descriptor_t *amqp_context_descriptor_lookup(amqp_context_t *context, amqp_symbol_t *symbol)
    {
        return amqp_descriptor_lookup(context->reference.amqp_descriptors, symbol);
    }

#ifdef __cplusplus
}
#endif
#endif

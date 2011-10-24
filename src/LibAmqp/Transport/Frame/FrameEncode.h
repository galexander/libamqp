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

#ifndef LIBAMQP_TRANSPORT_FRAME_FRAME_ENCODE_H
#define LIBAMQP_TRANSPORT_FRAME_FRAME_ENCODE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Transport/Frame/Frame.h"
#include "Plugin/Sasl.h"

extern void amqp_encode_sasl_mechanisms_frame(amqp_context_t *context, amqp_buffer_t *buffer);
extern void amqp_encode_sasl_init_frame(amqp_context_t *context, amqp_buffer_t *buffer, amqp_sasl_plugin_t *sasl_plugin);

#ifdef __cplusplus
}
#endif
#endif
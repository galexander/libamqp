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

#ifndef LIBAMQP_TRANSPORT_AMQP_AMQP_FRAMES_H
#define LIBAMQP_TRANSPORT_AMQP_AMQP_FRAMES_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LIBAMQP_BUFFER_T_TYPE
#define LIBAMQP_BUFFER_T_TYPE
typedef struct amqp_buffer_t amqp_buffer_t;
#endif

#ifndef LIBAMQP_AMQP_CONTEXT_TYPE_T
#define LIBAMQP_AMQP_CONTEXT_TYPE_T
typedef struct amqp_context_t amqp_context_t;
#endif

#ifndef LIBAMQP_AMQP_TYPE_T
#define LIBAMQP_AMQP_TYPE_T
typedef struct amqp_type_t amqp_type_t;
#endif

#ifndef LIBAMQP_AMQP_CONNECTION_TYPE_T
#define LIBAMQP_AMQP_CONNECTION_TYPE_T
typedef struct amqp_connection_t amqp_connection_t;
#endif

#ifndef LIBAMQP_AMQP_SESSION_TYPE_T
#define LIBAMQP_AMQP_SESSION_TYPE_T
typedef struct amqp_session_t amqp_session_t;
#endif

#ifndef LIBAMQP_AMQP_FRAME_TYPE_T
#define LIBAMQP_AMQP_FRAME_TYPE_T
typedef struct amqp_frame_t amqp_frame_t;
#endif


extern int amqp_send_amqp_open_frame(amqp_connection_t *connection);

extern int amqp_send_empty_frame(amqp_connection_t *connection);
extern int amqp_send_amqp_close_frame(amqp_connection_t *connection);
extern int amqp_process_close_frame(amqp_connection_t *connection, amqp_frame_t *frame);

extern int amqp_process_open_frame_from_client(amqp_connection_t *connection, amqp_frame_t *frame);
extern int amqp_process_open_frame_from_broker(amqp_connection_t *connection, amqp_frame_t *frame);

extern int amqp_send_amqp_begin_frame(amqp_connection_t *connection, amqp_session_t *session);
extern int amqp_process_begin_frame(amqp_connection_t *connection, amqp_frame_t *frame);


#ifdef __cplusplus
}
#endif
#endif

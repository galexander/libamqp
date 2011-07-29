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

#ifndef LIBAMQP_THREAD_MUTEX_H
#define LIBAMQP_THREAD_MUTEX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Thread/Thread.h"

typedef struct amqp_mutex_t
{
#if defined(AMQP__WIN32_THREADS)
    #error "no pthreads"
#else
    pthread_mutex_t mutex;
#endif
} amqp_mutex_t;

extern void amqp_mutex_initialize(amqp_mutex_t *mutex);
extern void amqp_mutex_destroy(amqp_mutex_t *mutex);
extern void amqp_mutex_lock(amqp_mutex_t *mutex);
extern void amqp_mutex_unlock(amqp_mutex_t *mutex);

#ifdef __cplusplus
}
#endif
#endif

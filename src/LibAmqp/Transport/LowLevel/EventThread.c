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
#include "Transport/LowLevel/EventThread.h"

static void event_thread_handler(void *argument)
{
    amqp_event_thread_t *event_thread  = (amqp_event_thread_t *) argument;
    amqp_event_loop_t *private_loop = 0;

    if (event_thread->loop == 0)
    {
        event_thread->loop = private_loop = ev_loop_new(0);
    }

    event_thread->context->thread_event_loop = event_thread->loop; // TODO - Inclined to forget this and errors are not obvious

    (*event_thread->handler)(event_thread);

    if (private_loop != 0)
    {
        ev_loop_destroy(private_loop);
    }

    amqp_semaphore_signal(&event_thread->thread_running_semaphore);
}

void async_watcher_callback(struct ev_loop *loop, ev_async *watcher, int flags)
{
    ev_break(loop, EVBREAK_ALL);
}

void amqp_event_thread_run_loop(amqp_event_thread_t *event_thread)
{
    ev_async *async_watcher = &event_thread->async_watcher;

    ev_async_init(async_watcher, async_watcher_callback);
    ev_async_start(event_thread->loop, async_watcher);

    amqp_semaphore_signal(&event_thread->thread_running_semaphore);
    ev_run(event_thread->loop, 0);
    ev_async_stop(event_thread->loop, async_watcher);
}

void amqp_event_thread_run_loop_with_shutdown_hook(amqp_event_thread_t *event_thread, amqp_connection_shutdown_hook_t hook, amqp_accept_handler_arguments_t *argument)
{
    assert(hook != 0);

    amqp_event_thread_run_loop(event_thread);
    (*hook)(event_thread->context, event_thread->loop, argument);
}

amqp_event_thread_t *amqp_event_thread_initialize(amqp_context_t *context, amqp_event_thread_handler_t handler, amqp_event_loop_t *loop, void *thread_arguments, const char *context_name)
{
    amqp_event_thread_t *result = AMQP_MALLOC(context, amqp_event_thread_t);
    amqp_semaphore_initialize(&result->thread_running_semaphore);

    result->handler = handler;
    result->loop = loop;
    result->thread_arguments = thread_arguments;
    result->context = amqp_context_clone(context);

    amqp_context_set_name(result->context, context_name);

    result->thread = amqp_thread_start(event_thread_handler, result);

    amqp_semaphore_wait(&result->thread_running_semaphore);
    return result;
}

static void send_break_request(amqp_event_thread_t *event_thread)
{
    ev_async *async_watcher = &event_thread->async_watcher;
    ev_async_send(event_thread->loop, async_watcher);
}

int amqp_event_thread_destroy(amqp_context_t *context, amqp_event_thread_t *event_thread)
{
    int allocations_ok = true;
    if (event_thread != 0)
    {
        send_break_request(event_thread);
        amqp_semaphore_wait(&event_thread->thread_running_semaphore);

        amqp_thread_destroy(event_thread->thread);
        allocations_ok = amqp_context_destroy(event_thread->context);

        amqp_semaphore_destroy(&event_thread->thread_running_semaphore);
        AMQP_FREE(context, event_thread);
    }
    return allocations_ok;
}
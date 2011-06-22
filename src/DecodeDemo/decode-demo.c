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
#include <assert.h>
//#include <stdio.h>

#include "Codec/Codec.h"

int main(int argc, char *argv[])
{
    int c;
    amqp_type_t *type;
    amqp_context_t *context = amqp_create_context();

    while ((c = getc(stdin)) != -1)
    {
        amqp_buffer_putc(context->decode.buffer, c);
    }

    type = amqp_decode(context);
    amqp_type_print_formatted(type);

    amqp_deallocate_type(context, type);
    amqp_destroy_context(context);

    assert(amqp_assert_that_calls_to_free_match_calls_to_malloc());

    return 0;
}

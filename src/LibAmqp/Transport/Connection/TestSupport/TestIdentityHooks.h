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

#ifndef LIBAMQP_TRANSPORT_CONNECTION_TEST_SUPPORT_TEST_USER_H
#define LIBAMQP_TRANSPORT_CONNECTION_TEST_SUPPORT_TEST_USER_H

#include "Context/Context.h"

namespace TestSupport
{
    class TestIdentityHooks
    {
    private:
        TestIdentityHooks();
        ~TestIdentityHooks();

    public:
        static const char *provide_login(amqp_context_t *context);
        static const char *provide_password(amqp_context_t *context);
        static const char *provide_email(amqp_context_t *context);
    };
}
#endif

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

#include <TestHarness.h>
#include "Transport/Frame/FrameTestFixture.h"
#include "Plugin/SaslPlain/SaslPlain.h"
#include "debug_helper.h"

namespace SuiteFrame
{
    FrameTestFixture::FrameTestFixture() 
    {
    }

    FrameTestFixture::~FrameTestFixture()
    {
    }

    int FrameTestFixture::check_header(uint8_t frame_type, uint16_t frame_specific)
    {
        if ((8U == frame->data_offset) && (frame_type == frame->frame_type) && (frame_specific == frame->type_specific.word))
        {
            return true;
        }

        SOUTV(frame->data_offset);
        SOUTV(frame->frame_type);
        SOUTV(frame->type_specific.word);
        return false;
    }

    int FrameTestFixture::check_sasl_header()
    {
        return check_header(AMQP_SASL_FRAME_TYPE, 0U);
    }

    int FrameTestFixture::check_amqp_header()
    {
        return check_header(AMQP_FRAME_TYPE, 0U);
    }
}

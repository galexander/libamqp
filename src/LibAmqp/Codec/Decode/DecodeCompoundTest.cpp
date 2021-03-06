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
#include "Context/ErrorHandling.h"

#include "Codec/Decode/DecodeTestFixture.h"

#include "Codec/Decode/Decode.h"
#include "Codec/Type/Type.h"

#include "debug_helper.h"

SUITE(CodecDecode)
{
    TEST_FIXTURE(DecodeTestFixture, SmallArray)
    {
        load_decode_buffer(test_data::array_shorts);
        type = amqp_decode(context, decode_buffer);

        ASSERT_VALID(type);

        CHECK_EQUAL(0xe0, type->constructor.format_code);

        CHECK_ARRAY(type);

        CHECK_EQUAL((size_t) 0x02, type->position.index);
        CHECK_EQUAL((size_t) 0x0c, type->position.size);

        CHECK_NOT_NULL(type->value.array.elements);
        CHECK_NOT_NULL(type->value.array.elements[0]);
        CHECK_EQUAL(0x61, type->value.array.elements[0]->constructor.format_code);

        CHECK_EQUAL((size_t) 5, type->value.array.count);
        for (int i = 0; i < 5; i++)
        {
            amqp_type_t *e = type->value.array.elements[i];
            ASSERT_VALID(e);
            CHECK(amqp_type_is_short(e));
            CHECK(amqp_type_is_contained(e));
        }

        CHECK_EQUAL((size_t) 0x04, type->value.array.elements[0]->position.index);
        CHECK_EQUAL((size_t) 0x02, type->value.array.elements[0]->position.size);
    }

    TEST_FIXTURE(DecodeTestFixture, LargeList)
    {
        load_decode_buffer(test_data::list);
        type = amqp_decode(context, decode_buffer);

        ASSERT_VALID(type);

        CHECK_EQUAL(0xd0, type->constructor.format_code);
        CHECK_LIST(type);
        CHECK_EQUAL((size_t) 6, type->value.list.count);

        amqp_type_t *e;

        e  = amqp_type_list_element(type, 0);
        CHECK_EQUAL(0xa3, e->constructor.format_code);
        CHECK(amqp_type_is_symbol(e));
        CHECK(amqp_type_is_contained(e));
        result = amqp_convert_bytes_to_cstr(context, e);
        CHECK_EQUAL("Foo", result);
        AMQP_FREE(context, result);

        e  = amqp_type_list_element(type, 1);
        CHECK_EQUAL(0x82, e->constructor.format_code);
        CHECK(amqp_type_is_double(e));
        CHECK(amqp_type_is_contained(e));
        CHECK_CLOSE(123.456, amqp_type_to_double(e), 0.0001);

        e  = amqp_type_list_element(type, 2);
        CHECK_EQUAL(0xa1, e->constructor.format_code);
        CHECK(amqp_type_is_string(e));
        CHECK(amqp_type_is_contained(e));
        result = amqp_convert_bytes_to_cstr(context, e);
        CHECK_EQUAL("Hello", result);
        AMQP_FREE(context, result);

        e  = amqp_type_list_element(type, 3);
        CHECK_EQUAL(0x61, e->constructor.format_code);
        CHECK(amqp_type_is_short(e));
        CHECK(amqp_type_is_contained(e));
        CHECK_EQUAL(10, amqp_type_to_short(e));

        e  = amqp_type_list_element(type, 4);
        CHECK_EQUAL(0xe0, e->constructor.format_code);
        CHECK_ARRAY(e);
        CHECK(amqp_type_is_contained(e));
        CHECK_EQUAL((size_t) 2, e->value.compound.count);

        e  = amqp_type_list_element(type, 5);
        CHECK_EQUAL(0xe0, e->constructor.format_code);
        CHECK_ARRAY(e);
        CHECK(amqp_type_is_contained(e));
        CHECK_EQUAL((size_t) 1, e->value.compound.count);
    }

    TEST_FIXTURE(DecodeTestFixture, leak)
    {
        load_decode_buffer(test_data::list);
        type = amqp_decode(context, decode_buffer);
    }

    TEST_FIXTURE(DecodeTestFixture, Map)
    {
        load_decode_buffer(test_data::map);
        type = amqp_decode(context, decode_buffer);

        ASSERT_VALID(type);
        CHECK_EQUAL(0xc1, type->constructor.format_code);
        CHECK_MAP(type);
        CHECK_EQUAL((size_t) 10, type->value.map.count); // number of entries not elements

        CHECK_EQUAL(0xa1, amqp_type_map_element(type, 0)->constructor.format_code);
        CHECK_EQUAL(0xc0, amqp_type_map_element(type, 1)->constructor.format_code);
        CHECK_EQUAL(0x40, amqp_type_map_element(type, 2)->constructor.format_code);
        CHECK_EQUAL(0x41, amqp_type_map_element(type, 3)->constructor.format_code);
        CHECK_EQUAL(0xa1, amqp_type_map_element(type, 4)->constructor.format_code);
        CHECK_EQUAL(0x82, amqp_type_map_element(type, 5)->constructor.format_code);
        CHECK_EQUAL(0xa1, amqp_type_map_element(type, 6)->constructor.format_code);
        CHECK_EQUAL(0x55, amqp_type_map_element(type, 7)->constructor.format_code);
        CHECK_EQUAL(0xa1, amqp_type_map_element(type, 8)->constructor.format_code);
        CHECK_EQUAL(0x81, amqp_type_map_element(type, 9)->constructor.format_code);
    }

    TEST_FIXTURE(DecodeTestFixture, arrray_elements_are_contained)
    {
        load_decode_buffer(test_data::array_shorts);
        type = amqp_decode(context, decode_buffer);
        ASSERT_VALID(type);
        CHECK(amqp_type_is_contained(type->value.array.elements[0]));
        CHECK(amqp_type_is_contained(type->value.array.elements[1]));
        CHECK(amqp_type_is_contained(type->value.array.elements[2]));
        CHECK(amqp_type_is_contained(type->value.array.elements[3]));
        CHECK(amqp_type_is_contained(type->value.array.elements[4]));
    }

    TEST_FIXTURE(DecodeTestFixture, empty_array_of_symbols)
    {
        test_data::empty_array_of_symbols.transfer_to(decode_buffer);
        type = amqp_decode(context, decode_buffer);

        CHECK(amqp_type_is_array(type));
        CHECK_EQUAL(0U, type->value.array.count);

        // TODO - check element type even
    }

    TEST_FIXTURE(DecodeTestFixture, list_elements_are_contained)
    {
        load_decode_buffer(test_data::list);
        type = amqp_decode(context, decode_buffer);
        ASSERT_VALID(type);
        CHECK(amqp_type_is_contained(amqp_type_list_element(type, 0)));
        CHECK(amqp_type_is_contained(amqp_type_list_element(type, 1)));
        CHECK(amqp_type_is_contained(amqp_type_list_element(type, 2)));
        CHECK(amqp_type_is_contained(amqp_type_list_element(type, 3)));
        CHECK(amqp_type_is_contained(amqp_type_list_element(type, 4)));
    }

    TEST_FIXTURE(DecodeTestFixture, map_entries_are_contained)
    {
        load_decode_buffer(test_data::map);
        type = amqp_decode(context, decode_buffer);
        ASSERT_VALID(type);
        CHECK(amqp_type_is_contained(amqp_type_map_element(type, 0)));
        CHECK(amqp_type_is_contained(amqp_type_map_element(type, 1)));
        CHECK(amqp_type_is_contained(amqp_type_map_element(type, 2)));
        CHECK(amqp_type_is_contained(amqp_type_map_element(type, 3)));
        CHECK(amqp_type_is_contained(amqp_type_map_element(type, 4)));
        CHECK(amqp_type_is_contained(amqp_type_map_element(type, 5)));
        CHECK(amqp_type_is_contained(amqp_type_map_element(type, 6)));
        CHECK(amqp_type_is_contained(amqp_type_map_element(type, 7)));
        CHECK(amqp_type_is_contained(amqp_type_map_element(type, 8)));
        CHECK(amqp_type_is_contained(amqp_type_map_element(type, 9)));
    }

    TEST_FIXTURE(DecodeTestFixture, described_list)
    {
        load_decode_buffer(test_data::described_list);

        type = amqp_decode(context, decode_buffer);
        ASSERT_VALID(type);

        CHECK(amqp_type_is_composite(type));
        CHECK(!amqp_type_is_descriptor(type));
        CHECK(!amqp_type_is_described(type));

        CHECK_EQUAL((size_t) 2, type->value.described.count);

        amqp_type_t *descriptor = type->value.described.elements[0];
        amqp_type_t *list = type->value.described.elements[1];

        CHECK(!amqp_type_is_composite(descriptor));
        CHECK(amqp_type_is_descriptor(descriptor));
        CHECK(!amqp_type_is_described(descriptor));

        CHECK(!amqp_type_is_composite(list));
        CHECK(!amqp_type_is_descriptor(list));
        CHECK(amqp_type_is_described(list));

        CHECK_LIST(list);

        CHECK(amqp_type_is_composite(list->value.list.elements[1]));
        CHECK(!amqp_type_is_descriptor(list->value.list.elements[1]));
        CHECK(!amqp_type_is_described(list->value.list.elements[1]));

        CHECK(amqp_type_is_descriptor(list->value.list.elements[1]->value.described.elements[0]));
        CHECK(amqp_type_is_described(list->value.list.elements[1]->value.described.elements[1]));

        CHECK(amqp_type_is_contained(list->value.list.elements[1]->value.described.elements[0]));
        CHECK(amqp_type_is_contained(list->value.list.elements[1]->value.described.elements[1]));

    }

    TEST_FIXTURE(DecodeTestFixture, missing_descriptor)
    {
        context->debug.level = 0;
        load_decode_buffer(test_data::missing_descriptor);
        type = amqp_decode(context, decode_buffer);
        ASSERT_INVALID(type);
        CHECK_EQUAL(AMQP_ERROR_NO_DESCRIPTOR, type->invalid_cause);
    }

    TEST_FIXTURE(DecodeTestFixture, missing_described_type)
    {
        context->debug.level = 0;
        load_decode_buffer(test_data::missing_described_type);
        type = amqp_decode(context, decode_buffer);
        ASSERT_INVALID(type);
        CHECK_EQUAL(AMQP_ERROR_NO_DESCRIBED_TYPE, type->invalid_cause);
    }

    TEST_FIXTURE(DecodeTestFixture, invalid_descriptor)
    {
        context->debug.level = 0;
        load_decode_buffer(test_data::invalid_descriptor);
        type = amqp_decode(context, decode_buffer);
        ASSERT_INVALID(type);
        CHECK_EQUAL(AMQP_ERROR_DESCRIPTOR_INVALID, type->invalid_cause);
    }

    TEST_FIXTURE(DecodeTestFixture, invalid_described_type)
    {
        context->debug.level = 0;
        load_decode_buffer(test_data::invalid_described_type);
        type = amqp_decode(context, decode_buffer);
        ASSERT_INVALID(type);
        CHECK_EQUAL(AMQP_ERROR_DESCRIBED_INVALID, type->invalid_cause);
    }

    TEST_FIXTURE(DecodeTestFixture, empty_map)
    {
        load_decode_buffer(test_data::empty_map);
        type = amqp_decode(context, decode_buffer);

        ASSERT_VALID(type);
        CHECK_EQUAL(0xc1, type->constructor.format_code);
        CHECK_MAP(type);
        CHECK_EQUAL((size_t) 0, type->value.map.count); // number of entries not elements
    }

    TEST_FIXTURE(DecodeTestFixture, empty_list_8)
    {
        load_decode_buffer(test_data::empty_list_8);
        type = amqp_decode(context, decode_buffer);
        ASSERT_VALID(type);

        CHECK_EQUAL(0xc0, type->constructor.format_code);
        CHECK_LIST(type);
        CHECK_EQUAL((size_t) 0, type->value.list.count);
    }

    TEST_FIXTURE(DecodeTestFixture, empty_list_0)
    {
        load_decode_buffer(test_data::empty_list_0);
        type = amqp_decode(context, decode_buffer);
        ASSERT_VALID(type);

        CHECK_EQUAL(0x45, type->constructor.format_code);
        CHECK_LIST(type);
        CHECK_EQUAL((size_t) 0, type->value.list.count);
    }
}
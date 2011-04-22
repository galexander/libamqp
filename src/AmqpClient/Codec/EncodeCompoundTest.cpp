#include "TestHarness.h"
#include "ErrorHandling/ErrorHandling.h"

#include "Codec/CodecTestSupport.h"
#include "Codec/Decode.h"
#include "Type/Type.h"
#include "Type/TypePrint.h"

#include <stdio.h>


namespace
{
    t::Context *c;
    amqp_type_t *type;
    char *result;
    amqp_context_t *context;

    void SetUp()
    {
        c = new t::Context();
        type = 0;
        result = 0;
        context = c->context();
    }

    void TearDown()
    {
        AMQP_FREE(result);
        AMQP_DEALLOC(amqp_type_t, c->context(), type);
        delete c;
    }
}

TEST(EncodeCompound, single_element_array)
{
    amqp_type_t *t;

    type = amqp_encode_array_8(context);
    ASSERT_INVALID(type);
    CHECK_ARRAY(type);

    t = amqp_encode_short(context, 10);
    ASSERT(amqp_type_is_contained(t));

    amqp_complete_type(context, type);
    ASSERT_VALID(type);

    CHECK_EQUAL((size_t) 1, (size_t) type->value.array.count)
    ASSERT_NOT_NULL(type->value.array.elements)

    CHECK_EQUAL((size_t) 2, type->position.index);
    CHECK_EQUAL((size_t) 4, type->position.size);

    CHECK_EQUAL(0x61, type->value.array.elements[0]->format_code);

    CHECK_EQUAL(1U, type->flags.is_encoded);
    CHECK_EQUAL(1U, type->value.array.elements[0]->flags.is_encoded);

    ASSERT_BUFFERS_MATCH(context->encode.buffer, test_data::single_element_array);
}

TEST(EncodeCompound, short_array)
{
    type = amqp_encode_array_8(context);
    ASSERT_NOT_NULL(type);
    ASSERT_INVALID(type);
    CHECK_ARRAY(type);

    ASSERT_NULL(type->value.array.elements)

    amqp_encode_short(context, 10);
    amqp_encode_short(context, 11);
    amqp_encode_short(context, 11);
    amqp_encode_short(context, 13);
    amqp_encode_short(context, 14);

    amqp_complete_type(context, type);
    ASSERT_VALID(type);

    ASSERT_NOT_NULL(type->value.array.elements)
    CHECK_EQUAL((size_t) 5, (size_t) type->value.array.count)
    CHECK_EQUAL((size_t) 2, type->position.index);
    CHECK_EQUAL((size_t) 12, type->position.size);

    CHECK_EQUAL(0x61, type->value.array.elements[0]->format_code);

    ASSERT_BUFFERS_MATCH(context->encode.buffer, test_data::array_shorts);
}

// mixing elements will mark the type as invalid and will leave the buffer in an undefined state.
TEST(EncodeCompound, invalid_array_with_mixed_types)
{
    type = amqp_encode_array_8(context);

    ASSERT_INVALID(type);
    CHECK_ARRAY(type);
    ASSERT_NULL(type->value.array.elements)

    amqp_encode_short(context, 10);
    amqp_encode_string_utf8(context, "hello");

    amqp_complete_type(context, type);
    ASSERT_INVALID(type);

    CHECK_EQUAL((size_t) 2, (size_t) type->value.array.count)

    CHECK_EQUAL(0x61, type->value.array.elements[0]->format_code);
    CHECK_EQUAL(0xa1, type->value.array.elements[1]->format_code);
}

// mixing elements will mark the type as invalid and will leave the buffer in an undefined state.
TEST(EncodeCompound, array_of_strings)
{
    type = amqp_encode_array_8(context);
    ASSERT_INVALID(type);
    CHECK_ARRAY(type);

    ASSERT_NULL(type->value.array.elements)

    amqp_encode_string_utf8(context, "the");
    amqp_encode_string_utf8(context, "little");
    amqp_encode_string_utf8(context, "brown");
    amqp_encode_string_utf8(context, "fox");

    amqp_complete_type(context, type);
    ASSERT_VALID(type);

    ASSERT_NOT_NULL(type->value.array.elements)
    CHECK_EQUAL((size_t) 4, (size_t) type->value.array.count)

    CHECK_EQUAL(0xa1, type->value.array.elements[0]->format_code);
    CHECK_EQUAL(0xa1, type->value.array.elements[1]->format_code);
    CHECK_EQUAL(0xa1, type->value.array.elements[2]->format_code);
    CHECK_EQUAL(0xa1, type->value.array.elements[3]->format_code);

    ASSERT_BUFFERS_MATCH(context->encode.buffer, test_data::string_array);
}

TEST(EncodeCompound, amqp_encode_list_8_of_shorts)
{
    type = amqp_encode_list_8(context);
    ASSERT_INVALID(type);

    CHECK_LIST(type);
    ASSERT_NULL(type->value.list.elements)

    amqp_encode_short(context, 10);
    amqp_encode_short(context, 11);
    amqp_encode_short(context, 11);
    amqp_encode_short(context, 13);
    amqp_encode_short(context, 14);

    amqp_complete_type(context, type);
    ASSERT_VALID(type);

    CHECK_EQUAL((size_t) 5, (size_t) type->value.list.count)
    CHECK_EQUAL((size_t) 2, type->position.index);
    CHECK_EQUAL((size_t) 16, type->position.size);

    ASSERT_BUFFERS_MATCH(context->encode.buffer, test_data::list_of_shorts);
}

TEST(EncodeCompound, amqp_encode_list_32)
{
    amqp_type_t *l;

    type = amqp_encode_list_32(context);
    ASSERT_INVALID(type);
    CHECK_LIST(type);
    ASSERT_NULL(type->value.list.elements)

    amqp_encode_symbol(context, "Foo");
    amqp_encode_double(context, 123.456);
    amqp_encode_string_utf8(context, "Hello");
    amqp_encode_short(context, 10);
        l = amqp_encode_array_8(context);
            amqp_encode_short(context, 10);
            amqp_encode_short(context, 11);
        amqp_complete_type(context, l);

        l = amqp_encode_array_8(context);
            amqp_encode_short(context, 12);
        amqp_complete_type(context, l);
    amqp_complete_type(context, type);

    ASSERT_VALID(type);
    CHECK_LIST(type);
    ASSERT_NOT_NULL(type->value.list.elements);

    CHECK_EQUAL((size_t) 6, (size_t) type->value.list.count)
    CHECK_EQUAL((size_t) 5, type->position.index);
    CHECK_EQUAL((size_t) 42, type->position.size);

    ASSERT_BUFFERS_MATCH(context->encode.buffer, test_data::list);
}

TEST(EncodeCompound, amqp_encode_map_8)
{
    amqp_type_t *l;

    type = amqp_encode_map_8(context);
    ASSERT_INVALID(type);

        amqp_encode_string_utf8(context, "list:");
        l = amqp_encode_list_8(context);
            amqp_encode_long(context, 1);
            amqp_encode_string_utf8(context, "two");
            amqp_encode_double(context, 3.141593);
            amqp_encode_null(context);
            amqp_encode_boolean(context, false);
        amqp_complete_type(context, l);

        ASSERT_VALID(l);
        CHECK_LIST(l);
        CHECK_EQUAL((size_t) 5, (size_t) l->value.list.count)
        CHECK_EQUAL((size_t) 12, l->position.index);
        CHECK_EQUAL((size_t) 26, l->position.size);


        amqp_encode_null(context);
        amqp_encode_boolean(context, true);

        amqp_encode_string_utf8(context, "pi");
        amqp_encode_double(context, 3.141593);

        amqp_encode_string_utf8(context, "two");
        amqp_encode_long(context, 2);

        amqp_encode_string_utf8(context, "one");
        amqp_encode_long(context, 1);

    amqp_complete_type(context, type);
    ASSERT_VALID(type);

    CHECK_MAP(type);
    ASSERT_NOT_NULL(type->value.map.entries);

    CHECK_EQUAL((size_t) 10, (size_t) type->value.map.count)
    CHECK_EQUAL((size_t) 2, type->position.index);
    CHECK_EQUAL((size_t) 79, type->position.size);

    ASSERT_BUFFERS_MATCH(context->encode.buffer, test_data::map);
}

TEST(EncodeCompound, empty_map)
{
    type = amqp_encode_map_8(context);
    ASSERT_INVALID(type);
    amqp_complete_type(context, type);
    ASSERT_VALID(type);

    CHECK_MAP(type);
    ASSERT_NULL(type->value.map.entries);

    CHECK_EQUAL((size_t) 0, (size_t) type->value.map.count)

    ASSERT_BUFFERS_MATCH(context->encode.buffer, test_data::empty_map);
}

TEST(EncodeCompound, empty_list_8)
{
    type = amqp_encode_list_8(context);
    ASSERT_INVALID(type);
    amqp_complete_type(context, type);
    ASSERT_VALID(type);

    CHECK_LIST(type);
    ASSERT_NULL(type->value.list.elements);
    CHECK_EQUAL((size_t) 0, (size_t) type->value.list.count)

    ASSERT_BUFFERS_MATCH(context->encode.buffer, test_data::empty_list_8);
}

static amqp_type_t *encode_described_list(amqp_context_t *context)
{
    amqp_type_t *result = amqp_start_encode_described_type(context);
        amqp_encode_string_utf8(context, "List");
        {
            amqp_type_t *list = amqp_encode_list_8(context);
            amqp_encode_string_utf8(context, "Pie");
            {
                amqp_type_t *pi = amqp_start_encode_described_type(context);
                amqp_encode_string_utf8(context, "PI");
                amqp_encode_double(context, 3.141593);
                amqp_complete_type(context, pi);
            }
            amqp_encode_short(context, 10);
            amqp_complete_type(context, list);
        }
    amqp_complete_type(context, result);
    return result;
}

TEST(EncodeCompound, described_list)
{
    type = encode_described_list(context);

    ASSERT_VALID(type);
    ASSERT_BUFFERS_MATCH(context->encode.buffer, test_data::described_list);
}

TEST(EncodeCompound, described_list_structure)
{
    type = encode_described_list(context);

    ASSERT_VALID(type);

    CHECK(amqp_type_is_described(type));
    CHECK(!amqp_type_is_descriptor(type));
    CHECK(!amqp_type_has_descriptor(type));

    ASSERT_EQUAL((size_t) 2, type->value.described.count);

    amqp_type_t *descriptor = type->value.described.elements[0];
    amqp_type_t *list = type->value.described.elements[1];

    CHECK(!amqp_type_is_described(descriptor));
    CHECK(amqp_type_is_descriptor(descriptor));
    CHECK(!amqp_type_has_descriptor(descriptor));

    CHECK(!amqp_type_is_described(list));
    CHECK(!amqp_type_is_descriptor(list));
    CHECK(amqp_type_has_descriptor(list));

    CHECK_LIST(list);

    CHECK(amqp_type_is_described(list->value.list.elements[1]));
    CHECK(!amqp_type_is_descriptor(list->value.list.elements[1]));
    CHECK(!amqp_type_has_descriptor(list->value.list.elements[1]));

    CHECK(amqp_type_is_descriptor(list->value.list.elements[1]->value.described.elements[0]));
    CHECK(amqp_type_has_descriptor(list->value.list.elements[1]->value.described.elements[1]));

    CHECK(amqp_type_is_contained(list->value.list.elements[1]->value.described.elements[0]));
    CHECK(amqp_type_is_contained(list->value.list.elements[1]->value.described.elements[1]));
}
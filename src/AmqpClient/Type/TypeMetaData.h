#ifndef STORMMQ_AMQP_TYPE_META_DATA_H
#define STORMMQ_AMQP_TYPE_META_DATA_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>
#include <stdlib.h>

#ifndef AMQP_CONTEXT_T_TYPE
#define AMQP_CONTEXT_T_TYPE
typedef struct amqp_context_t amqp_context_t;
#endif

#ifndef AMQP_TYPE_T
#define AMQP_TYPE_T
typedef struct amqp_type_t amqp_type_t;
#endif

typedef struct amqp_type_meta_data_t amqp_type_meta_data_t;
typedef int amqp_decoder_t(amqp_type_meta_data_t *, amqp_type_t *);

typedef struct amqp_type_methods_t amqp_type_methods_t;
typedef void amqp_type_print_method_t(amqp_context_t *context, amqp_type_t *type);
struct amqp_type_methods_t
{
    amqp_type_print_method_t *print;
};

struct amqp_type_meta_data_t
{
    unsigned char format_code;
    int width;
    amqp_decoder_t *type_decoder;
    amqp_type_methods_t *methods;
    const char *name;
    const char *encoding_name;
    int category_name_index;
};

typedef struct amqp_type_description_t amqp_type_description_t;
struct amqp_type_description_t
{
    size_t size;
    size_t overhead;
    amqp_type_meta_data_t *meta_data;
};

typedef struct amqp_type_encode_meta_data_t amqp_type_encode_meta_data_t;
typedef int amqp_describe_type_t(amqp_context_t *context, amqp_type_description_t *description, va_list ap);
typedef amqp_type_t *amqp_encode_caller_t(amqp_context_t *context, amqp_type_description_t *description, va_list ap);

struct amqp_type_encode_meta_data_t
{
    amqp_describe_type_t *describe;
    amqp_encode_caller_t *encode;
};

#define AMQP_TYPE_CATEGORY_FIXED        0
#define AMQP_TYPE_CATEGORY_VARIABLE     1
#define AMQP_TYPE_CATEGORY_COMPOUND     2
#define AMQP_TYPE_CATEGORY_ARRAY        3

extern const char *amqp_category_names[];

extern amqp_type_meta_data_t amqp_type_meta_data_described;
extern amqp_type_meta_data_t amqp_type_meta_data_null;
extern amqp_type_meta_data_t amqp_type_meta_data_boolean_true;
extern amqp_type_meta_data_t amqp_type_meta_data_boolean_false;
extern amqp_type_meta_data_t amqp_type_meta_data_extension_fixed_0;

extern amqp_type_meta_data_t amqp_type_meta_data_ubyte;
extern amqp_type_meta_data_t amqp_type_meta_data_byte;
extern amqp_type_meta_data_t amqp_type_meta_data_uint_small_uint;
extern amqp_type_meta_data_t amqp_type_meta_data_ulong_small_ulong;
extern amqp_type_meta_data_t amqp_type_meta_data_int_small_int;
extern amqp_type_meta_data_t amqp_type_meta_data_long_small_long;
extern amqp_type_meta_data_t amqp_type_meta_data_extension_fixed_1;

extern amqp_type_meta_data_t amqp_type_meta_data_ushort;
extern amqp_type_meta_data_t amqp_type_meta_data_short;
extern amqp_type_meta_data_t amqp_type_meta_data_extension_fixed_2;

extern amqp_type_meta_data_t amqp_type_meta_data_uint;
extern amqp_type_meta_data_t amqp_type_meta_data_int;
extern amqp_type_meta_data_t amqp_type_meta_data_float_ieee_754;
extern amqp_type_meta_data_t amqp_type_meta_data_char_utf32;
extern amqp_type_meta_data_t amqp_type_meta_data_decimal32_ieee_754;
extern amqp_type_meta_data_t amqp_type_meta_data_extension_fixed_4;

extern amqp_type_meta_data_t amqp_type_meta_data_ulong;
extern amqp_type_meta_data_t amqp_type_meta_data_long;
extern amqp_type_meta_data_t amqp_type_meta_data_double_ieee_754;
extern amqp_type_meta_data_t amqp_type_meta_data_timestamp_ms64;
extern amqp_type_meta_data_t amqp_type_meta_data_decimal64_ieee_754;
extern amqp_type_meta_data_t amqp_type_meta_data_extension_fixed_8;

extern amqp_type_meta_data_t amqp_type_meta_data_uuid;
extern amqp_type_meta_data_t amqp_type_meta_data_extension_fixed_16;

extern amqp_type_meta_data_t amqp_type_meta_data_binary_vbin8;
extern amqp_type_meta_data_t amqp_type_meta_data_binary_vbin32;

extern amqp_type_meta_data_t amqp_type_meta_data_string_str8_utf8;
extern amqp_type_meta_data_t amqp_type_meta_data_string_str8_utf16;
extern amqp_type_meta_data_t amqp_type_meta_data_symbol_sym8;
extern amqp_type_meta_data_t amqp_type_meta_data_symbol_sym32;

extern amqp_type_meta_data_t amqp_type_meta_data_extension_variable_1;
extern amqp_type_meta_data_t amqp_type_meta_data_string_str32_utf8;
extern amqp_type_meta_data_t amqp_type_meta_data_string_str32_utf16;
extern amqp_type_meta_data_t amqp_type_meta_data_extension_variable_4;
extern amqp_type_meta_data_t amqp_type_meta_data_list_8;
extern amqp_type_meta_data_t amqp_type_meta_data_map_8;
extern amqp_type_meta_data_t amqp_type_meta_data_extension_compound_1;
extern amqp_type_meta_data_t amqp_type_meta_data_list_32;
extern amqp_type_meta_data_t amqp_type_meta_data_map_32;
extern amqp_type_meta_data_t amqp_type_meta_data_extension_compound_4;
extern amqp_type_meta_data_t amqp_type_meta_data_array_8;
extern amqp_type_meta_data_t amqp_type_meta_data_extension_array_1;
extern amqp_type_meta_data_t amqp_type_meta_data_array_32;
extern amqp_type_meta_data_t amqp_type_meta_data_extension_array_4;

extern amqp_type_meta_data_t *amqp_type_meta_data_lookup(amqp_context_t *context, int format_code);

#ifdef __cplusplus
}
#endif
#endif


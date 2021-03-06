#include <stdarg.h>
#include <check.h>
#include <metaresc.h>
#include <regression.h>

TYPEDEF_CHAR_ARRAY (char_array_t, 256)
TYPEDEF_STRUCT (struct_string_t, string_t x)
TYPEDEF_STRUCT (struct_charp_t, (char *, x))
TYPEDEF_STRUCT (struct_char_array_t, (char, x, [256])) /* saved as an array of characters */
TYPEDEF_STRUCT (struct_ca_t, CHAR_ARRAY (char, x, [256])) /* saved as a quoted string */
TYPEDEF_STRUCT (struct_ca_type_t, (char_array_t, x))

TYPEDEF_STRUCT (struct_ca_str_t, (char_array_t, x), string_t y)
TYPEDEF_STRUCT (struct_str_ca_t, string_t y, (char_array_t, x))
TYPEDEF_STRUCT (struct_str_str_t, string_t x, string_t y)

#define ASSERT_SAVE_LOAD_CHAR_POINTER(METHOD, ...) ({			\
      ASSERT_SAVE_LOAD_TYPE (METHOD, string_t,  __VA_ARGS__);		\
      ASSERT_SAVE_LOAD_TYPE (METHOD, char *, __VA_ARGS__);		\
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_string_t, __VA_ARGS__);	\
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_charp_t, __VA_ARGS__);	\
    })

#define ASSERT_SAVE_LOAD_STRING(METHOD, ...) ({				\
      ASSERT_SAVE_LOAD_CHAR_POINTER (METHOD, __VA_ARGS__);		\
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_char_array_t, __VA_ARGS__); \
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_ca_t, __VA_ARGS__);		\
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_ca_type_t, __VA_ARGS__);	\
    })

MR_START_TEST (null_string_t, "NULL string") { ALL_METHODS (ASSERT_SAVE_LOAD_CHAR_POINTER, NULL); } END_TEST
MR_START_TEST (empty_string_t, "empty string") { ALL_METHODS (ASSERT_SAVE_LOAD_STRING, ""); } END_TEST
MR_START_TEST (printable_string_t, "all printable characters string") { ALL_METHODS (ASSERT_SAVE_LOAD_STRING, "\040\041\042\043\044\045\046\047\050\051\052\053\054\055\056\057\060\061\062\063\064\065\066\067\070\071\072\073\074\075\076\077\100\101\102\103\104\105\106\107\110\111\112\113\114\115\116\117\120\121\122\123\124\125\126\127\130\131\132\133\134\135\136\137\140\141\142\143\144\145\146\147\150\151\152\153\154\155\156\157\160\161\162\163\164\165\166\167\170\171\172\173\174\175\176"); } END_TEST
MR_START_TEST (xml_special_string_t, "xml special characters string") { ALL_METHODS (ASSERT_SAVE_LOAD_STRING, "&<>;'\"\t\r\n"); } END_TEST

MR_START_TEST (pointer_match_content, "strings points on another field") {
  struct_ca_str_t orig_ca_str = { .x = "string_t" };
  struct_str_ca_t orig_str_ca = { .x = "string_t" };
  orig_ca_str.y = orig_ca_str.x;
  orig_str_ca.y = orig_str_ca.x;
  ALL_METHODS (ASSERT_SAVE_LOAD, struct_ca_str_t, &orig_ca_str);
  ALL_METHODS (ASSERT_SAVE_LOAD, struct_str_ca_t, &orig_str_ca);
} END_TEST

MR_START_TEST (pointer_match_another_pointer, "two strings points on the same content") {
  struct_str_str_t orig = { .x = "string_t", };
  orig.y = orig.x;
  ALL_METHODS (ASSERT_SAVE_LOAD, struct_str_str_t, &orig);
  orig.y = (void*)&orig;
  ALL_METHODS (ASSERT_SAVE_LOAD, struct_str_str_t, &orig);
} END_TEST

MR_START_TEST (pointer_match_another_content, "strings points on the some other content") {
  struct_string_t orig;
  orig.x = (void*)&orig;
  ALL_METHODS (ASSERT_SAVE_LOAD, struct_string_t, &orig);
} END_TEST

MAIN ();


#include "item.h"
#include "item_test.h"
#include "testing.h"
#include "stopwatch.h"


// Prototypes
static char const* unit_test_bool();
static char const* unit_test_char();
static char const* unit_test_double();
static char const* unit_test_float();
static char const* unit_test_int();
static char const* unit_test_int8();
static char const* unit_test_int16();
static char const* unit_test_int32();
static char const* unit_test_int64();
static char const* unit_test_long();
static char const* unit_test_longlong();
static char const* unit_test_short();
static char const* unit_test_sizet();
static char const* unit_test_uint();
static char const* unit_test_uint8();
static char const* unit_test_uint16();
static char const* unit_test_uint32();
static char const* unit_test_uint64();
static char const* unit_test_uchar();
static char const* unit_test_ushort();
static char const* unit_test_ulong();
static char const* unit_test_ulonglong();
static char const* unit_test_ptr();
static char const* unit_test_cptr();

void pro_perf_test_item() {
}

void pro_unit_test_item()
{
  pro_print_unit_result("ITEM: bool", unit_test_bool());
  pro_print_unit_result("ITEM: char", unit_test_char());
  pro_print_unit_result("ITEM: double", unit_test_double());
  pro_print_unit_result("ITEM: float", unit_test_float());
  pro_print_unit_result("ITEM: int", unit_test_int());
  pro_print_unit_result("ITEM: int8_t", unit_test_int8());
  pro_print_unit_result("ITEM: int16_t", unit_test_int16());
  pro_print_unit_result("ITEM: int32_t", unit_test_int32());
  pro_print_unit_result("ITEM: int64_t", unit_test_int64());
  pro_print_unit_result("ITEM: long", unit_test_long());
  pro_print_unit_result("ITEM: long long", unit_test_longlong());
  pro_print_unit_result("ITEM: short", unit_test_short());
  pro_print_unit_result("ITEM: size_t", unit_test_sizet());
  pro_print_unit_result("ITEM: unsigned int", unit_test_uint());
  pro_print_unit_result("ITEM: uint8_t", unit_test_uint8());
  pro_print_unit_result("ITEM: uint16_t", unit_test_uint16());
  pro_print_unit_result("ITEM: uint32_t", unit_test_uint32());
  pro_print_unit_result("ITEM: uint64_t", unit_test_uint64());
  pro_print_unit_result("ITEM: unsigned char", unit_test_uchar());
  pro_print_unit_result("ITEM: unsigned short", unit_test_ushort());
  pro_print_unit_result("ITEM: unsigned long", unit_test_ulong());
  pro_print_unit_result("ITEM: unsigned long long", unit_test_ulonglong());
  pro_print_unit_result("ITEM: pointer", unit_test_ptr());
  pro_print_unit_result("ITEM: const pointer", unit_test_cptr());
}

#define PRO_ITEM_PERF_TEST_INIT(name, type, member, val) \
  static uint64_t name() { \
    pro_stopwatch_t stopwatch; \
    uint64_t const iters = 100000000; \
    pro_stopwatch_init(&stopwatch); \
    pro_stopwatch_start(&stopwatch); \
    for(uint64_t ii = 0; ii != iters; ++ii) { \
      pro_item_ ## type (val); \
    } \
    pro_stopwatch_stop(&stopwatch); \
    return pro_stopwatch_elapsed_nanos(&stopwatch) / iters; \
  }

//PRO_ITEM_PERF_TEST_INIT(perf_test_bool_init, bool, b, true)

#define PRO_ITEM_TEST_DEF(name, member, val1, val2) \
  static char const* name() { \
    pro_item_t item1; \
    item1.member = val1; \
    if (item1.member != val1) { \
      return "Failed to store value correctly!"; \
    } \
    pro_item_t item2; \
    item2.member = val2; \
    if (pro_item_are_equal(item1, item2)) { \
      return "Incorrectly asserted equality between unequal items."; \
    } \
    pro_item_t item3; \
    item3.member = val1; \
    if (!pro_item_are_equal(item1, item3)) { \
      return "Incorrectly asserted inequality between equal items."; \
    } \
    return ""; \
  }

PRO_ITEM_TEST_DEF(unit_test_bool, b, true, false)
PRO_ITEM_TEST_DEF(unit_test_char, c, 'a', 'A')
PRO_ITEM_TEST_DEF(unit_test_double, d, 0.0, 123.456)
PRO_ITEM_TEST_DEF(unit_test_float, f, 0.0, 123.456)
PRO_ITEM_TEST_DEF(unit_test_int, i, -1, 1234)
PRO_ITEM_TEST_DEF(unit_test_int8, i8, -1, 127)
PRO_ITEM_TEST_DEF(unit_test_int16, i16, 1, 2)
PRO_ITEM_TEST_DEF(unit_test_int32, i32, 1, 2)
PRO_ITEM_TEST_DEF(unit_test_int64, i64, 1, 2)
PRO_ITEM_TEST_DEF(unit_test_long, l, 1, 2)
PRO_ITEM_TEST_DEF(unit_test_longlong, ll, 1, 2)
PRO_ITEM_TEST_DEF(unit_test_short, s, 1, 2)
PRO_ITEM_TEST_DEF(unit_test_sizet, szt, 1, 2)
PRO_ITEM_TEST_DEF(unit_test_uint, u, 1, 2)
PRO_ITEM_TEST_DEF(unit_test_uint8, u8, 1, 2)
PRO_ITEM_TEST_DEF(unit_test_uint16, u16, 1, 2)
PRO_ITEM_TEST_DEF(unit_test_uint32, u32, 1, 2)
PRO_ITEM_TEST_DEF(unit_test_uint64, u64, 1, 2)
PRO_ITEM_TEST_DEF(unit_test_uchar, uc, 'a', 'b')
PRO_ITEM_TEST_DEF(unit_test_ushort, us, 1, 2)
PRO_ITEM_TEST_DEF(unit_test_ulong, ul, 1, 2)
PRO_ITEM_TEST_DEF(unit_test_ulonglong, ull, 1, 2)
PRO_ITEM_TEST_DEF(unit_test_ptr, ptr, 0, (void*)&pro_unit_test_item)
PRO_ITEM_TEST_DEF(unit_test_cptr, cptr, 0, (void*)&pro_unit_test_item)



#include "item.h"
#include "item_test.h"
#include "testing.h"


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

void pmt_perf_test_item()
{
}

void pmt_unit_test_item()
{
  pmt_print_unit_result("ITEM: bool", unit_test_bool());
  pmt_print_unit_result("ITEM: char", unit_test_char());
  pmt_print_unit_result("ITEM: double", unit_test_double());
  pmt_print_unit_result("ITEM: float", unit_test_float());
  pmt_print_unit_result("ITEM: int", unit_test_int());
  pmt_print_unit_result("ITEM: int8_t", unit_test_int8());
  pmt_print_unit_result("ITEM: int16_t", unit_test_int16());
  pmt_print_unit_result("ITEM: int32_t", unit_test_int32());
  pmt_print_unit_result("ITEM: int64_t", unit_test_int64());
  pmt_print_unit_result("ITEM: long", unit_test_long());
  pmt_print_unit_result("ITEM: long long", unit_test_longlong());
  pmt_print_unit_result("ITEM: short", unit_test_short());
  pmt_print_unit_result("ITEM: size_t", unit_test_sizet());
  pmt_print_unit_result("ITEM: unsigned int", unit_test_uint());
  pmt_print_unit_result("ITEM: uint8_t", unit_test_uint8());
  pmt_print_unit_result("ITEM: uint16_t", unit_test_uint16());
  pmt_print_unit_result("ITEM: uint32_t", unit_test_uint32());
  pmt_print_unit_result("ITEM: uint64_t", unit_test_uint64());
  pmt_print_unit_result("ITEM: unsigned char", unit_test_uchar());
  pmt_print_unit_result("ITEM: unsigned short", unit_test_ushort());
  pmt_print_unit_result("ITEM: unsigned long", unit_test_ulong());
  pmt_print_unit_result("ITEM: unsigned long long", unit_test_ulonglong());
  pmt_print_unit_result("ITEM: pointer", unit_test_ptr());
  pmt_print_unit_result("ITEM: const pointer", unit_test_cptr());
}

#define PMT_ITEM_TEST_DEF(name, member, val1, val2) \
  static char const* name() { \
    pmt_item_t item1; \
    item1.member = val1; \
    if (item1.member != val1) { \
      return "Failed to store value correctly!"; \
    } \
    pmt_item_t item2; \
    item2.member = val2; \
    if (pmt_item_are_equal(item1, item2)) { \
      return "Incorrectly asserted equality between unequal items."; \
    } \
    pmt_item_t item3; \
    item3.member = val1; \
    if (!pmt_item_are_equal(item1, item3)) { \
      return "Incorrectly asserted inequality between equal items."; \
    } \
    return ""; \
  }

PMT_ITEM_TEST_DEF(unit_test_bool, b, true, false)
PMT_ITEM_TEST_DEF(unit_test_char, c, 'a', 'A')
PMT_ITEM_TEST_DEF(unit_test_double, d, 0.0, 123.456)
PMT_ITEM_TEST_DEF(unit_test_float, f, 0.0, 123.456)
PMT_ITEM_TEST_DEF(unit_test_int, i, -1, 1234)
PMT_ITEM_TEST_DEF(unit_test_int8, i8, -1, 127)
PMT_ITEM_TEST_DEF(unit_test_int16, i16, 1, 2)
PMT_ITEM_TEST_DEF(unit_test_int32, i32, 1, 2)
PMT_ITEM_TEST_DEF(unit_test_int64, i64, 1, 2)
PMT_ITEM_TEST_DEF(unit_test_long, l, 1, 2)
PMT_ITEM_TEST_DEF(unit_test_longlong, ll, 1, 2)
PMT_ITEM_TEST_DEF(unit_test_short, s, 1, 2)
PMT_ITEM_TEST_DEF(unit_test_sizet, szt, 1, 2)
PMT_ITEM_TEST_DEF(unit_test_uint, u, 1, 2)
PMT_ITEM_TEST_DEF(unit_test_uint8, u8, 1, 2)
PMT_ITEM_TEST_DEF(unit_test_uint16, u16, 1, 2)
PMT_ITEM_TEST_DEF(unit_test_uint32, u32, 1, 2)
PMT_ITEM_TEST_DEF(unit_test_uint64, u64, 1, 2)
PMT_ITEM_TEST_DEF(unit_test_uchar, uc, 'a', 'b')
PMT_ITEM_TEST_DEF(unit_test_ushort, us, 1, 2)
PMT_ITEM_TEST_DEF(unit_test_ulong, ul, 1, 2)
PMT_ITEM_TEST_DEF(unit_test_ulonglong, ull, 1, 2)
PMT_ITEM_TEST_DEF(unit_test_ptr, ptr, 0, (void*)&pmt_unit_test_item)
PMT_ITEM_TEST_DEF(unit_test_cptr, cptr, 0, (void*)&pmt_unit_test_item)


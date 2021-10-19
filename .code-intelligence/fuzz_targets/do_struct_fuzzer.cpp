#include <mutator.h>

#include <string>

#include "api.h"

INIT_STRUCT_FUZZER() { RegisterLengthField("data_size", "data"); }

DEFINE_STRUCT_FUZZER(struct MyStruct, m) {
  DoStuffWithMyStruct(m);  // Disregard the output.
}

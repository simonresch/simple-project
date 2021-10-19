#include "api.h"

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  TriggerUndefinedBehaviorSanitizer();

  return 0;
}

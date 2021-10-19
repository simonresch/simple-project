// Copyright 2017 Google Inc. All Rights Reserved.
// Licensed under the Apache License, Version 2.0 (the "License");
#include <mutator.h>
#include "json_api.h"
#include <string>

DEFINE_JSON_FUZZER(const uint8_t *data, size_t size) {
  std::string str(reinterpret_cast<const char *>(data), size);
  DoJSONStuff(str);  // Disregard the output.
}

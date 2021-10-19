// Copyright 2017 Google Inc. All Rights Reserved.
// Licensed under the Apache License, Version 2.0 (the "License");
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  if (size >= 4 && data[0] == 'B' && data[1] == 'U' && data[2] == 'G' &&
      data[3] == '!') {
    *(char *)1 = 2;
  }
  return 0;
}

// Copyright 2017 Google Inc. All Rights Reserved.
// Licensed under the Apache License, Version 2.0 (the "License");

// A library that does ... stuff.
// Serves as an example of good fuzz testing and OSS-Fuzz integration.
#include <stddef.h>
#include <string>

struct MyStruct {
  int extra_value;
  int data_size;
  const char *data;
};

size_t DoStuff(const std::string &str);

size_t DoStuffWithMyStruct(struct MyStruct m);

int TriggerUndefinedBehaviorSanitizer();

void LeakMemory();
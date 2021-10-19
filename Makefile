# Copyright 2017 Google Inc. All Rights Reserved.
# Licensed under the Apache License, Version 2.0 (the "License");

# Simple example of a build file that nicely integrates a fuzz target
# with the rest of the project.
#
# We use 'make' as the build system, but these ideas are applicable
# to any other build system

# By default, use our own standalone_fuzz_target_runner.
# This runner does no fuzzing, but simply executes the inputs
# provided via parameters.
# Run e.g. "make all LIB_FUZZING_ENGINE=/path/to/libFuzzer.a"
# to link the fuzzer(s) against a real fuzzing engine.
#
# OSS-Fuzz will define it's own <value for LIB_FUZZING_ENGINE.
# LIB_FUZZING_ENGINE ?= standalone_fuzz_target_runner.o

# Values for CC, CFLAGS, CXX, CXXFLAGS are provided by OSS-Fuzz.
# Outside of OSS-Fuzz use the ones you prefer or rely on the default values.
# Do not use the -fsanitize=* flags by default.
# OSS-Fuzz will use different -fsanitize=* flags for different builds (asan, ubsan, msan, ...)

# You may add extra compiler flags like this:
CXXFLAGS += -std=c++11

all: do_stuff_unittest socket_server # do_stuff_fuzzer

# Continuos integration system should run "make clean && make check"
check: all
	./do_stuff_unittest
	# ./do_stuff_fuzzer do_stuff_test_data/*

# Unit tests
do_stuff_unittest: do_stuff_unittest.cpp api.a json_api.a
	@echo Compiling do_stuff_unittest
	${CXX} ${CXXFLAGS} -pthread -lpthread $< api.a -o $@

# Fuzz target, links against $LIB_FUZZING_ENGINE, so that
# you may choose which fuzzing engine to use.
# do_stuff_fuzzer: do_stuff_fuzzer.cpp api.a standalone_fuzz_target_runner.o
# 	${CXX} ${CXXFLAGS} $< api.a ${LIB_FUZZING_ENGINE} -o $@
# 	zip -q -r do_stuff_fuzzer_seed_corpus.zip do_stuff_test_data

api.o: api.cpp api.h
	${CXX} ${CXXFLAGS} -fpic $< -c

api.a: api.o
	ar rv api.a api.o

libapi.so: api.o
	$(CXX) ${CXXFLAGS} -shared -o libapi.so api.o

test_link_directly:
	$(CXX) ${CXXFLAGS} api.cpp main.cpp

test_link_object: api.o
	$(CXX) ${CXXFLAGS} api.o main.cpp

test_link_shared_object: libapi.so
	$(CXX) ${CXXFLAGS} -lapi -L. main.cpp

test_link_static_lib: api.o
	$(CXX) ${CXXFLAGS} api.o main.cpp

test_compile_shared_object_directly:
	$(CXX) ${CXXFLAGS} -shared -o libapi.so api.cpp

socket_server: socket_server_main.c libserver_funcs.a
	$(CC) ${CFLAGS} -lm -o socket_server socket_server_main.c -L. -lserver_funcs

server_funcs.o: server_funcs.c server_funcs.h
	$(CC) ${CFLAGS} -O -c server_funcs.c

libserver_funcs.a: server_funcs.o
	ar rcs libserver_funcs.a server_funcs.o

json_api.a: json_api.o json.o
	ar rv json_api.a json_api.o json.o

json_api.o: json_api.cpp json_api.h
	$(CXX) ${CXXFLAGS} -o $@ -c  $<

json.o: json11.cpp json11.hpp
	$(CXX) ${CXXFLAGS} -o $@ -c  $<

clean:
	rm -f *.o *.a *.so a.out socket_server

# The standalone fuzz target runner.
# standalone_fuzz_target_runner.o: standalone_fuzz_target_runner.cpp

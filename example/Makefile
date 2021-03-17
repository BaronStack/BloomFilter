# step 1:
# prepare the compiler
# prepare targe file name
# prepare dependecy libarary
CC = g++
CFLAGS  := -Wall -O2 -fPIC -std=c++11

# ifneq ($(USE_RTTI), 1)
# 	CFLAGS += -fno-rtti
# endif

# header file's path
INCLUDE_PATH = -I../include
LIBPATH = ../libfilter.so
STATIC_LIBPATH = ../libfilter.a

all : bloomfilter_test

bloomfilter_test: bloomfilter_test.cc
	$(CC) $(CFLAGS)  $(STATIC_LIBPATH) $(INCLUDE_PATH) $@.cc -o$@  $(LDFLAGS)

# 4. clean the files except source file
clean:
	rm -f ./bloomfilter_test

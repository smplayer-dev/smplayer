PROG = simple_web_server
MODULE_CFLAGS=-DMG_DISABLE_DAV_AUTH -DMG_ENABLE_HTTP_CGI=0 -DMG_ENABLE_THREADS=0 -DMG_ENABLE_HTTP_SSI=0

SOURCES = main.c mongoose.c
CFLAGS = -O2 -W -Wall -Werror -Wno-unused-function $(CFLAGS_EXTRA) $(MODULE_CFLAGS)

ifeq ($(OS), Windows_NT)
CFLAGS += -lws2_32
CC = gcc
DEL_FILE = del
PROG := $(PROG).exe
else
CFLAGS += -pthread
DEL_FILE = rm -f
endif

all: $(PROG)

$(PROG): $(SOURCES) Makefile
	$(CC) $(SOURCES) -o $@ $(CFLAGS)

clean:
	$(DEL_FILE) $(PROG)

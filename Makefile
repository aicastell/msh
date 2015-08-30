MOD_NAME=msh
MOD_DIR_SRC=src
MOD_DIR_INC=inc
MOD_DIR_BIN=bin
PRJ_DIR_RELEASE_BIN=/usr/local/bin

CXX=$(CROSS_PREFIX)gcc
LD=$(CROSS_PREFIX)gcc

CXXFLAGS:=$(CXXFLAGS) -c
CXXFLAGS:=$(CXXFLAGS) -ggdb3
CXXFLAGS:=$(CXXFLAGS) -Wall
CXXFLAGS:=$(CXXFLAGS) -I$(MOD_DIR_INC)

SRCS:=$(wildcard src/*.c)
OBJS:=$(SRCS:src%.c=bin%.o)

.DEFAULT_GOAL := $(MOD_NAME)

pre:
	mkdir -p $(MOD_DIR_BIN)
	mkdir -p $(PRJ_DIR_RELEASE_BIN)

clean:
	rm -f $(MOD_DIR_BIN)/$(MOD_NAME) $(OBJS)

distclean: clean
	rm -Rf $(PRJ_DIR_RELEASE_BIN)/$(MOD_NAME)

install: $(MOD_NAME)
	sudo cp $(MOD_DIR_BIN)/$(MOD_NAME) $(PRJ_DIR_RELEASE_BIN)
	sudo chmod +x $(PRJ_DIR_RELEASE_BIN)/$(MOD_NAME)
	sudo strip $(PRJ_DIR_RELEASE_BIN)/$(MOD_NAME)

$(MOD_NAME): pre $(OBJS)
	$(LD) $(OBJS) -o $(MOD_DIR_BIN)/$(MOD_NAME) $(LDFLAGS)

$(MOD_DIR_BIN)/%.o: $(MOD_DIR_SRC)/%.c
	$(CXX) $(CXXFLAGS) $< -o $@

.PHONY: pre clean distclean install $(MOD_NAME)

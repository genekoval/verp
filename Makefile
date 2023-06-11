project := verp
summary := A C++ version parsing library and command line utility.

STD := c++20

cli := $(project)
$(cli).type := executable
$(cli).libs := commline fmt

install := $(cli)
targets := $(install)

test.libs := fmt gtest gtest_main

install.directories = $(include)/$(project)

files = $(include) $(src) Makefile VERSION

include mkbuild/base.mk

$(obj)/$(cli)/main.o: CXXFLAGS +=\
 -DNAME='"$(cli)"'\
 -DVERSION='"$(version)"'

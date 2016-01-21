# This file is part of AirStashPlayer.
# Copyright (c) 2016 Wearable Inc.
#
# AirStashPlayer is based on ijkPlayer:
# Copyright (c) 2013-2014 Zhang Rui <bbcallen@gmail.com>
#
# Portions of ijkPlayer are based on kxmovie:
# Copyright (c) 2012 Konstantin Boukreev. All rights reserved.
#
# AirStashPlayer is free software: you can redistribute it and/or
# modify it under the terms of version 3 of the GNU Lesser General
# Public License as published by the Free Software Foundation.
#
# AirStashPlayer is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with AirStashPlayer.  If not, see
# <http://www.gnu.org/licenses/>.

CC = g++
CPPFLAGS = -std=c++11 -I./ -Wno-deprecated-register
YACC = /opt/bin/bison
YACC_FLAGS = --debug --verbose -d
YYLEX = flex
YYLEX_FLAGS = 

all: airstash

YACC_DEPS = \
	jni.bison.y \
	jni.flex.l \

CXX_OBJS = \
	ast/ast_class.o \
	ast/ast_compilation_unit.o \
	ast/ast_constructor.o \
	ast/ast_field.o \
	ast/ast_identifier.o \
	ast/ast_member.o \
	ast/ast_method.o \
	ast/ast_node.o \
	ast/ast_property_accessor.o \
	ast/ast_reference_type.o \
	ast/ast__context.o \
	ast/ast__namespace.o \
	jni.flex.yy.o \
	jni.bison.tab.o \
	airstash.o \

DEPS := $(CXX_OBJS:.o=.d)
-include $(DEPS)

$(CXX_OBJS): Makefile jni.bison.tab.cpp jni.flex.yy.cpp

%.o: %.cpp
	$(CC) $(CPPFLAGS) -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(CC) $(CPPFLAGS) -c -o $@ $<

airstash: $(CXX_OBJS)
	$(CC) -o airstash $(CXX_OBJS)

jni.bison.tab.cpp: $(YACC_DEPS)
	$(YACC) $(YACC_FLAGS) -o $@ jni.bison.y

jni.flex.yy.cpp: $(YACC_DEPS)
	$(YYLEX) $(YYLEX_FLAGS) -o $@ jni.flex.l

.PHONY: all clean
clean:
	rm -f jni.bison.tab.cpp
	rm -f jni.bison.tab.hpp
	rm -f jni.flex.yy.cpp
	rm -f $(CXX_OBJS)
	rm -f $(DEPS)
	rm -f airstash

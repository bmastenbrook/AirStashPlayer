/*
 * This file is part of AirStashPlayer.
 * Copyright (c) 2016 Wearable Inc.
 *
 * AirStashPlayer is based on ijkPlayer:
 * Copyright (c) 2013-2014 Zhang Rui <bbcallen@gmail.com>
 *
 * Portions of ijkPlayer are based on kxmovie:
 * Copyright (c) 2012 Konstantin Boukreev. All rights reserved.
 *
 * AirStashPlayer is free software: you can redistribute it and/or
 * modify it under the terms of version 3 of the GNU Lesser General
 * Public License as published by the Free Software Foundation.
 *
 * AirStashPlayer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with AirStashPlayer.  If not, see
 * <http://www.gnu.org/licenses/>.
 */
#ifndef AIRSTASH_PARSER_HPP
#define AIRSTASH_PARSER_HPP

#include <cstdio>
#include <list>
#include <set>
#include <map>
#include <memory>
#include <string>
#include <sstream>
#include <vector>
#include "bfc/bfc.h"

#define FLEX_LOGF(...)
// #define BISON_LOGF(...)
#define BISON_LOGF printf

extern int yylex();
extern FILE *yyin;
extern FILE *yyout;
extern int yylineno;
static void yyerror(const char *s) { printf("ERROR: at line %d\n\t%s\n", yylineno, s);}

NAMESPACE_STD_BEGIN

template<class char_t, class traits_t>
inline std::basic_ostream<char_t, traits_t>&
operator<<(
    std::basic_ostream<char_t, traits_t>            &os,
    const std::auto_ptr<std::basic_string<char_t> > &str)
{
    return os << str->c_str();
}

class ast_fill
{
public:
    ast_fill(int width): m_width(width) {;}
    int m_width;
};

template<class char_t, class traits_t>
inline std::basic_ostream<char_t, traits_t>&
operator<<(
    std::basic_ostream<char_t, traits_t>    &os,
    const ast_fill                          &rhs)
{
    for (int i = 0; i < rhs.m_width; ++i)
        os << ' ';
    return os;
}

NAMESPACE_STD_END

#endif

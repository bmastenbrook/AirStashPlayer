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

#ifndef AIRSTASH_PARSER_AST__AST_ARGUMENT__HPP
#define AIRSTASH_PARSER_AST__AST_ARGUMENT__HPP

#include "ast__def.hpp"
#include "ast_type.hpp"

NAMESPACE_AST_BEGIN

class Argument: public Identifier
{
public:
    AST_CHILD_DEFINE(Type, type);

public:
    // @Override
    virtual void debug_print(int indent) override {
        get_type()->debug_print(0);
        printf(" %s", get_name()->c_str());
    }

public:
    AST_IMPLEMENT(Argument);
protected:
    explicit Argument(const bfc::string_ptr &name): Identifier(name) {;}
public:
    static pointer_type make_ptr(const bfc::string_ptr& name) {return pointer_type(new Argument(name));}
};

class ArgumentList: public NodeList<Argument>
{
public:
    // @Override
    virtual void debug_print(int indent) override {
        iterator begin = this->begin();
        iterator end   = this->end();

        if (begin != end) {
            (*begin)->debug_print(indent);
            ++begin;

            for (NULL; begin != end; ++begin) {
                printf(", ");
                (*begin)->debug_print(indent);
            }
        }
    }

public:
    AST_IMPLEMENT_ABSTRACT(ArgumentList);
protected:
    explicit ArgumentList() {;}
public:
    static pointer_type make_ptr() {return pointer_type(new ArgumentList());}
};

NAMESPACE_AST_END

#endif

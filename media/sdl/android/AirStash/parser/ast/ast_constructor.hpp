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

#ifndef AIRSTASH_PARSER_AST__AST_CONSTRUCTOR__HPP
#define AIRSTASH_PARSER_AST__AST_CONSTRUCTOR__HPP

#include "ast__def.hpp"
#include "ast_method.hpp"
#include "ast_field.hpp"

NAMESPACE_AST_BEGIN

class Constructor: public Method
{
public:
    AST_GETTER_DECL_OVERRIDE(bfc::string_ptr,   c_jni_sign);
    AST_GETTER_DECL_OVERRIDE(bfc::string_ptr,   c_jni_id_name);
    AST_GETTER_DECL_OVERRIDE(bfc::string_ptr,   c_jni_method_name);

    AST_GETTER_DECL_OVERRIDE(bfc::string_ptr,   c_call_api);
    AST_GETTER_DECL_OVERRIDE(bfc::string_ptr,   c_call_object_id);
    AST_GETTER_DECL_OVERRIDE(bool,              c_call_need_this);

public:
    virtual void debug_print(int indent) override {
        print_space(indent);

        get_modifier_set()->debug_print(0);
        printf("%s(", get_name()->c_str());
        get_argument_list()->debug_print(0);
        printf(");\n");
    }

public:
    AST_IMPLEMENT(Constructor);
protected:
    explicit Constructor(const bfc::string_ptr &name): Method(name) {;}
public:
    static pointer_type make_ptr(const bfc::string_ptr& name) {return pointer_type(new Constructor(name));}
};

NAMESPACE_AST_END

#endif

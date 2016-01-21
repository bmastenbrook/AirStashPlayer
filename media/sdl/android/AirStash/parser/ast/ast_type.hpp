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

#ifndef AIRSTASH_PARSER_AST__AST_TYPE__HPP
#define AIRSTASH_PARSER_AST__AST_TYPE__HPP

#include "ast__def.hpp"
#include "ast_identifier.hpp"

NAMESPACE_AST_BEGIN

class Type: public Identifier
{
public:
    AST_PROPERTY_DEFINE(bool, is_array);

    virtual bool is_reference_type() = 0;
    virtual bool is_void_type() = 0;
    virtual bool is_string_type() = 0;

    AST_GETTER_DECL_0(bfc::string_ptr, c_type);
    AST_GETTER_DECL_0(bfc::string_ptr, c_sign_in_method);
    AST_GETTER_DECL_0(bfc::string_ptr, c_default_value);

    AST_GETTER_DECL_0(bfc::string_ptr, c_call_instance_method_api);
    AST_GETTER_DECL_0(bfc::string_ptr, c_call_static_method_api);
    AST_GETTER_DECL_0(bfc::string_ptr, c_get_instance_field_api);
    AST_GETTER_DECL_0(bfc::string_ptr, c_get_static_field_api);
    AST_GETTER_DECL_0(bfc::string_ptr, c_set_instance_field_api);
    AST_GETTER_DECL_0(bfc::string_ptr, c_set_static_field_api);

public:
    // @Override
    virtual void debug_print(int indent) override {
        if (get_is_array()) {
            printf("%s[]", get_name()->c_str());
        } else {
            printf("%s", get_name()->c_str());
        }
    }

public:
    AST_IMPLEMENT_ABSTRACT(Type);
protected:
    explicit Type(const bfc::string_ptr &name): Identifier(name) {init(NULL);}
    explicit Type(Node *other): Identifier(other) {init(other);}
private:
    void init(Node *other) {
        set_is_array(false);

        if (other) {
            Type *other_ = dynamic_cast<Type *>(other);
            if (other_)
                set_is_array(other_->get_is_array());
        }
    }
};

NAMESPACE_AST_END

#endif

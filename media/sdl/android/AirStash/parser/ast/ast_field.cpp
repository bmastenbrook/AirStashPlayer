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

#include "ast_field.hpp"
#include "ast_class.hpp"
#include "ast_property_accessor.hpp"

using namespace ast;

//@Override
bfc::string_ptr Field::get_c_jni_sign()
{
    return get_type()->get_c_sign_in_method();
}

//@Override
bfc::string_ptr Field::get_c_jni_id_name()
{
    std::ostringstream os;
    os << "field_" << get_name();
    return bfc::string::make_ptr(os);
}

//@Override
void Field::build_c_func_decl(std::ostream &os)
{
    PropertyGetter::make_ptr(this)->build_c_func_decl(os);
    PropertySetter::make_ptr(this)->build_c_func_decl(os);
}

//@Override
void Field::build_c_member_id_decl(std::ostream &os)
{
    os << build_indent() << "jfieldID " << get_c_jni_id_name() << ";" << std::endl;
}

//@Override
void Field::build_c_member_id_load(std::ostream &os)
{
    os << std::endl;
    os << build_indent() << "class_id = " << get_this_class()->get_c_jni_id() << ";\n";
    os << build_indent() << "name     = \"" << get_name() << "\";\n";
    os << build_indent() << "sign     = \"" << get_c_jni_sign() << "\";\n";
    os << build_indent() << get_c_jni_id() << " = " << (is_static() ? "AirStash_GetStaticFieldID__catchAll" : "AirStash_GetFieldID__catchAll")
                            << "(env, class_id, name, sign);\n";
    os << build_indent() << "if (" << get_c_jni_id() << " == NULL)\n";
    os << build_indent() << "    goto fail;\n";
}

//@Override
void Field::build_c_func_impl(std::ostream &os)
{
    PropertyGetter::make_ptr(this)->build_c_func_impl(os);
    PropertySetter::make_ptr(this)->build_c_func_impl(os);
}

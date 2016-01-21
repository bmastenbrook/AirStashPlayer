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

#include "ast_identifier.hpp"

using namespace ast;

Identifier::pointer_type Identifier::get_outer_class_identifier()
{
    Identifier::pointer_type prefix = get_prefix();
    if (!prefix)
        return NULL;

    if (prefix->is_class_identifier())
        return prefix;

    return prefix->get_outer_class_identifier();
}

bfc::string_ptr Identifier::get_java_long_name()
{
    std::ostringstream os;

    Identifier::pointer_type prefix = get_prefix();
    if (prefix) {
        os << prefix->get_java_long_name();
        os << ".";
    }

    os << get_name();
    return bfc::string::make_ptr(os);
}

bfc::string_ptr Identifier::get_c_long_name()
{
    std::ostringstream os;

    Identifier::pointer_type prefix = get_prefix();
    if (prefix) {
        os << prefix->get_c_long_name();
        os << "_";
    }

    os << get_name();
    return bfc::string::make_ptr(os);
}

bfc::string_ptr Identifier::get_c_jni_sign()
{
    std::ostringstream os;

    Identifier::pointer_type prefix = get_prefix();
    if (prefix) {
        os << prefix->get_c_jni_sign();
        os << "/";
    }

    os << get_name();
    return bfc::string::make_ptr(os);
}

bfc::string_ptr Identifier::get_fs_long_path()
{
    std::ostringstream os;

    Identifier::pointer_type prefix = get_prefix();
    if (prefix) {
        os << prefix->get_fs_long_path();
        os << "/";
    }

    os << get_name();
    return bfc::string::make_ptr(os);
}



bfc::string_ptr ClassIdentifier::get_c_jni_sign()
{
    std::ostringstream os;

    Identifier::pointer_type prefix = get_prefix();
    if (prefix) {
        os << prefix->get_c_jni_sign();
        if (get_outer_class_identifier())
            os << "$";
        else
            os << "/";
    }

    os << get_name();
    return bfc::string::make_ptr(os);
}

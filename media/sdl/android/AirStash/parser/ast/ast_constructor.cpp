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

#include "ast_constructor.hpp"
#include "ast_class.hpp"

using namespace ast;

bfc::string_ptr Constructor::get_c_jni_sign()
{
    std::ostringstream os;
    os << "(";

    ArgumentList::iterator begin = get_argument_list()->begin();
    ArgumentList::iterator end   = get_argument_list()->end();
    for (NULL; begin != end; ++begin) {
        os << (*begin)->get_type()->get_c_sign_in_method();
    }

    os << ")V";
    return bfc::string::make_ptr(os);
}

bfc::string_ptr Constructor::get_c_jni_id_name()
{
    std::ostringstream os;
    os << "constructor_" << get_name();
    return bfc::string::make_ptr(os);
}

bfc::string_ptr Constructor::get_c_jni_method_name()
{
    return bfc::string::make_ptr("<init>");
}

bfc::string_ptr Constructor::get_c_call_api()
{
    return bfc::string::make_ptr("NewObject");
}

bfc::string_ptr Constructor::get_c_call_object_id()
{
    return get_this_class()->get_c_jni_id();
}

bool Constructor::get_c_call_need_this()
{
    return false;
}

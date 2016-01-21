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
#include "ast_property_accessor.hpp"
#include "ast_class.hpp"

using namespace ast;

bfc::string_ptr PropertyGetter::get_c_call_api()
{
    if (is_static()) {
        return get_field()->get_type()->get_c_get_static_field_api();
    } else {
        return get_field()->get_type()->get_c_get_instance_field_api();
    }
}

bfc::string_ptr PropertyGetter::get_c_call_method_id()
{
    std::ostringstream os;
    os << get_this_class()->get_c_jni_class_instance() << "." << get_field()->get_c_jni_id_name();
    return bfc::string::make_ptr(os);
}



bfc::string_ptr PropertySetter::get_c_call_api()
{
    if (is_static()) {
        return get_field()->get_type()->get_c_set_static_field_api();
    } else {
        return get_field()->get_type()->get_c_set_instance_field_api();
    }
}

bfc::string_ptr PropertySetter::get_c_call_method_id()
{
    std::ostringstream os;
    os << get_this_class()->get_c_jni_class_instance() << "." << get_field()->get_c_jni_id_name();
    return bfc::string::make_ptr(os);
}

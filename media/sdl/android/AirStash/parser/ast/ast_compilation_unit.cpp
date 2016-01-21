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

#include "ast_compilation_unit.hpp"

#include <fstream>
#include <iostream>
#include "ast_method.hpp"
#include "ast_class.hpp"
#include "ast__context.hpp"

using namespace ast;

static const char *AIRSTASH_LICENSE_HEADER =
"/*\n"
" * This file is part of AirStashPlayer.\n"
" * Copyright (c) 2016 Wearable Inc.\n"
" *\n"
" * AirStashPlayer is based on ijkPlayer:\n"
" * Copyright (c) 2013-2014 Zhang Rui <bbcallen@gmail.com>\n"
" *\n"
" * Portions of ijkPlayer are based on kxmovie:\n"
" * Copyright (c) 2012 Konstantin Boukreev. All rights reserved.\n"
" *\n"
" * AirStashPlayer is free software: you can redistribute it and/or\n"
" * modify it under the terms of version 3 of the GNU Lesser General\n"
" * Public License as published by the Free Software Foundation.\n"
" *\n"
" * AirStashPlayer is distributed in the hope that it will be useful,\n"
" * but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
" * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU\n"
" * Lesser General Public License for more details.\n"
" *\n"
" * You should have received a copy of the GNU Lesser General Public\n"
" * License along with AirStashPlayer.  If not, see\n"
" * <http://www.gnu.org/licenses/>.\n"
" */\n";
void CompilationUnit::do_build(std::ostream &h_os, std::ostream &c_os)
{
    Class      *clazz       = get_clazz();

    printf("==========\n");
    // printf("%s\n", __func__);

    // .h
    h_os << AIRSTASH_LICENSE_HEADER << std::endl;
    h_os << "#ifndef " << get_header_macro() << std::endl;
    h_os << "#define " << get_header_macro() << std::endl;
    h_os << std::endl;
    h_os << "#include \"sdl/android/AirStash/internal/internal.h\"" << std::endl;
    h_os << std::endl;

    clazz->build_c_func_decl(h_os);
    h_os << std::endl;
    if (clazz->is_simple_c_class_name()) {
        h_os << "#define AIRSTASH_HAVE__" << clazz->get_c_class_name() << std::endl;
        h_os << std::endl;
    }
    if (clazz->is_include_util()) {
        h_os << "#include \"" << clazz->get_name() << ".util.h\"" << std::endl;
        h_os << std::endl;
    }
    h_os << "#endif//" << get_header_macro() << std::endl;

    // .c
    c_os << AIRSTASH_LICENSE_HEADER << std::endl;
    c_os << "#include \"" << clazz->get_name() << ".h\"" << std::endl;

    clazz->build_c_class_decl(c_os);
    clazz->build_c_func_impl(c_os);
}

void CompilationUnit::build()
{
    std::ofstream h_stream;
    std::ofstream c_stream;
    std::ostream *h_stream_ptr = &std::cout;
    std::ostream *c_stream_ptr = &std::cout;

    if (!Context::instance()->get_h_file_path().empty()) {
        h_stream.open(Context::instance()->get_h_file_path().c_str());
        if (!h_stream.is_open()) {
            printf("failed to open output .h file %s\n", Context::instance()->get_h_file_path().c_str());
            return;
        }
        h_stream_ptr = &h_stream;
    }

    if (!Context::instance()->get_c_file_path().empty()) {
        c_stream.open(Context::instance()->get_c_file_path().c_str());
        if (!c_stream) {
            printf("failed to open output .c file %s\n", Context::instance()->get_c_file_path().c_str());
            return;
        }
        c_stream_ptr = &c_stream;
    }

    do_build(*h_stream_ptr, *c_stream_ptr);
}

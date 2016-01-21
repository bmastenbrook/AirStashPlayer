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

#include "parser.hpp"
#include "jni.bison.tab.hpp"
#include "ast/ast.hpp"
#include "bfc/bfc.h"

extern int flex_main(int argc, char **argv);
extern int bison_main(int argc, char **argv);

extern int yyparse();

int main(int argc, char **argv)
{
    printf("%s\n", argv[0]);

    if ( argc < 2 ) {
        printf(
            " usage:\n"
            "   airstash <input_file> <output_c_file> <output_h_file>\n");
        return -1;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        printf("failed to open input file %s\n", argv[1]);
        return -1;
    }

    ast::Context::instance()->set_h_file_path(argc >= 3 ? argv[2] : "");
    ast::Context::instance()->set_c_file_path(argc >= 4 ? argv[3] : "");

    const char *file_name = strrchr(argv[1], '/');
    std::string dir_name = std::string(argv[1], file_name - argv[1]);
    // printf("add java path: %s\n", dir_name->c_str());
    ast::Context::instance()->set_java_class_dir(dir_name.c_str());

    int ret = yyparse();
    fclose(yyin);
    return ret;
}

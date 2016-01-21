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

#ifndef AIRSTASH_PARSER_AST__AST_COMPILATION_UNIT__HPP
#define AIRSTASH_PARSER_AST__AST_COMPILATION_UNIT__HPP

#include "ast__def.hpp"
#include "ast__namespace.hpp"
#include "ast_identifier.hpp"
#include "ast_import.hpp"
#include "ast_class.hpp"

NAMESPACE_AST_BEGIN

class CompilationUnit: public Node
{
public:
    AST_PROPERTY_DEFINE(Namespace*,     local_namespace);

    AST_CHILD_DEFINE(ImportList,        import_list);
    AST_CHILD_DEFINE(Class,             clazz);
    AST_CHILD_DEFINE(Identifier,        package);

    bfc::string_ptr get_header_macro() {
        std::ostringstream os;
        os << "AirStash__";
        os << get_package()->get_c_long_name();
        os << "_";
        os << get_clazz()->get_name();
        os << "__H";
        return bfc::string::make_ptr(os);
    }

    bfc::string_ptr get_include_path() {
        std::ostringstream os;
        os << "sdl/android/player/c/";
        os << get_package()->get_fs_long_path();
        os << "/";
        os << get_clazz()->get_name();
        os << ".h";
        return bfc::string::make_ptr(os);
    }

    void do_build(std::ostream &h_os, std::ostream &c_os);
    void build();

public:
    // @Override
    virtual CompilationUnit *get_this_compilation_unit() override {return this;}
    // output
    virtual void debug_print(int indent) override {
        get_package()->debug_print(0);
        printf(";\n");
        get_import_list()->debug_print(0);
        printf("\n");
        get_clazz()->debug_print(0);
        printf("\n");
    }

private:
    void init() {
        set_local_namespace(new Namespace());
    }

public:
    AST_IMPLEMENT(CompilationUnit);
protected:
    explicit CompilationUnit() {init();}
public:
    static pointer_type make_ptr() {return pointer_type(new CompilationUnit());}
};

NAMESPACE_AST_END

#endif

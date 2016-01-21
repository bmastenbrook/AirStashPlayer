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

#ifndef AIRSTASH_PARSER_AST__AST_IMPORT__HPP
#define AIRSTASH_PARSER_AST__AST_IMPORT__HPP

#include "ast__def.hpp"
#include "ast_node.hpp"
#include "ast_identifier.hpp"

NAMESPACE_AST_BEGIN

class ImportList: public NodeList<Identifier>
{
public:
    // @Override
    virtual void debug_print(int indent) override {
        if (size() > 0) {
            printf("\n");

            iterator begin = this->begin();
            iterator end   = this->end();

            for (NULL; begin != end; ++begin) {
                printf("import ");
                (*begin)->debug_print(indent);
                printf(";\n");
            }
        }
    }

public:
    AST_IMPLEMENT(ImportList);
protected:
    explicit ImportList() {;}
public:
    static pointer_type make_ptr() {return pointer_type(new ImportList());}
};

NAMESPACE_AST_END

#endif

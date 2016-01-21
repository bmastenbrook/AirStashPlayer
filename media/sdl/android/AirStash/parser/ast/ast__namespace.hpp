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

#ifndef AIRSTASH_PARSER_AST__AST__NAMESPACE__HPP
#define AIRSTASH_PARSER_AST__AST__NAMESPACE__HPP

#include "ast__def.hpp"
#include "ast_identifier.hpp"

NAMESPACE_AST_BEGIN

class Namespace
{
public:
    typedef std::map<bfc::string_ptr, Identifier::pointer_type> IdentifierMap;
    typedef IdentifierMap::iterator iterator;

    AST_PROPERTY_DEFINE(Identifier::pointer_type, identifier);

    iterator begin() {return m_id_map.begin();}
    iterator end()   {return m_id_map.end();}

    void add_package_identifier(Identifier *package_identifier);
    void add_class_identifier(Identifier *class_identifier);
    template<class T>
    void add_class_identifiers(T begin, T end) {
        for (NULL; begin != end; ++begin) {
            add_class_identifier(*begin);
        }
    }

    Identifier *find_identifier(const bfc::string_ptr &name);

    void clear() {m_id_map.clear();}

private:
    IdentifierMap m_id_map;
};

NAMESPACE_AST_END

#endif

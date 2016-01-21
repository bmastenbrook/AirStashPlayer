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

#include "ast__namespace.hpp"
#include <dirent.h>
#include <cstring>
#include "ast__context.hpp"
#include "ast_class.hpp"

using namespace ast;

void Namespace::add_package_identifier(Identifier *package_identifier)
{
    DIR *d;
    struct dirent *dir;

    assert(package_identifier);
    d = opendir(Context::instance()->get_java_class_dir().c_str());
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            const char *ext = strchr(dir->d_name, '.');
            if (ext && 0 == strcmp(ext, ".java")) {
                bfc::string_ptr class_name = bfc::string::make_ptr(dir->d_name, ext - dir->d_name);
                add_class_identifier(package_identifier->make_by_join(class_name).get());
            }
        }
        closedir(d);
    }
}

void Namespace::add_class_identifier(Identifier *class_identifier)
{
    bfc::string_ptr name        = class_identifier->get_name();
    bfc::string_ptr full_name   = class_identifier->get_java_long_name();
    m_id_map[name]      = class_identifier;
    m_id_map[full_name] = class_identifier;

    Identifier::pointer_type outer_class = class_identifier->get_outer_class_identifier();
    if (!outer_class)
        return;

    Identifier::pointer_type inner_identifier = Identifier::make_ptr(class_identifier->get_name());
    Identifier::pointer_type outer_identifier = inner_identifier;
    while (outer_class) {
        outer_identifier->set_prefix(Identifier::make_ptr(outer_class->get_name()));
        outer_identifier = outer_identifier->get_prefix();

        bfc::string_ptr          relative_name       = inner_identifier->get_java_long_name();
        Identifier::pointer_type relative_identifier = Identifier::parse(relative_name->c_str());
        m_id_map[relative_name] = relative_identifier;

        outer_class = outer_class->get_outer_class_identifier();
    }
}

Identifier *Namespace::find_identifier(const bfc::string_ptr &name)
{
    IdentifierMap::iterator find = m_id_map.find(name);
    return find == m_id_map.end() ? NULL : find->second.get();
}

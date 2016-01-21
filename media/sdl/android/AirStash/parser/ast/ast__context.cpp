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

#include "ast__context.hpp"

#include "ast.hpp"

using namespace ast;

Context *Context::s_instance;

Namespace *Context::get_local_namespace()
{
    assert(!m_namespace_stack.empty());
    return m_namespace_stack.back();
}

void Context::push_local_namespace(Namespace *local_namespace)
{
    m_namespace_stack.push_back(local_namespace);
}

void Context::pop_local_namespace()
{
    assert(!m_namespace_stack.empty());
    m_namespace_stack.pop_back();
}

Identifier *Context::find_identifier(Node *node, const bfc::string_ptr& name)
{
    Identifier *qid = NULL;

    CompilationUnit *this_compilation_unit = node->get_this_compilation_unit();
    if (this_compilation_unit) {
        // before AST build
        Class *this_class = node->get_this_class();
        while (this_class) {
            qid = this_class->get_local_namespace()->find_identifier(name);
            if (qid)
                return qid;

            if (!this_class->get_parent())
                break;

            this_class = this_class->get_parent()->get_this_class();
        }

        qid = this_compilation_unit->get_local_namespace()->find_identifier(name);
        if (qid)
            return qid;
    } else {
        // after AST build
        NamespaceStack::reverse_iterator rbegin = m_namespace_stack.rbegin();
        NamespaceStack::reverse_iterator rend   = m_namespace_stack.rend();

        for (NULL; rbegin != rend; ++rbegin) {
            qid = (*rbegin)->find_identifier(name);
            if (qid)
                return qid;
        }
    }

    qid = get_global_name_space()->find_identifier(name);
    return qid;
}

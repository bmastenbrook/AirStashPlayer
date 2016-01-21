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

#ifndef AIRSTASH_PARSER_AST__AST__FORWARD__HPP
#define AIRSTASH_PARSER_AST__AST__FORWARD__HPP

#include <cassert>
#include "bfc/bfc.h"

#define NAMESPACE_AST_BEGIN namespace ast {
#define NAMESPACE_AST_END   }

NAMESPACE_AST_BEGIN

// https://docs.oracle.com/javase/specs/jls/se7/html/jls-18.html
class Annotation;
class Annotations;
class Argument;
class ArgumentList;
class BasicType;
class Class;
class ClassIdentifier;
class CompilationUnit;
class Constructor;
class Field;
class Identifier;
class ImportList;
class MemberList;
class Method;
class MethodList;
class ModifierSet;
class Node;
class PackageIdentifier;
class ReferenceType;
class Type;

NAMESPACE_AST_END

#endif

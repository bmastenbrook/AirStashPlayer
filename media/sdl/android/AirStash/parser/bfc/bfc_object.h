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

#ifndef BFC__OBJECT__H
#define BFC__OBJECT__H

#include <assert.h>
#include "bfc__def.h"

NAMESPACE_BFC_BEGIN

class object
{
public:
    int add_ref() const
    {
        return __sync_add_and_fetch(&m_ref_count, 1);
    }

    int release() const
    {
        int ref_count = __sync_sub_and_fetch(&m_ref_count, 1);
        assert(ref_count >= 0);
        if (ref_count == 0)
            delete this;
        return ref_count;
    }

    int _get_ref() const {return m_ref_count;}

private:
    volatile int m_ref_count;

protected:
    object(): m_ref_count(0) {;}
    virtual ~object() {;}

private:
    object(const object&);
    object &operator=(const object&);
};

typedef rc_ptr<object> object_ptr;

NAMESPACE_BFC_END

#endif

﻿/*****************************************************************************
 * Copyright (C) 2018-2019 MillGame authors
 *
 * Authors: liuweilhy <liuweilhy@163.com>
 *          Calcitem <calcitem@outlook.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifndef STACK_H
#define STACK_H

template <typename T, size_t size = 128>
class Stack
{
public:
    inline void push(const T &obj)
    {
        p++;
        memcpy(arr + p, &obj, sizeof(T));

        assert(p < size);
    }

    inline void pop()
    {
        p--;
    }

    inline T &top()
    {
        return arr[p];
    }

private:
    int p { -1 };
    T arr[size];
};

#endif // STACK_H

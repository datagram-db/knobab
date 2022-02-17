/* Copyright (C) 1992-2018 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#ifndef _DB_BZ_OID
#define _DB_BZ_OID 1

#ifdef  _WIN64
#define WINDOWS
#include <Windows.h>
#if REG_DWORD == REG_DWORD_LITTLE_ENDIAN
#define IS_LITTLE_ENDIAN
#else
#define IS_BIG_ENDIAN
#endif
#endif

#ifdef unix
#include <features.h>
#include <endian.h>
#if	__BYTE_ORDER == __LITTLE_ENDIAN
#define IS_LITTLE_ENDIAN
#elif __BYTE_ORDER == __BIG_ENDIAN
#define IS_BIG_ENDIAN00
#endif
#endif

#include <ostream>

using trace_t = uint32_t;
using event_t = uint16_t;
//using future_t = unsigned char;
using act_t    = uint16_t;
using monotone_hash_t = uint_least64_t;

union _oid
{
    monotone_hash_t monotone_hash;

    /* This is the IEEE 754 single-precision format.  */
    struct S
    {
#ifdef IS_LITTLE_ENDIAN
        event_t          event_id:16;             // The last 16 bits represent the position of the even within the trace
        trace_t          trace_id:32;             // The following 32 bits represent the id associated to the trace
        act_t               act  :16;                // The next  8 bits contain the act information
        //future_t            future:4;                // The first 8 bits are associated to future information for the slot
#endif				/* Little endian.  */
#ifdef IS_BIG_ENDIAN
        // Big endians represent the numbers similarly, but in the reverse direction.

        //future_t           future:4;
        act_t              act   :16;
        trace_t          trace_id:32;             // The following 32 bits represent the id associated to the trace
        event_t          event_id:16;             // The last 16 bits represent the position of the even within the trace
#endif				/* Big endian.  */
    } __attribute__((packed)) parts;

} ;

struct oid {
    //static_assert(sizeof(_oid) == sizeof(monotone_hash_t) && sizeof(monotone_hash_t) == sizeof(_oid::S), "Error: the bit in the bitfield are not correctly packed");
    _oid id;

    oid();
    oid(const monotone_hash_t& x);
    oid(act_t fact, trace_t trace_id, event_t event_pos);
    oid(const oid& x);
    oid& operator=(const oid& x);

    bool operator<(const oid &rhs)  const;
    bool operator>(const oid &rhs)  const;
    bool operator<=(const oid &rhs) const;
    bool operator>=(const oid &rhs) const;
    bool operator==(const oid &rhs) const;
    bool operator!=(const oid &rhs) const;
};

/**
 * Putting the float inside a data structure which, by using a union with a float and bit for a struct, is able to
 * get which bits are which
 */
#ifdef WINDOWS
#define declare_printable_float(name, val)   _oid name; name.monotone_hash = (val)
#else
#define declare_printable_float(name, val)   _oid name = { .monotone_hash = (val) }
#endif


#endif
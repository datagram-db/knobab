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

#include <yaucl/structures/bit_tagged_unions.h>
#include <ostream>

using trace_t = uint32_t;
using event_t = uint16_t;
using act_t    = uint16_t;
using monotone_hash_t = uint_least64_t;

TAGGED_UNION_WITH_ENCAPSULATION_BEGIN(monotone_hash_t, oid, 0, 3, act_t act:16, trace_t trace_id:32, event_t event_id:16)
    (act_t fact, trace_t trace_id, event_t event_pos) {
        id.parts.act = fact;
        id.parts.trace_id = trace_id;
        id.parts.event_id = event_pos;
    }
TAGGED_UNION_ENCAPSULATOR_END


#endif

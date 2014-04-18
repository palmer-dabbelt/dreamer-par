/*
 * Copyright (C) 2014 Palmer Dabbelt
 *   <palmer.dabbelt@eecs.berkeley.edu>
 *
 * This file is part of dreamer-par.
 *
 * dreamer-par is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * dreamer-par is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with dreamer-par.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "avail_mem.h++"

avail_mem::avail_mem(size_t start_cycle, const std::weak_ptr<tile>& owner)
    : avail_reg(start_cycle, owner)
{
}

std::string avail_mem::to_string(void) const
{
    char buffer[1024];
    if (_end_cycle == (size_t)(-1)) {
        snprintf(buffer, 1024, "memory in %s after %lu",
                 _owner.lock()->name().c_str(),
                 _start_cycle
            );
    } else {
        abort();
    }
    return buffer;
}

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

#include "avail_reg.h++"
#include "tile.h++"

avail_reg::avail_reg(size_t start_cycle, const std::weak_ptr<tile>& owner)
    : _start_cycle(start_cycle),
      _end_cycle(-1),
      _owner(owner)
{
}

ssize_t avail_reg::obtain(const std::shared_ptr<tile>& t,
                          size_t first_cycle,
                          bool commit __attribute__((unused)))
{
    /* This register has been deallocated.  When this happens a
     * different availiability object will represent where this lives
     * in memory, so we just bail out here. */
    if (first_cycle > _end_cycle)
        return -1;

    /* Registers can only be read by the tile that they're written
     * by. */
    if (t->name() != _owner.lock()->name())
        return -1;

    size_t first_availiable_cycle = _start_cycle + t->m()->alu_delay() + 1;
    if (first_availiable_cycle < first_cycle)
        return first_cycle;
    return first_availiable_cycle;
}

void avail_reg::deallocate(size_t end_cycle)
{
    if (end_cycle >= _end_cycle)
        return;

    _end_cycle = end_cycle;
}

std::string avail_reg::to_string(void) const
{
    char buffer[1024];
    if (_end_cycle == (size_t)(-1)) {
        snprintf(buffer, 1024, "register in %s after %lu",
                 _owner.lock()->name().c_str(),
                 _start_cycle
            );
    } else {
        abort();
    }
    return buffer;
}


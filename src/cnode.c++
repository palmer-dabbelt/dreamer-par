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

#include "cnode.h++"

cnode::cnode(const std::string name,
             const libflo::unknown<size_t>& width,
             const libflo::unknown<size_t>& depth,
             bool is_mem,
             bool is_const,
             libflo::unknown<size_t> cycle)
    : libflo::node(name, width, depth, is_mem, is_const, cycle),
      _avail_list()
{
}

void cnode::make_availiable(const std::shared_ptr<availiability>& a)
{
    _avail_list.push_back(a);
}

ssize_t cnode::obtain(const std::shared_ptr<tile>& tile,
                      size_t first_cycle,
                      bool commit)
{
    std::shared_ptr<availiability> best = NULL;
    ssize_t best_cycle = -1;

    for (const auto& a: _avail_list) {
        ssize_t cycle = a->obtain(tile, first_cycle, false);
        if ((best == NULL) || (best_cycle > cycle)) {
            best = a;
            best_cycle = cycle;
        }
    }

    if (best == NULL)
        return -1;

    return best->obtain(tile, first_cycle, commit);
}

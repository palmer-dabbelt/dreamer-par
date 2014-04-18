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

#ifndef AVAIL_MEM_HXX
#define AVAIL_MEM_HXX

class avail_mem;

#include "avail_reg.h++"

/* This tracks the availiability of a value inside a register. */
class avail_mem: public avail_reg {
public:
    /* Nodes stored in memory can be deallocated just like registers.
     * This happens when there's been a register spill */
    avail_mem(size_t start_cycle, const std::weak_ptr<tile>& owner);

    avail_mem(size_t start_cycle, size_t end_cycle,
              const std::weak_ptr<tile>& owner);

    /* Overrides from "class avail_reg". */
    std::string to_string(void) const override;
};

#endif

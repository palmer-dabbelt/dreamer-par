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

#ifndef AVAIL_REG_HXX
#define AVAIL_REG_HXX

class avail_reg;

#include "availiability.h++"

/* This tracks the availiability of a value inside a register. */
class avail_reg: public availiability {
protected:
    ssize_t _start_cycle;
    ssize_t _end_cycle;
    std::weak_ptr<tile> _owner;

public:
    /* There are two ways to create a value that exists inside a
     * register: either this value can exist for all time (it has yet
     * to be deallocated) or it can exist for only a fixed internal
     * (we for some reason know it'll be deallocated soon). */
    avail_reg(ssize_t start_cycle, const std::weak_ptr<tile>& owner);

    avail_reg(ssize_t start_cycle, ssize_t end_cycle,
              const std::weak_ptr<tile>& owner);

    /* Overrides from "class availiability". */
    ssize_t cost_to_obtain(const std::shared_ptr<tile>& tile,
                           ssize_t cycle) override;
    std::shared_ptr<libdrasm::regval> obtain(
        const std::shared_ptr<tile>& tile,
        ssize_t& cycle) override;
    void deallocate(size_t cycle) override;
    std::string to_string(void) const override;
};

#endif

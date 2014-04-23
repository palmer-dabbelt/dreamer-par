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

avail_reg::avail_reg(ssize_t start_cycle, const std::weak_ptr<tile>& owner)
    : _start_cycle(start_cycle),
      _end_cycle(-1),
      _owner(owner)
{
}

ssize_t avail_reg::cost_to_obtain(const std::shared_ptr<tile>& tile,
                                  ssize_t cycle)
{
    /* This register has been deallocated.  When this happens a
     * different availiability object will represent where this lives
     * in memory, so we just bail out here. */
    if ((_end_cycle >= 0) && (cycle > _end_cycle))
        return -1;

    /* There's two ways we can make a register availiable: either we
     * can send it out over the network or we can just read it from
     * our regfile directly. */
    if (tile->name() != _owner.lock()->name()) {
        /* The register can be broadcast over the network. */
        auto owner_cycle = _owner.lock()->find_free_instruction(cycle);
        auto net = std::make_shared<avail_net>(owner_cycle, _owner);
        return net->cost_to_obtain(tile, cycle);
    } else {
        /* If the target tile is the same as where the register lives
         * then we just need to wait for the register to be done
         * computing, which happens after any ALU delay. */
        ssize_t first_cycle = _start_cycle + tile->m()->alu_delay() + 1;
        if (first_cycle < cycle)
            return cycle;
        return first_cycle;
    }
}

std::shared_ptr<libdrasm::regval> avail_reg::obtain(
    const std::shared_ptr<tile>& tile,
    ssize_t& cycle)
{
    if ((_end_cycle >= 0) && ((ssize_t)cycle > _end_cycle))
        return NULL;

    if (tile->name() != _owner.lock()->name()) {
        auto owner_cycle = _owner.lock()->find_free_instruction(cycle);
        auto net = std::make_shared<avail_net>(owner_cycle, _owner);
        return net->obtain(tile, cycle);
    } else {
        ssize_t first_cycle = _start_cycle + tile->m()->alu_delay() + 1;
        if (first_cycle > cycle)
            cycle = first_cycle;
        return std::make_shared<libdrasm::regval>();
    }
}

void avail_reg::deallocate(size_t end_cycle)
{
    if ((ssize_t)end_cycle >= _end_cycle)
        return;

    _end_cycle = end_cycle;
}

std::string avail_reg::to_string(void) const
{
    char buffer[1024];
    if (_end_cycle == -1) {
        snprintf(buffer, 1024, "register in %s after %lu",
                 _owner.lock()->name().c_str(),
                 _start_cycle
            );
    } else {
        abort();
    }
    return buffer;
}


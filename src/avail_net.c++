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

#include "avail_net.h++"
#include "avail_reg.h++"
#include "tile.h++"

avail_net::avail_net(ssize_t start_cycle,
                     const std::weak_ptr<tile>& owner)
    : _cycle(start_cycle),
      _owner(owner)
{
}

ssize_t avail_net::cost_to_obtain(const std::shared_ptr<tile>& tile,
                                  ssize_t cycle)
{
    /* FIXME: Does it make sense to loop back network ports? */
    if (tile->name() == _owner.lock()->name())
        return -1;

    /* If for some reason there's no path between these two then it
     * won't be possible to route at all. */
    auto path = _owner.lock()->search(tile);
    if (path == NULL)
        return -1;

    /* Just assume no congestion, the shortest route will always be
     * picked. */
    ssize_t arrival_cycle = _cycle + path->cost();
    if (arrival_cycle < cycle)
        return cycle;
    return arrival_cycle;
}

std::shared_ptr<libdrasm::regval> avail_net::obtain(
        const std::shared_ptr<tile>& tile,
        ssize_t& cycle)
{
    /* FIXME: Does it make sense to loop back network ports? */
    if (tile->name() == _owner.lock()->name())
        return NULL;

    /* If for some reason there's no path between these two then it
     * won't be possible to route at all. */
    auto path = _owner.lock()->search(tile);
    if (path == NULL)
        return NULL;

    /* Start tracking the arrival cycle of this packet as it flows
     * through each tile.  We start with the first cycle that this
     * packet is availiable at. */
    auto arrival_cycle = _cycle;

    /* Walk the path that this packet will take, finding a free
     * transmission slot at every point in time and updating the
     * arrival cycle to match that point explicitly. */
    for (const auto& step: path->steps()) {
        auto incoming_cycle = step->find_free_transmission(arrival_cycle);
        if (incoming_cycle < 0) {
            fprintf(stderr, "Ran out of network memory\n");
            abort();
        }
        /* FIXME: This isn't correct, it should be a per-route
         * parameter. */
        arrival_cycle = incoming_cycle + 2;
    }

    /* Here we allocate a register to store the result as it arrives
     * on the destination tile. */
    auto reg = tile->allocate_register(arrival_cycle);
    if (reg == NULL)
        return NULL;

    if (arrival_cycle > cycle)
        cycle = arrival_cycle;
    return std::make_shared<libdrasm::regval>();
}

void avail_net::deallocate(size_t end_cycle __attribute__((unused)))
{
}

std::string avail_net::to_string(void) const
{
    char buffer[1024];
    snprintf(buffer, 1024, "network in %s at %lu",
             _owner.lock()->name().c_str(),
             _cycle
        );
    return buffer;
}

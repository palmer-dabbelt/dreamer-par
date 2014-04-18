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

avail_net::avail_net(size_t start_cycle,
                     const std::weak_ptr<tile>& owner,
                     const std::weak_ptr<cnode>& node)
    : _cycle(start_cycle),
      _owner(owner),
      _node(node)
{
}

ssize_t avail_net::obtain(const std::shared_ptr<tile>& t,
                          size_t first_cycle,
                          bool commit)
{
    /* FIXME: Does it make sense to loop back network ports? */
    if (t->name() == _owner.lock()->name())
        return -1;

    /* If for some reason there's no path between these two then it
     * won't be possible to route at all. */
    auto path = _owner.lock()->search(t);
    if (path == NULL)
        return -1;

    /* FIXME: We aren't doing any sort of congestion control here... */
    ssize_t arrival_cycle = _cycle + path->cost();
    arrival_cycle = t->find_free_instruction(arrival_cycle);
    if (arrival_cycle < 0)
        return -1;

    ssize_t reg = t->allocate_register(arrival_cycle, false);
    if (reg < 0)
        return -1;

    if (commit) {
        if (t->allocate_register(arrival_cycle, commit) < 0) {
            fprintf(stderr, "Register allocation failed\n");
            abort();
        }
    }

    if ((size_t)arrival_cycle < first_cycle)
        return first_cycle;
    return arrival_cycle;
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

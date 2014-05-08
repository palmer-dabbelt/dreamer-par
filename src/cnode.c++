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
#include "avail_reg.h++"
#include "avail_net.h++"

cnode::cnode(const std::string name,
             const libflo::unknown<size_t>& width,
             const libflo::unknown<size_t>& depth,
             bool is_mem,
             bool is_const,
             libflo::unknown<size_t> cycle,
             const libflo::unknown<std::string>& posn)
    : libflo::node(name, width, depth, is_mem, is_const, cycle, posn),
      _avail_list()
{
}

void cnode::make_availiable(const std::shared_ptr<availiability>& a)
{
    _avail_list.push_back(a);
}

std::shared_ptr<libdrasm::regval> cnode::obtain(
    const std::shared_ptr<tile>& tile,
    ssize_t& cycle)
{
    std::shared_ptr<availiability> best = NULL;
    ssize_t best_cost = 0;

    for (const auto& a: _avail_list) {
        ssize_t cost = a->cost_to_obtain(tile, cycle);
#ifdef PRINT_PLACEMENT
        fprintf(stderr, "Obtained from '%s' cost %ld\n",
                a->to_string().c_str(),
                cost
            );
#endif

        if (cost < 0)
            continue;

        if ((best == NULL) || (best_cost > cost)) {
            best = a;
            best_cost = cost;
        }
    }

#ifdef PRINT_PLACEMENT
    fprintf(stderr, "best: '%s'\n",
            (best == NULL) ? "NULL" : best->to_string().c_str()
        );
#endif

    if (best == NULL)
        return NULL;

    auto obtained = best->obtain(tile, cycle);
    if (obtained == NULL) {
        fprintf(stderr, "'%s' provides '%s' with cost %ld, but no obtain\n",
                best->to_string().c_str(),
                name().c_str(),
                best_cost
            );
        abort();
    }
    return obtained;
}

void cnode::set_owner(const std::shared_ptr<tile>& owner)
{
    if (_owner != NULL)
        _owner->remove_node();

    _owner = owner;
    owner->add_node();
}

void cnode::computed_at(const std::shared_ptr<tile>& tile, ssize_t cycle)
{
    make_availiable(std::make_shared<avail_reg>(cycle, tile));
    make_availiable(std::make_shared<avail_net>(cycle, tile));
}

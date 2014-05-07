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

#include "place.h++"
#include "tempsched.h++"

static void place_arbitrary(
    const std::shared_ptr<libflo::flo<cnode, operation>>& flo,
    const std::vector<std::shared_ptr<tile>>& tiles
    );

static void place_anneal(
    const std::shared_ptr<libflo::flo<cnode, operation>>& flo,
    const std::vector<std::shared_ptr<tile>>& tiles
    );

static double find_cost(const std::shared_ptr<operation>& op,
                        const std::shared_ptr<tile>& target);

static double cost_on_tile(const std::shared_ptr<operation>& op,
                           const std::shared_ptr<tile>& tile);

void place(const std::shared_ptr<libflo::flo<cnode, operation>>& flo,
           const std::vector<std::shared_ptr<tile>>& tiles)
{
    place_arbitrary(flo, tiles);
    place_anneal(flo, tiles);
}

void place_arbitrary(
    const std::shared_ptr<libflo::flo<cnode, operation>>& flo,
    const std::vector<std::shared_ptr<tile>>& tiles
    )
{
    size_t last = 0;

    for (const auto& node: flo->nodes()) {
        if (node->owner() != NULL)
            continue;

        if (node->is_mem() == true)
            continue;

        node->set_owner(tiles[last]);
        last = (last + 1) % tiles.size();
    }
}

void place_anneal(
    const std::shared_ptr<libflo::flo<cnode, operation>>& flo,
    const std::vector<std::shared_ptr<tile>>& tiles
    )
{
    tempsched schedule;

    for (const auto temperature: schedule) {
        size_t swaps = 0;

        for (const auto& op: flo->operations()) {
            size_t to_try = rand() % tiles.size();
            double cost = find_cost(op, tiles[to_try]);
            if ((cost > 0) || (rand() < (RAND_MAX * temperature))) {
                op->d()->set_owner(tiles[to_try]);
                swaps++;
            }
        }
    }
}

double find_cost(const std::shared_ptr<operation>& op,
                 const std::shared_ptr<tile>& target)
{
    auto c = cost_on_tile(op, op->d()->owner());
    auto n = cost_on_tile(op, target);
    return c - n;
}

double cost_on_tile(const std::shared_ptr<operation>& op,
                    const std::shared_ptr<tile>& tile)
{
    double cost = 0;
    for (const auto& source: op->sources()) {
        /* Constants are always free. */
        if (source->is_const() == true)
            continue;

        /* The scheduler will place memories, there's some extra
         * constraints on them so we shouldn't screw with them
         * here. */
        if (source->is_mem() == true)
            continue;

        if (source->owner() == NULL) {
            fprintf(stderr, "source '%s' in '%s' has no owner\n",
                    source->name().c_str(),
                    op->to_string().c_str()
                );
            abort();
        }

        auto path = source->owner()->search(tile);

        /* This happens when we attempt to move to the same place, so
         * it's OK. */
        if (path == NULL)
            continue;

        cost += (path->cost() * path->cost() * path->cost());
    }

    return cost * tile->node_count();
}

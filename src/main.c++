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

#include "tile.h++"
#include "machine.h++"
#include "cnode.h++"
#include "operation.h++"
#include <libflo/flo.h++>

int main(int argc __attribute__((unused)),
         const char **argv __attribute__((unused)))
{
    /* Create a DREAMER machine descriptor, which contains all the
     * information that's necessary in order to describe a DREAMER
     * target. */
    std::shared_ptr<machine> m = std::make_shared<machine>(
        2, 2,
        [&m](size_t x, size_t y) -> std::shared_ptr<tile> {
            char buffer[1024];
            snprintf(buffer, 1024, "(%lu,%lu)", x, y);
            auto sp = std::make_shared<tile>(buffer, m);
            sp->set_self_pointer(sp);
            return sp;
        }
        );

    /* Parse the flo file that we're going to PAR. */
    auto f = libflo::flo<cnode, operation>::parse(argv[1]);

    /* Walk through that Flo file and attempt a place-and-route. */
    for (const auto& op: f->operations()) {

        /* Search every tile for the cheapest place to put this
         * operation that's still legal. */
        std::shared_ptr<tile> min_place = NULL;
        ssize_t min_cycle = -1;
        for (const auto& tile: m->network()->nodes()) {
            auto cycle = tile->place(op, false);
            if (cycle == -1)
                continue;

            if ((cycle < min_cycle) || (min_place == NULL)) {
                min_cycle = cycle;
                min_place = tile;
            }
        }

        /* If we were unable to place the node then just bail out.  At
         * some point I should probably figure out why! */
        if (min_place == NULL) {
            fprintf(stderr, "Unable to place operation '%s'\n",
                    op->to_string().c_str());
            abort();
        }

        /* Commit the placement for this node. */
        min_place->place(op, true);
    }

    return 0;
}

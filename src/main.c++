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

#include "avail_mem.h++"
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
        1, 1,
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

    /* The first thing we need to do is place memories. */
    for (const auto& mem: f->nodes()) {
        if (mem->is_mem() == false)
            continue;

#ifdef PRINT_PLACEMENT
        fprintf(stderr, "Placing Memory '%s'\n",
                mem->name().c_str()
            );
#endif

        ssize_t min_filled = -1;
        std::shared_ptr<tile> min_place = NULL;
        for (const auto& tile: m->network()->nodes()) {
            auto filled = tile->find_free_array(mem->depth());
            if (filled == -1)
                continue;

            if ((filled < min_filled) || (min_place == NULL)) {
                min_filled = filled;
                min_place = tile;
            }
        }

        if (min_place == NULL) {
            fprintf(stderr, "Unable to place memory '%s'\n",
                    mem->name().c_str()
                );
            abort();
        }

        min_place->use_array(min_filled, mem->depth());
        mem->set_owner(min_place);
        mem->make_availiable(std::make_shared<avail_mem>(0, min_place));
    }

    /* Walk through that Flo file and attempt a place-and-route. */
    size_t placed = 0;
    for (const auto& op: f->operations()) {
#ifdef PRINT_PLACEMENT
        if (op->op() != libflo::opcode::INIT) {
            fprintf(stderr, "Placing '%s'\n",
                    op->to_string().c_str()
                );
        }
#endif

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

            fprintf(stderr, "  Availiability Mapping:\n");
            for (const auto& source: op->sources()) {
                for (const auto& avail: source->avail_list()) {
                    fprintf(stderr, "    '%s' -> '%s'\n",
                            source->name().c_str(),
                            avail->to_string().c_str()
                        );
                }
            }

            fprintf(stderr, "%lu placed\n", placed);

            abort();
        }

#ifdef PRINT_PLACEMENT
        /* Commit the placement for this node. */
        if (op->op() != libflo::opcode::INIT) {
            fprintf(stderr, "\tat '%s' cycle %lu\n",
                    min_place->name().c_str(),
                    min_cycle
                );
        }
#endif

        min_place->place(op, true);
        placed++;
    }

    return 0;
}

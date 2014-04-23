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
        8, 8,
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
        fprintf(stderr, "\n\n\n\nPlacing Memory '%s'\n",
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

    /* Here we start with a list of all the possible targets on which
     * we can schedule an instruction.  These are kept in sorted order
     * by the cycle that their last instruction will run on. */
    std::vector<std::shared_ptr<tile>> tiles;
    for (const auto& tile: m->network()->nodes())
        tiles.push_back(tile);

    /* Walk through that Flo file and attempt a place-and-route every
     * Flo node by creating a number of DREAMER instructions that can
     * compute that node. */
    size_t placed = 0;
    for (const auto& op: f->operations()) {
#ifdef PRINT_PLACEMENT
        if (op->op() != libflo::opcode::INIT) {
            fprintf(stderr, "\n\n\n\nPlacing '%s'\n",
                    op->to_string().c_str()
                );
        }
#endif

        /* Search each tile, starting with the one that has the
         * earliest availiable cycle, in order to find a tile at which
         * we can actually schedule this node. */
        std::shared_ptr<tile> target_tile = NULL;
        for (const auto& tile: tiles) {
            /* Here's where we actually place the tile -- if this
             * returns TRUE then the correct instructions are already
             * in place that store this operation in the correct
             * place. */
            bool did_place = tile->place(op);
            if (did_place == true) {
                target_tile = tile;
                break;
            }
        }

        /* If we made it through the list of all tiles are were unable
         * to place this operation anywhere then we'll just bail
         * out. */
        if (target_tile == NULL) {
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
            fprintf(stderr, "\tat '%s' cycle %ld\n",
                    target_tile->name().c_str(),
                    op->cycle()
                );
        }
#endif

        placed++;
#ifdef DEBUG_PLACE_MODULO
        if ((placed % DEBUG_PLACE_MODULO) == 0)
            fprintf(stderr, "Placed: %ld\n", placed);
#endif

        std::sort(tiles.begin(), tiles.end(),
                  [](const std::shared_ptr<tile>& a,
                     const std::shared_ptr<tile>& b)
                  -> bool
                  {
                      if (a == NULL && b == NULL)
                          return 0;
                      if (a == NULL)
                          return 1;
                      if (b == NULL)
                          return -1;

                      auto ai = a->last_used_instruction();
                      auto bi = b->last_used_instruction();
                      return ai < bi;
                  }
            );
    }

    ssize_t last_cycle = 0;
    for (const auto& tile: tiles) {
        if (tile->find_free_instruction(0) > last_cycle)
            last_cycle = tile->last_used_instruction();
    }
    printf("%ld\n", last_cycle);

    return 0;
}

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
#include "cnode.h++"
#include "machine.h++"
#include "operation.h++"
#include "place.h++"
#include "tile.h++"
#include <libflo/flo.h++>

int main(int argc, const char **argv)
{
    if (argc != 3) {
        printf("%s <PROGRAM.flo> <NETWORK.ocn>: DREAMER Place-and-Route\n",
               argv[0]);
        printf("\tPerforms place-and-route for a DREAMER machine\n");

        return ((argc == 2) && (strcmp(argv[1], "--help") == 0)) ? 0 : 1;
    }

    /* Create a DREAMER machine descriptor, which contains all the
     * information that's necessary in order to describe a DREAMER
     * target. */
    std::shared_ptr<machine> m = std::make_shared<machine>(
        argv[2],
        [&m](std::string name) -> std::shared_ptr<tile> {
            auto sp = std::make_shared<tile>(name, m);
            sp->set_self_pointer(sp);
            return sp;
        }
        );

    /* Parse the flo file that we're going to PAR. */
    auto f = libflo::flo<cnode, operation>::parse(argv[1]);

    /* Here we start with a list of all the possible targets on which
     * we can schedule an instruction.  These are kept in sorted order
     * by the cycle that their last instruction will run on. */
    std::vector<std::shared_ptr<tile>> tiles;
    for (const auto& tile: m->network()->nodes())
        tiles.push_back(tile);

    /* Do placement here. */
    place(f, tiles);

    /* Walk through that Flo file and attempt a place-and-route every
     * Flo node by creating a number of DREAMER instructions that can
     * compute that node. */
    size_t placed = 0;
    for (const auto& op: f->operations()) {
        /* First we sort the  */
        std::sort(tiles.begin(), tiles.end(),
                  [](const std::shared_ptr<tile>& a,
                     const std::shared_ptr<tile>& b)
                  -> bool
                  {
                      auto ai = a->last_used_instruction();
                      auto bi = b->last_used_instruction();
                      return ai < bi;
                  }
            );

        /* If this operation has ever been placed then we need to sort
         * again, this time placing the node where this has been
         * placed first. */
        std::shared_ptr<tile> ontile = NULL;
        if (op->d()->pos_known() == true) {
            auto x = op->d()->x();
            auto y = op->d()->y();
            ontile = m->network()->lookup(x, y);
            if (ontile == NULL) {
                fprintf(stderr, "Placed on (%lu, %lu), but no tile\n",
                        x, y);
                abort();
            }
        }

        /* Check if this was placed by the integrated annealing
         * code. */
        if (op->d()->owner() != NULL)
            ontile = op->d()->owner();

        if (ontile != NULL) {
            std::stable_sort(tiles.begin(), tiles.end(),
                             [&ontile](const std::shared_ptr<tile>& a,
                                       const std::shared_ptr<tile>& b
                                             __attribute__((unused)))
                             {
                                 return ontile->name() == a->name();
                             }
                );
        }

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
    }

    ssize_t last_cycle = 0;
    for (const auto& tile: m->network()->nodes()) {
#ifdef DEBUG_ALL_TILE_COUNTS
        fprintf(stderr, "%s -> %ld\n",
                tile->name().c_str(),
                tile->find_free_instruction(0)
            );
#endif

        if (tile->find_free_instruction(0) > last_cycle)
            last_cycle = tile->last_used_instruction();
    }
    printf("%ld\n", last_cycle);

    return 0;
}

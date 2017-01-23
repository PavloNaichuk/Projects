#include "BattleCityGame.h"

/*
 * Refinements
 *
 * Improve occassional performance drop utilizing async computations or threads
 * Tank AI should ignore the target if there is non-destroyable obstacle between them immediately
 * Add better notify model changes mechanic
 * Improve spawning of enemy tanks to avoid likelihood of being spawned at the same location
 * Improve calculation of the game unit rotation angle
 * Investigate object pools to avoid unnecessary dynamic memory allocations/deallocations
 * Add explosions to missile hitting with object
 * Add animations to game units
 * Add audio support
 */

int main(int argc, char** argv)
{
    BattleCityGame battleCityGame;
    return battleCityGame.start(argc, argv);
}

#include "BattleCityGame.h"

/*
 * Game Units
 *
 * The game entities are derived from the base object GameUnit.
 * There are two types of game units - stationary (attached to one place on the map) and mobile (that is movable).
 * Walls on the map are examples of stationary units while tank and missile are instances of mobile units.
 *
 * Level Map
 *
 * Level map is represented as 2D matrix of equally-sized tiles (cells).
 * Each tile contains information about its location and stationary units standing on it.
 * Keeping information about stationary unit associated with the tile allows to speed up collision detection process.
 *
 * Collision Detection
 *
 * Collsion detection prevents game units from penetrating into other game units while navigating.
 * Since game units are allowed to move only in left, right, up and down directions,
 * collision detection functionality has been optimized specifically for this use-case.
 * Two collision feeler points are attached to the front face of the game unit
 * which are propagated along its heading on the test distance to reveal any obstacles.
 *
 * Tank AI
 *
 * Tank object is controlled by Tank AI object.
 * For player tank, tank AI object has dummy (empty) implementation
 * as the player is in charge for making decisions on the next action.
 * Tank AI object for enemy tank consists of goal objects. On each game frame update,
 * the goal is re-evaluated and based on its cost AI object will choose the next action.
 * Enemy tank is supplied with two goals: destroy player base and destroy player.
 * We define goal priority based on how close enemy tank is located to its target.
 *
 * Navigation
 *
 * In order to find a path to the player base and player tank, tank AI exploits
 * breadth-first search algorithm. Since tank unit has size greater than one map tile,
 * the original breadth-first search algorithm has been modified to take game unit size
 * into account during path-finding stage. Specifically, each map tile is assigned
 * so-called clearance metric which represents the number of tiles in bottom/right direction
 * to the first obstacle. If clearance value of the tile is less than the size of the game unit,
 * that tile should be discarded during path-finding.
 */

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

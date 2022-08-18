/******************************************************************************
 * File: Trailblazer.h
 *
 * Exports functions that use Dijkstra's algorithm, A* search, and Kruskal's
 * algorithm as specified in the assignment handout.
 */

#ifndef Trailblazer_Included
#define Trailblazer_Included

#include "TrailblazerTypes.h"
#include "TrailblazerPQueue.h"
#include "set.h"
#include "grid.h"
#include "random.h"
#include "vector.h"

/* Creates final path, puts chosen locations in vector in order from
 * start to end.
 */
void makePath(Vector<Loc>& path, Loc curr, Grid<node>& info, Loc start);

/* For each node v connected to curr by an edge of length L:
 * If v is gray: colors v yellow, sets v's candidate distance to be dist + L,
 * sets v's parent to be curr and enqueue v into the priority queue with priority dist + L.
 * Otherwise, if v is yellow and the candidate distance to v is greater than dist + L:
 * sets v's candidate distance to be dist + L, sets v's parent to be curr and updates v's 
 * priority in the priority queue to dist + L
 */
void addNeighbours(Grid<double>& world, Loc curr, Loc end, TrailblazerPQueue<Loc>& toVisit, Grid<node>& nodes,
					double costFn(Loc from, Loc to, Grid<double>& world),
					double heuristic(Loc start, Loc end, Grid<double>& world));

/* Function: shortestPath
 * 
 * Finds the shortest path between the locations given by start and end in the
 * specified world.	 The cost of moving from one edge to the next is specified
 * by the given cost function.	The resulting path is then returned as a
 * Vector<Loc> containing the locations to visit in the order in which they
 * would be visited.	If no path is found, this function should report an
 * error.
 *
 * In Part Two of this assignment, you will need to add an additional parameter
 * to this function that represents the heuristic to use while performing the
 * search.  Make sure to update both this function prototype and the
 * implementation inside of Trailblazer.cpp.
 */
Vector<Loc>
shortestPath(Loc start,
             Loc end,
             Grid<double>& world,
             double costFn(Loc from, Loc to, Grid<double>& world),
			 double heuristic(Loc start, Loc end, Grid<double>& world));

/* Inserts all edges in the graph into a priority queue
 */
void generateEdges(TrailblazerPQueue<Edge>& edges, int numRows, int numCols);

/* Finds which cluster Loc belongs to (dsu algorithm) 
 */
Loc findParent(Loc vert, Grid<Loc>& parentGrid);

/* Sets up parentGrid in a way that every node has different parent 
 * (value of each cell is parent Loc), so in every cell is written the cluster it belongs to.
 */
void makeClusters(Grid<Loc>& parentGrid);

/* Function: createMaze
 * 
 * Creates a maze of the specified dimensions using a randomized version of
 * Kruskal's algorithm, then returns a set of all of the edges in the maze.
 *
 * As specified in the assignment handout, the edges you should return here
 * represent the connections between locations in the graph that are passable.
 * Our provided starter code will then use these edges to build up a Grid
 * representation of the maze.
 */
Set<Edge> createMaze(int numRows, int numCols);

#endif

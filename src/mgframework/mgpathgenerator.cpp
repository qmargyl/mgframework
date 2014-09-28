
#include "mgpathgenerator.h"
#include "mgpathitem.h"
#include <list>
#include <stdio.h>
#include <algorithm>


void MGPathGenerator::calculatePathAStar(int x1, int y1, int x2, int y2, MGMap &map, std::list<MGPathItem> &path)
{
	// Goal == Start
	if(x1 == x2 && y1 == y2)
	{
		return;
	}

	int xMin = 0;
	int yMin = 0;
	int xMax = map.getWidth() - 1;
	int yMax = map.getHeight() - 1;

	std::list<MGAStarNode> open;
	std::list<MGAStarNode> closed;
	MGAStarNode nodeGoal(x2, y2, 0.0);
	MGAStarNode nodeStart(x1, y1, nodeGoal, 0.0);
	open.push_back(nodeStart);

	int passes = 0;
	int openDeletions = 0;
	int closedDeletions = 0;
	int openAdditions = 0;
	int successorDeletions = 0;
	int closedAdditions = 0;
	int successorAdditions = 0;
	int openLookups = 0;
	int closedLookups = 0;
	int successorLookups = 0;

	bool pathWasFound = false;
	path.clear();

	while(!open.empty())
	{
		passes++;
	
		// Take the node with smallest F from open list and call it nodeCurrent
		std::list<MGAStarNode>::iterator nodeCurrentIt = open.begin();
		openLookups++;
		for(std::list<MGAStarNode>::iterator nodeIt = open.begin(); nodeIt != open.end(); nodeIt++)
		{
			if((*nodeIt).getF() < (*nodeCurrentIt).getF())
			{
				nodeCurrentIt = nodeIt;
			}
		}

		printf("Best open = (%d, %d): %f\n", (*nodeCurrentIt).getX(), (*nodeCurrentIt).getY(), (*nodeCurrentIt).getF());

		MGAStarNode nodeCurrent(*nodeCurrentIt);
		openDeletions++;
		open.erase(nodeCurrentIt);
		
		// Step 6 done!!

		if(nodeCurrent == nodeGoal)
		{
			pathWasFound = true;
			closedAdditions++;
			closed.push_back(nodeCurrent);

			// TODO: Remove printf
			printf(	"Path found (%d,%d) -> (%d,%d) in %d passes\n"
					"%d open deletions, %d closed deletions, %d successor deletions, "
					"%d open additions, %d closed additions, %d successor additions, "
					"%d open lookups, %d closed lookups, %d successor lookups\n", 
					nodeStart.getX(), nodeStart.getY(), nodeGoal.getX(), nodeGoal.getY(), passes, 
					openDeletions, closedDeletions, successorDeletions,
					openAdditions, closedAdditions, successorAdditions,
					openLookups, closedLookups, successorLookups);
			for(std::list<MGAStarNode>::iterator nodeIt = closed.begin(); nodeIt != closed.end(); nodeIt++)
			{
				printf("(%d,%d) <- (%d,%d), ", (*nodeIt).getX(), (*nodeIt).getY(), (*nodeIt).getParentX(), (*nodeIt).getParentY());
			}
			printf("DONE\n");


			//Now, save the actual path in @path. Start from $closed(last) and follow it to @nodeStart
			if(!closed.empty())
			{
				MGPathItem pathEnt(closed.back().getX(), closed.back().getY());
				int lastParentX = closed.back().getParentX();
				int lastParentY = closed.back().getParentY();
				path.push_front(pathEnt);
				while(pathEnt.getX() != nodeStart.getX() || pathEnt.getY() != nodeStart.getY())
				{
					if(pathEnt.getX() == lastParentX && pathEnt.getY() == lastParentY)
					{
						path.push_front(pathEnt);
						lastParentX = closed.back().getParentX();
						lastParentY = closed.back().getParentY();
					}
					closed.pop_back();
					pathEnt = MGPathItem(closed.back().getX(), closed.back().getY());
				}
			}
			
			// TODO: Remove printf
			printf("Result:\n");
			for(std::list<MGPathItem>::iterator nodeIt = path.begin(); nodeIt != path.end(); nodeIt++)
			{
				printf("(%d,%d)\n", (*nodeIt).getX(), (*nodeIt).getY());
			}
			
			break;
		}
		
		// Step 7 done!!
		
		// Generate successors (neighbors)
		std::list<MGAStarNode> potentialSuccessors;
		std::list<MGAStarNode> successors;
		int x = nodeCurrent.getX();
		int y = nodeCurrent.getY();

		potentialSuccessors.push_back(MGAStarNode(x + 1, y + 1, nodeGoal, nodeCurrent.getG() + sqrt_2));
		potentialSuccessors.push_back(MGAStarNode(x + 1, y, nodeGoal, nodeCurrent.getG() + 1.0));
		potentialSuccessors.push_back(MGAStarNode(x, y + 1, nodeGoal, nodeCurrent.getG() + 1.0));
		potentialSuccessors.push_back(MGAStarNode(x - 1, y - 1, nodeGoal, nodeCurrent.getG() + sqrt_2));
		potentialSuccessors.push_back(MGAStarNode(x - 1, y, nodeGoal, nodeCurrent.getG() + 1.0));
		potentialSuccessors.push_back(MGAStarNode(x, y - 1, nodeGoal, nodeCurrent.getG() + 1.0));
		potentialSuccessors.push_back(MGAStarNode(x + 1, y - 1, nodeGoal, nodeCurrent.getG() + sqrt_2));
		potentialSuccessors.push_back(MGAStarNode(x - 1, y + 1, nodeGoal, nodeCurrent.getG() + sqrt_2));

		for(std::list<MGAStarNode>::iterator potSuccIt = potentialSuccessors.begin(); potSuccIt != potentialSuccessors.end(); potSuccIt++)
		{
			if(	(*potSuccIt).getX() >= xMin &&
				(*potSuccIt).getX() <= xMax &&
				(*potSuccIt).getY() >= yMin &&
				(*potSuccIt).getY() <= yMax &&
				map.occupant((*potSuccIt).getX(), (*potSuccIt).getY()) == 0 &&
				((*potSuccIt).getX() != nodeCurrent.getParentX() || (*potSuccIt).getY() != nodeCurrent.getParentY()))
			{
				successors.push_back((*potSuccIt));
				successorAdditions++;
			}
		}

		// Step 8 (and 11) done!!

		for(std::list<MGAStarNode>::iterator nodeSuccessor = successors.begin(); nodeSuccessor != successors.end(); )
		{
			(*nodeSuccessor).setH((*nodeSuccessor).heuristic(nodeGoal));

			// Find the successor in the open list
			openLookups++;
			std::list<MGAStarNode>::iterator evaluateOpenIt = find(open.begin(), open.end(), *nodeSuccessor);
			if(evaluateOpenIt != open.end())
			{
				if((*evaluateOpenIt).getF() <= (*nodeSuccessor).getF())
				{
					nodeSuccessor = successors.erase(nodeSuccessor);
					successorDeletions++;
					continue;
				}
				else
				{
					open.erase(evaluateOpenIt);
					openDeletions++;
				}
			}

			// Find the successor in the closed list
			closedLookups++;
			std::list<MGAStarNode>::iterator evaluateClosedIt = find(closed.begin(), closed.end(), *nodeSuccessor);
			if(evaluateClosedIt != closed.end())
			{
				if((*evaluateClosedIt).getF() <= (*nodeSuccessor).getF())
				{
					nodeSuccessor = successors.erase(nodeSuccessor);
					successorDeletions++;
					continue;
				}
				else
				{
					closed.erase(evaluateClosedIt);
					closedDeletions++;
				}
			}
			
			// Step 15 done!!
			
			(*nodeSuccessor).setParent(nodeCurrent);

			// Step 17 done!!
			printf("\tAdding successor (%d, %d): %f\n", (*nodeSuccessor).getX(), (*nodeSuccessor).getY(), (*nodeSuccessor).getF());
			open.push_front(*nodeSuccessor); // push_front allows the algorithm to find the most recently added neighbor first
			openAdditions++;
			
			nodeSuccessor++;
		}
		
		closed.push_back(nodeCurrent);
		closedAdditions++;
	}
	
	if(!pathWasFound)
	{
		// TODO: Remove printf
		printf(	"Path not found (%d,%d) -> (%d,%d) in %d passes\n"
				"%d open deletions, %d closed deletions, %d successor deletions, "
				"%d open additions, %d closed additions, %d successor additions, "
				"%d open lookups, %d closed lookups, %d successor lookups\n",
				nodeStart.getX(), nodeStart.getY(), nodeGoal.getX(), nodeGoal.getY(), passes,
				openDeletions, closedDeletions, successorDeletions,
				openAdditions, closedAdditions, successorAdditions,
				openLookups, closedLookups, successorLookups);
	}

}


void MGPathGenerator::calculatePathBasic(int x1, int y1, int x2, int y2, MGMap &map, std::list<MGPathItem> &path)
{
		int x = x1;
		int y = y1;
		std::list<MGPathItem> neighbors;
		std::list<MGPathItem> evaluated;
		MGPathItem *n;
		MGPathItem *e;

		MGPathItem *now = new MGPathItem(x, y, MGFramework::distance(x, y, x2, y2));
		path.push_back(*now);

		while (x != x2 || y != y2)
		{
			if(map.occupant(x2, y2) != 0)
			{
				// XXX: Here we should try to find a tile close to target instead of giving up.
				//      - Set a new bx,by close to tagret and continue instead of breaking?
				//      - Condition that the distance to target is not short?
				break;
			}

			if(x + 1 < map.getWidth() && y + 1 < map.getHeight() && !map.occupant(x + 1, y + 1))
			{
				n = new MGPathItem(x + 1, y + 1, distance(x + 1, y + 1, x2, y2));
				bool skip = false;
				for (std::list<MGPathItem>::iterator it = neighbors.begin(); it != neighbors.end(); ++it)
				{
					if(n->equalCoordinate(&(*it)))
					{
						skip = true;
					}
				}
				if(!skip) neighbors.push_front(*n);
			}
			if(x - 1 >= 0 && y - 1 >= 0 && !map.occupant(x - 1, y - 1))
			{
				n = new MGPathItem(x - 1, y - 1, distance(x - 1, y - 1, x2, y2));
				bool skip = false;
				for (std::list<MGPathItem>::iterator it = neighbors.begin(); it != neighbors.end(); ++it)
				{
					if(n->equalCoordinate(&(*it)))
					{
						skip = true;
					}
				}
				if(!skip) neighbors.push_front(*n);
			}
			if(x + 1 < map.getWidth() && y - 1 >= 0 && !map.occupant(x + 1, y - 1))
			{
				n = new MGPathItem(x + 1, y - 1, distance(x + 1, y - 1, x2, y2));
				bool skip = false;
				for (std::list<MGPathItem>::iterator it = neighbors.begin(); it != neighbors.end(); ++it)
				{
					if(n->equalCoordinate(&(*it)))
					{
						skip = true;
					}
				}
				if(!skip) neighbors.push_front(*n);
			}
			if(x - 1 >= 0 && y + 1 < map.getHeight() && !map.occupant(x - 1, y + 1))
			{
				n = new MGPathItem(x - 1, y + 1, distance(x - 1, y + 1, x2, y2));
				bool skip = false;
				for (std::list<MGPathItem>::iterator it = neighbors.begin(); it != neighbors.end(); ++it)
				{
					if(n->equalCoordinate(&(*it)))
					{
						skip = true;
					}
				}
				if(!skip) neighbors.push_front(*n);
			}
			if(y + 1 < map.getHeight() && !map.occupant(x, y + 1))
			{
				n = new MGPathItem(x, y + 1, distance(x, y + 1, x2, y2));
				bool skip = false;
				for (std::list<MGPathItem>::iterator it = neighbors.begin(); it != neighbors.end(); ++it)
				{
					if(n->equalCoordinate(&(*it)))
					{
						skip = true;
					}
				}
				if(!skip) neighbors.push_front(*n);
			}
			if(y - 1 >= 0 && !map.occupant(x, y - 1))
			{
				n = new MGPathItem(x, y - 1, distance(x, y - 1, x2, y2));
				bool skip = false;
				for (std::list<MGPathItem>::iterator it = neighbors.begin(); it != neighbors.end(); ++it)
				{
					if(n->equalCoordinate(&(*it)))
					{
						skip = true;
					}
				}
				if(!skip) neighbors.push_front(*n);
			}
			if(x + 1 < map.getWidth() && !map.occupant(x + 1, y))
			{
				n = new MGPathItem(x + 1, y, distance(x + 1, y, x2, y2));
				bool skip = false;
				for (std::list<MGPathItem>::iterator it = neighbors.begin(); it != neighbors.end(); ++it)
				{
					if(n->equalCoordinate(&(*it)))
					{
						skip = true;
					}
				}
				if(!skip) neighbors.push_front(*n);
			}
			if(x - 1 >= 0 && !map.occupant(x - 1, y))
			{
				n = new MGPathItem(x - 1, y, distance(x - 1, y, x2, y2));
				bool skip = false;
				for (std::list<MGPathItem>::iterator it = neighbors.begin(); it != neighbors.end(); ++it)
				{
					if(n->equalCoordinate(&(*it)))
					{
						skip = true;
					}
				}
				if(!skip) neighbors.push_front(*n);
			}

			if(!neighbors.empty())
			{
				int bestx = 0;
				int besty = 0;
				double besth = distance(0, 0, map.getWidth(), map.getHeight());
				bool found = false;

				// XXX: Set a limit for how many neighbors we will consider at the most, in each step. 
				for (std::list<MGPathItem>::iterator it = neighbors.begin(); it != neighbors.end(); ++it)
				{
					if((*it).getH()<besth)
					{
						// First we check if the found tile has already been evaluated.
						bool alreadyEvaluated = false;
						for (std::list<MGPathItem>::iterator ae = evaluated.begin(); ae != evaluated.end(); ++ae)
						{
							if((*ae).equalCoordinate(&(*it)))
							{
								alreadyEvaluated = true;
							}
						}

						if(!alreadyEvaluated)
						{
							// Add the current (x,y) to list of evaluated coordinates..
							e = new MGPathItem(x, y, MGFramework::distance(x, y, x2, y2));
							evaluated.push_back(*e);

							while(x < (*it).getX() - 1 || x > (*it).getX() + 1 || y < (*it).getY() - 1 || y > (*it).getY() + 1)
							{
								// The found tile is not a neighbor of {x,y}
								// Set H to a large number to not try this neighbor again.
								// Back-track.
								(*it).setH(map.getWidth() + map.getHeight());

								if(it != neighbors.begin())
								{
									--it;
									if(path.size() > 0)
									{
										path.pop_back();

										if(!path.empty())
										{
											x = path.back().getX();
											y = path.back().getY();
										}
										else
										{
											break;
										}
									}
								}
								else
								{
									break;
								}
							}

							bestx = (*it).getX();
							besty = (*it).getY();
							besth = (*it).getH();
							found = !(x < bestx - 1 || x > bestx + 1 || y < besty - 1 || y > besty + 1);
						}
					}
				}

				if(found)
				{
					if(x < bestx - 1 || x > bestx + 1 || y < besty - 1 || y > besty + 1)
					{
						// XXX: Is this really necessary?
						MGFLOG_STATIC_ERROR("MGMap::calculatePath suggested a non-neighbor as next step in path");
						path.clear();
						break;
					}
					MGPathItem *pI = new MGPathItem(bestx, besty, MGFramework::distance(bestx, besty, x2, y2));
					path.push_back(*pI);
					x = bestx;
					y = besty;
				}
				else
				{
					// XXX: Should we really give up here? What about back tracking?
					//      Yes i think so. We have already tried to back-track...

					// XXX: Why is this only ok one tile away from target? What if a cluster of tiles is occupied?
					if(MGFramework::distance(x, y, x2, y2) < 2 && map.occupant(x2, y2) != 0)
					{
						// The tile we want to go to is occopied and we are one tile away
					}
					else
					{

					}
					break;
				}
			}
			else
			{
				//MGFLOG_WARNING("MGMap::calculatePath was not able to find a path as there are no available neighbor tiles to evaluate");
				if( (x + 1 < map.getWidth() && y + 1 < map.getHeight() && !map.occupant(x + 1, y + 1)) ||
					(x - 1 >= 0 && y - 1 >= 0 && !map.occupant(x - 1, y - 1)) ||
					(x + 1 < map.getWidth() && y - 1 >= 0 && !map.occupant(x + 1, y - 1)) ||
					(x - 1 >= 0 && y + 1 < map.getHeight() && !map.occupant(x - 1, y + 1)) ||
					(y + 1 < map.getHeight() && !map.occupant(x, y + 1)) ||
					(y - 1 >= 0 && !map.occupant(x, y - 1)) ||
					(x + 1 < map.getWidth() && !map.occupant(x + 1, y)) ||
					(x - 1 >= 0 && !map.occupant(x - 1, y)) )
				{
					MGFLOG_STATIC_ERROR("MGMap::calculatePath stopped calculation without valid reason");
				}
				break;
			}

			// Some additional checks..
			if(neighbors.size() > 100)
			{
				// Purging when "too" many neighbors
				size_t nToPurge = neighbors.size() / 2;
				for(int i = 0; i < (int)nToPurge; ++i)
				{
					neighbors.pop_back();
				}
			}
			if(path.size() > 1000)
			{
				// XXX: This is not necessarily an error but keep it like that for now so it stands out more...
				MGFLOG_STATIC_ERROR("MGMap::calculatePath created a too long path, aborting");
				path.clear();
				break;
			}
		}//while
}
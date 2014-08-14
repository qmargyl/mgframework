
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
	int yMax = map.getHeight() -1;

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
		std::list<MGAStarNode> successors;
		int x = nodeCurrent.getX();
		int y = nodeCurrent.getY();
		if(x < xMax && y < yMax && map.occupant(x + 1, y + 1) == 0)
		{
			successors.push_back(MGAStarNode(x + 1, y + 1, nodeGoal, nodeCurrent.getG() + sqrt_2));
			successorAdditions++;
		}
		if(x < xMax && map.occupant(x + 1, y) == 0)
		{
			successors.push_back(MGAStarNode(x + 1, y, nodeGoal, nodeCurrent.getG() + 1.0));
			successorAdditions++;
		}
		if(y < yMax && map.occupant(x, y + 1) == 0)
		{
			successors.push_back(MGAStarNode(x, y + 1, nodeGoal, nodeCurrent.getG() + 1.0));
			successorAdditions++;
		}
		if(x > xMin && y > yMin && map.occupant(x - 1, y - 1) == 0)
		{
			successors.push_back(MGAStarNode(x - 1, y - 1, nodeGoal, nodeCurrent.getG() + sqrt_2));
			successorAdditions++;
		}
		if(x > xMin && map.occupant(x - 1, y) == 0)
		{
			successors.push_back(MGAStarNode(x - 1, y, nodeGoal, nodeCurrent.getG() + 1.0));
			successorAdditions++;
		}
		if(y > yMin && map.occupant(x, y - 1) == 0)
		{
			successors.push_back(MGAStarNode(x, y - 1, nodeGoal, nodeCurrent.getG() + 1.0));
			successorAdditions++;
		}
		if(x < xMax && y > yMin && map.occupant(x + 1, y - 1) == 0)
		{
			successors.push_back(MGAStarNode(x + 1, y - 1, nodeGoal, nodeCurrent.getG() + sqrt_2));
			successorAdditions++;
		}
		if(x > xMin && y < yMax && map.occupant(x - 1, y + 1) == 0)
		{
			successors.push_back(MGAStarNode(x - 1, y + 1, nodeGoal, nodeCurrent.getG() + sqrt_2));
			successorAdditions++;
		}
		
		// Step 8 (and 11) done!!

		for(std::list<MGAStarNode>::iterator nodeSuccessor = successors.begin(); nodeSuccessor != successors.end(); )
		{
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
			
			// Step 16 done!!
			
			(*nodeSuccessor).setH((*nodeSuccessor).heuristic(nodeGoal));
			
			// Step 17 done!!
			
			open.push_back(*nodeSuccessor);
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
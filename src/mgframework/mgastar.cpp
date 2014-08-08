

// Algorithm pseudo code taken from http://heyes-jones.com/pseudocode.php
// Step --- Description
// 1        Create a node containing the goal state node_goal 
// 2        Create a node containing the start state node_start 
// 3        Put node_start on the open list 
// 4        While the OPEN list is not empty 
// 5        { 
// 6            Get the node off the open list with the lowest f and call it node_current 
// 7            If node_current is the same state as node_goal we have found the solution; break from the while loop 
// 8            Generate each state node_successor that can come after node_current 
// 9            For each node_successor of node_current 
// 10           { 
// 11               Set the cost of node_successor to be the cost of node_current plus the cost to get to node_successor from node_current 
// 12               Find node_successor on the OPEN list 
// 13               If node_successor is on the OPEN list but the existing one is as good or better then discard this successor and continue 
// 14               If node_successor is on the CLOSED list but the existing one is as good or better then discard this successor and continue 
// 15               Remove occurrences of node_successor from OPEN and CLOSED 
// 16               Set the parent of node_successor to node_current 
// 17               Set h to be the estimated distance to node_goal (Using the heuristic function) 
// 18               Add node_successor to the OPEN list 
// 19           } 
// 20           Add node_current to the CLOSED list 
// 21       } 


#define sqrt_2 (1.414213562373095)

class MGAStarNode
{
	private:
		int m_X;
		int m_Y;
		int m_ParentX;
		int m_ParentY;
		double m_H;
		double m_G;

	public:
		
		MGAStarNode(int x, int y, const MGAStarNode& goal, double g = 0.0)
		: m_X(x),
		  m_Y(y),
		  m_ParentX(x),
		  m_ParentY(y),
		  m_H(0.0),
		  m_G(g)
		{
			setH(heuristic(goal));
		}		
		
		MGAStarNode(const MGAStarNode& node)
		: m_X(node.getX()),
		  m_Y(node.getY()),
		  m_ParentX(node.getParentX()),
		  m_ParentY(node.getParentY()),
		  m_H(node.getH()),
		  m_G(node.getG())
		{
		
		}
		
		// True if the states of @this and @node are equal
		bool operator==(const MGAStarNode& node)
		{
			return m_X == node.getX() && m_Y == node.getY();
		}
		
		const int getX() const { return m_X; }
		const int getY() const { return m_Y; }
		const double getF() const { return getG() + getH(); }
		const double getH() const { return m_H; }
		const double getG() const { return m_G; }
		const int getParentX() const { return m_ParentX; }
		const int getParentY() const { return m_ParentY; }
		
		//void setG(double g){ m_G = g; }
		void setH(double h){ m_H = h; }
		
		// Under-estimate the remaining path length from @this to @node
		double heuristic(const MGAStarNode &node)
		{
			// Pythagorean Theorem
			int dx = getX() - node.getX();
			int dy = getY() - node.getY();
			return sqrt(dx*dx+dy*dy);
		}
		
		void setParent(MGAStarNode &node)
		{
			m_ParentX = node.getX();
			m_ParentY = node.getY();
		}
};

void calculateAStar(int x1, int y1, int x2, int y2, int xMin, int yMin, int xMax, int yMax, int map[][9], std::list<MGAStarNode> &path)
{
	std::list<MGAStarNode> open;
	std::list<MGAStarNode> closed;
	MGAStarNode nodeGoal(x2, y2, nodeGoal, 0.0);
	MGAStarNode nodeStart(x1, y1, nodeGoal, 0.0);
	open.push_back(nodeStart);

	int passes = 0;
	int deletions = 0;
	int additions = 0;
	int lookups = 0;
	bool pathWasFound = false;

	while(!open.empty())
	{
		passes++;
	
		// Take the node with smallest F from open list and call it nodeCurrent
		std::list<MGAStarNode>::iterator nodeCurrentIt = open.begin();
		lookups++;
		for(std::list<MGAStarNode>::iterator nodeIt = open.begin(); nodeIt != open.end(); nodeIt++)
		{
			if((*nodeIt).getF() < (*nodeCurrentIt).getF())
			{
				nodeCurrentIt = nodeIt;
			}
		}
		MGAStarNode nodeCurrent(*nodeCurrentIt);
		deletions++;
		open.erase(nodeCurrentIt);
		
		// Step 6 done!!

		if(nodeCurrent == nodeGoal)
		{
			pathWasFound = true;
			additions++;
			closed.push_back(nodeCurrent);
			printf(	"Path found (%d,%d) -> (%d,%d) in %d passes, %d deletions, %d additions, %d lookups!!\n", 
					nodeStart.getX(), nodeStart.getY(), nodeGoal.getX(), nodeGoal.getY(), passes, deletions, additions, lookups);
			for(std::list<MGAStarNode>::iterator nodeIt = closed.begin(); nodeIt != closed.end(); nodeIt++)
			{
				printf("(%d,%d) <- (%d,%d)\n", (*nodeIt).getX(), (*nodeIt).getY(), (*nodeIt).getParentX(), (*nodeIt).getParentY());
			}
			// TODO: Save the actual path in @path. Start from $closed(last) and follow it to @nodeGoal
			break;
		}
		
		// Step 7 done!!
		
		// Generate successors (neighbors)
		std::list<MGAStarNode> successors;
		int x = nodeCurrent.getX();
		int y = nodeCurrent.getY();
		if(x < xMax && y < yMax && map[y + 1][x + 1] == 0)
		{
			successors.push_back(MGAStarNode(x + 1, y + 1, nodeGoal, nodeCurrent.getG() + sqrt_2));
			additions++;
		}
		if(x < xMax && map[y][x + 1] == 0)
		{
			successors.push_back(MGAStarNode(x + 1, y, nodeGoal, nodeCurrent.getG() + 1.0));
			additions++;
		}
		if(y < yMax && map[y + 1][x] == 0)
		{
			successors.push_back(MGAStarNode(x, y + 1, nodeGoal, nodeCurrent.getG() + 1.0));
			additions++;
		}
		if(x > xMin && y > yMin && map[y - 1][x - 1] == 0)
		{
			successors.push_back(MGAStarNode(x - 1, y - 1, nodeGoal, nodeCurrent.getG() + sqrt_2));
			additions++;
		}
		if(x > xMin && map[y][x - 1] == 0)
		{
			successors.push_back(MGAStarNode(x - 1, y, nodeGoal, nodeCurrent.getG() + 1.0));
			additions++;
		}
		if(y > yMin && map[y - 1][x] == 0)
		{
			successors.push_back(MGAStarNode(x, y - 1, nodeGoal, nodeCurrent.getG() + 1.0));
			additions++;
		}
		if(x < xMax && y > yMin && map[y - 1][x + 1] == 0)
		{
			successors.push_back(MGAStarNode(x + 1, y - 1, nodeGoal, nodeCurrent.getG() + sqrt_2));
			additions++;
		}
		if(x > xMin && y < yMax && map[y + 1][x - 1] == 0)
		{
			successors.push_back(MGAStarNode(x - 1, y + 1, nodeGoal, nodeCurrent.getG() + sqrt_2));
			additions++;
		}
		
		// Step 8 (and 11) done!!

		for(std::list<MGAStarNode>::iterator nodeSuccessor = successors.begin(); nodeSuccessor != successors.end(); )
		{
			// Find the successor in the open list
			lookups++;
			std::list<MGAStarNode>::iterator evaluateOpenIt = find(open.begin(), open.end(), *nodeSuccessor);
			if(evaluateOpenIt != open.end() && (*evaluateOpenIt).getF() <= (*nodeSuccessor).getF())
			{
				nodeSuccessor = successors.erase(nodeSuccessor);
				deletions++;
				continue;
			}			
			
			// Step 13 done!!
			
			// Find the successor in the closed list
			lookups++;
			std::list<MGAStarNode>::iterator evaluateClosedIt = find(closed.begin(), closed.end(), *nodeSuccessor);
			if(evaluateClosedIt != closed.end() && (*evaluateClosedIt).getF() <= (*nodeSuccessor).getF())
			{
				nodeSuccessor = successors.erase(nodeSuccessor);
				deletions++;
				continue;
			}
			
			// Step 14 done!!
			
			// Remove the successor from the open list, if present
			lookups++;
			std::list<MGAStarNode>::iterator removeOpenIt = find(open.begin(), open.end(), *nodeSuccessor);
			if(removeOpenIt != open.end())
			{
				open.erase(removeOpenIt);
				deletions++;
			}
			
			// Remove the successor from the closed list, if present
			lookups++;
			std::list<MGAStarNode>::iterator removeClosedIt = find(closed.begin(), closed.end(), *nodeSuccessor);
			if(removeClosedIt != closed.end())
			{
				closed.erase(removeClosedIt);
				deletions++;
			}			

			// Step 15 done!!
			
			(*nodeSuccessor).setParent(nodeCurrent);
			
			// Step 16 done!!
			
			(*nodeSuccessor).setH((*nodeSuccessor).heuristic(nodeGoal));
			
			// Step 17 done!!
			
			open.push_back(*nodeSuccessor);
			additions++;
			
			nodeSuccessor++;
		}
		
		closed.push_back(nodeCurrent);
		additions++;
	}
	
	if(!pathWasFound)
	{
		printf(	"Path not found (%d,%d) -> (%d,%d) in %d passes, %d deletions, %d additions, %d lookups!!\n",
				nodeStart.getX(), nodeStart.getY(), nodeGoal.getX(), nodeGoal.getY(), passes, deletions, additions, lookups);
		path.clear();
	}
}

int main()
{
	int map[9][9] = {
						{0, 0, 0, 0, 0, 1, 0, 0, 0},
						{0, 0, 0, 0, 0, 1, 0, 0, 0},
						{0, 1, 0, 0, 0, 1, 0, 0, 0},
						{1, 1, 0, 1, 1, 1, 0, 1, 1},
						{0, 0, 0, 1, 0, 0, 0, 1, 0},
						{0, 1, 1, 1, 0, 0, 0, 1, 0},
						{0, 0, 0, 0, 0, 0, 1, 1, 1},
						{0, 0, 0, 0, 1, 1, 1, 1, 1},
						{0, 0, 0, 0, 1, 1, 1, 1, 0}
					};

	std::list<MGAStarNode> path;
	calculateAStar(1,1,6,4,0,0,8,8, map, path);
	path.clear();
	calculateAStar(8,1,4,2,0,0,8,8, map, path);
	path.clear();
	calculateAStar(8,4,8,8,0,0,8,8, map, path); // no path available
	path.clear();
}




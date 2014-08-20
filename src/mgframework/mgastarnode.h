#ifndef _MGASTARNODE_H
#define _MGASTARNODE_H

#include <cmath>
#include <algorithm>

#ifndef sqrt_2
#define sqrt_2 (1.414213562373095)
#endif

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

		MGAStarNode(int x, int y, double g = 0.0)
		: m_X(x),
		  m_Y(y),
		  m_ParentX(x),
		  m_ParentY(y),
		  m_H(0.0),
		  m_G(g)
		{

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
		
		void setH(double h){ m_H = h; }
		
		// Under-estimate the remaining path length from @this to @node
		/*
		double heuristic(const MGAStarNode &node)
		{
			// Pythagorean Theorem
			int dx = getX() - node.getX();
			int dy = getY() - node.getY();
			return std::sqrt((double)(dx*dx+dy*dy));
		}
		*/

		double heuristic(const MGAStarNode &node)
		{
			// Actual distance when there are no obstacles between start and goal
			int dx = abs(getX() - node.getX());
			int dy = abs(getY() - node.getY());
			int dxy = std::max(dx, dy) - std::min(dx, dy);
			return std::min(dx, dy) * sqrt_2 + dxy * 1.0;
		}
		
		void setParent(MGAStarNode &node)
		{
			m_ParentX = node.getX();
			m_ParentY = node.getY();
		}
};

#endif
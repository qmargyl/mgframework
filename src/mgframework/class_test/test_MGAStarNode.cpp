#include "../../project2_test.h"
#include "../mgastarnode.h"

void Project2Test::test_MGAStarNode()
{
	// Construct node 1
	MGAStarNode node1(3, 4, 2.0);
	ASSERT_EQ(node1.getX(), 3, "Node returns incorrect X");
	ASSERT_EQ(node1.getY(), 4, "Node returns incorrect Y");
	ASSERT_EQ(node1.getG(), 2.0, "Node returns incorrect G");
	ASSERT_EQ(node1.getH(), 0.0, "Node returns incorrect H");
	ASSERT_EQ(node1.getF(), 2.0, "Node returns incorrect F");
	ASSERT_EQ(node1.getParentX(), 3, "Node returns incorrect ParentX");
	ASSERT_EQ(node1.getParentY(), 4, "Node returns incorrect ParentY");

	// Construct node 2
	MGAStarNode node2(3, 5, node1, 3.0);
	ASSERT_EQ(node2.getX(), 3, "Node returns incorrect X");
	ASSERT_EQ(node2.getY(), 5, "Node returns incorrect Y");
	ASSERT_EQ(node2.getG(), 3.0, "Node returns incorrect G");
	ASSERT_EQ(node2.getH(), 1.0, "Node returns incorrect H");
	ASSERT_EQ(node2.getF(), 4.0, "Node returns incorrect F");
	ASSERT_EQ(node2.getParentX(), 3, "Node returns incorrect ParentX");
	ASSERT_EQ(node2.getParentY(), 5, "Node returns incorrect ParentY");

	// Copy node 2 into node 3
	MGAStarNode node3(node2);
	ASSERT_EQ(node3.getX(), 3, "Node returns incorrect X");
	ASSERT_EQ(node3.getY(), 5, "Node returns incorrect Y");
	ASSERT_EQ(node3.getG(), 3.0, "Node returns incorrect G");
	ASSERT_EQ(node3.getH(), 1.0, "Node returns incorrect H");
	ASSERT_EQ(node3.getF(), 4.0, "Node returns incorrect F");
	ASSERT_EQ(node3.getParentX(), 3, "Node returns incorrect ParentX");
	ASSERT_EQ(node3.getParentY(), 5, "Node returns incorrect ParentY");
//	ASSERT_EQ(node3 == node2, true, "Copied node not equal to source");

	// Test the heuristic
	MGAStarNode node4(5, 5);
	ASSERT_EQ(node4.heuristic(MGAStarNode(5, 4)), 1.0, "Heuristic returned incorrect result");
	ASSERT_EQ(node4.heuristic(MGAStarNode(5, 6)), 1.0, "Heuristic returned incorrect result");
	ASSERT_EQ(node4.heuristic(MGAStarNode(4, 5)), 1.0, "Heuristic returned incorrect result");
	ASSERT_EQ(node4.heuristic(MGAStarNode(6, 5)), 1.0, "Heuristic returned incorrect result");

	ASSERT_EQ(node4.heuristic(MGAStarNode(5, 1)), 4.0, "Heuristic returned incorrect result");
	ASSERT_EQ(node4.heuristic(MGAStarNode(5, 15)), 10.0, "Heuristic returned incorrect result");
	ASSERT_EQ(node4.heuristic(MGAStarNode(2, 5)), 3.0, "Heuristic returned incorrect result");
	ASSERT_EQ(node4.heuristic(MGAStarNode(95, 5)), 90.0, "Heuristic returned incorrect result");

	ASSERT_EQ(node4.heuristic(MGAStarNode(5, -5)), 10.0, "Heuristic returned incorrect result");
	ASSERT_EQ(node4.heuristic(MGAStarNode(5, -15)), 20.0, "Heuristic returned incorrect result");
	ASSERT_EQ(node4.heuristic(MGAStarNode(-2, 5)), 7.0, "Heuristic returned incorrect result");
	ASSERT_EQ(node4.heuristic(MGAStarNode(-95, 5)), 100.0, "Heuristic returned incorrect result");
}
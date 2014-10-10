#include "../../project2_test.h"
#include "../mgpathgenerator.h"
#include "../mgpathitem.h"
#include "../mgmap.h"
#include <list>

void Project2Test::test_MGPathGenerator_calculatePathAStar()
{
	// Setup
	std::list<MGPathItem> path;
	MGMap map;
	map.init(10, 10, 16, 16, 640, 480);

	// Diagonal
	MGPathGenerator::calculatePathAStar(1, 1, 9, 9, map, path);
	ASSERT_NOT_EQUAL(path.size(), 9, "Diagonal path in open terrain has incorrect length");
	path.clear();
	ASSERT_NOT_EQUAL(path.size(), 0, "Path is non-empty");

	// Diagonal
	MGPathGenerator::calculatePathAStar(9, 9, 1, 1, map, path);
	ASSERT_NOT_EQUAL(path.size(), 9, "Diagonal path in open terrain has incorrect length");
	path.clear();
	ASSERT_NOT_EQUAL(path.size(), 0, "Path is non-empty");

	// Diagonal
	MGPathGenerator::calculatePathAStar(1, 9, 9, 1, map, path);
	ASSERT_NOT_EQUAL(path.size(), 9, "Diagonal path in open terrain has incorrect length");
	path.clear();
	ASSERT_NOT_EQUAL(path.size(), 0, "Path is non-empty");

	// Diagonal
	MGPathGenerator::calculatePathAStar(9, 1, 1, 9, map, path);
	ASSERT_NOT_EQUAL(path.size(), 9, "Diagonal path in open terrain has incorrect length");
	path.clear();
	ASSERT_NOT_EQUAL(path.size(), 0, "Path is non-empty");

	// Almost horizontal
	MGPathGenerator::calculatePathAStar(1, 4, 9, 5, map, path);
	ASSERT_NOT_EQUAL(path.size(), 9, "Path in open terrain has incorrect length");
	path.clear();
	ASSERT_NOT_EQUAL(path.size(), 0, "Path is non-empty");

	// Horizontal
	MGPathGenerator::calculatePathAStar(2, 2, 7, 2, map, path);
	ASSERT_NOT_EQUAL(path.size(), 6, "Horizontal path in open terrain has incorrect length");
	path.clear();
	ASSERT_NOT_EQUAL(path.size(), 0, "Path is non-empty");

	// Horizontal
	MGPathGenerator::calculatePathAStar(6, 5, 1, 5, map, path);
	ASSERT_NOT_EQUAL(path.size(), 6, "Horizontal path in open terrain has incorrect length");
	path.clear();
	ASSERT_NOT_EQUAL(path.size(), 0, "Path is non-empty");

	// Vertical
	MGPathGenerator::calculatePathAStar(4, 9, 4, 0, map, path);
	ASSERT_NOT_EQUAL(path.size(), 10, "Vertical path in open terrain has incorrect length");
	path.clear();
	ASSERT_NOT_EQUAL(path.size(), 0, "Path is non-empty");

	// 5,5->7,7
	MGPathGenerator::calculatePathAStar(5, 5, 7, 7, map, path);
	ASSERT_NOT_EQUAL(path.size(), 3, "5,5->7,7 path in open terrain has incorrect length");
	path.clear();
	ASSERT_NOT_EQUAL(path.size(), 0, "Path is non-empty");

	// Large map
	map.reInit(100, 100, 16, 16);

	// 5,5->7,8
	MGPathGenerator::calculatePathAStar(5, 5, 7, 8, map, path);
	ASSERT_NOT_EQUAL(path.size(), 4, "5,5->7,8 path in open terrain has incorrect length");
	path.clear();
	ASSERT_NOT_EQUAL(path.size(), 0, "Path is non-empty");

	// Long path
	MGPathGenerator::calculatePathAStar(5, 11, 93, 94, map, path);
	ASSERT_NOT_EQUAL(path.size(), 89, "Long path in open terrain has incorrect length");
	path.clear();
	ASSERT_NOT_EQUAL(path.size(), 0, "Path is non-empty");
}

void Project2Test::test_MGPathGenerator_calculatePathAStarAroundObstacle()
{
	// Setup
	std::list<MGPathItem> path;
	MGMap map;
	map.init(10, 10, 16, 16, 640, 480);
	map.occupy(5, 1, 17);
	map.occupy(5, 2, 17);
	map.occupy(5, 3, 17);
	map.occupy(5, 4, 17);
	map.occupy(5, 5, 17);
	map.occupy(5, 6, 17);
	map.occupy(5, 7, 17);
	map.occupy(5, 8, 17);

	// Trigger
	MGPathGenerator::calculatePathAStar(3, 5, 8, 5, map, path);

	// Verify
	ASSERT_NOT_EQUAL(path.front().getX(), 3, "Incorrect path content");
	ASSERT_NOT_EQUAL(path.front().getY(), 5, "Incorrect path content");
	ASSERT_NOT_EQUAL(path.back().getX(), 8, "Incorrect path content");
	ASSERT_NOT_EQUAL(path.back().getY(), 5, "Incorrect path content");
	ASSERT_NOT_EQUAL(path.size() > 7, true, "Path is corrupt");
}

void Project2Test::test_MGPathGenerator_calculatePathAStarUnreachable()
{
	// Setup
	std::list<MGPathItem> path;
	MGMap map;
	map.init(10, 10, 16, 16, 640, 480);
	map.occupy(2, 2, 17);
	map.occupy(3, 2, 17);
	map.occupy(4, 2, 17);
	map.occupy(2, 3, 17);
	map.occupy(4, 3, 17);
	map.occupy(2, 4, 17);
	map.occupy(3, 4, 17);
	map.occupy(4, 4, 17);

	// Trigger
	MGPathGenerator::calculatePathAStar(7, 7, 3, 3, map, path);

	// Verify
	ASSERT_NOT_EQUAL(path.size(), 0, "Path to unreachable tile is non-empty");
}

#include "../mgclasstester.h"
#include "../mgpathgenerator.h"
#include "../mgpathitem.h"
#include "../mgmap.h"
#include <list>

void MGClassTester::test_MGPathGenerator_calculatePathAStar()
{
	std::list<MGPathItem> path;
	MGMap map;

	map.init(10, 10, 16, 16, 640, 480);

	// Diagonal
	MGPathGenerator::calculatePathAStar(1, 1, 9, 9, map, path);
	ASSERT_NOT_EQUAL(path.size(), 8, "Diagonal path in open terrain has incorrect length");
	path.clear();
	ASSERT_NOT_EQUAL(path.size(), 0, "Path is non-empty");

	// Diagonal
	MGPathGenerator::calculatePathAStar(9, 9, 1, 1, map, path);
	ASSERT_NOT_EQUAL(path.size(), 8, "Diagonal path in open terrain has incorrect length");
	path.clear();
	ASSERT_NOT_EQUAL(path.size(), 0, "Path is non-empty");

	// Diagonal
	MGPathGenerator::calculatePathAStar(1, 9, 9, 1, map, path);
	ASSERT_NOT_EQUAL(path.size(), 8, "Diagonal path in open terrain has incorrect length");
	path.clear();
	ASSERT_NOT_EQUAL(path.size(), 0, "Path is non-empty");

	// Diagonal
	MGPathGenerator::calculatePathAStar(9, 1, 1, 9, map, path);
	ASSERT_NOT_EQUAL(path.size(), 8, "Diagonal path in open terrain has incorrect length");
	path.clear();
	ASSERT_NOT_EQUAL(path.size(), 0, "Path is non-empty");

	// Almost horizontal
	MGPathGenerator::calculatePathAStar(1, 4, 9, 5, map, path);
	ASSERT_NOT_EQUAL(path.size(), 8, "Path in open terrain has incorrect length");
	path.clear();
	ASSERT_NOT_EQUAL(path.size(), 0, "Path is non-empty");

	// Horizontal
	MGPathGenerator::calculatePathAStar(2, 2, 7, 2, map, path);
	ASSERT_NOT_EQUAL(path.size(), 5, "Horizontal path in open terrain has incorrect length");
	path.clear();
	ASSERT_NOT_EQUAL(path.size(), 0, "Path is non-empty");

	// Horizontal
	MGPathGenerator::calculatePathAStar(6, 5, 1, 5, map, path);
	ASSERT_NOT_EQUAL(path.size(), 5, "Horizontal path in open terrain has incorrect length");
	path.clear();
	ASSERT_NOT_EQUAL(path.size(), 0, "Path is non-empty");

	// Vertical
	MGPathGenerator::calculatePathAStar(4, 9, 4, 0, map, path);
	ASSERT_NOT_EQUAL(path.size(), 9, "Vertical path in open terrain has incorrect length");
	path.clear();
	ASSERT_NOT_EQUAL(path.size(), 0, "Path is non-empty");

	// 5,5->7,7
	MGPathGenerator::calculatePathAStar(5, 5, 7, 7, map, path);
	ASSERT_NOT_EQUAL(path.size(), 2, "5,5->7,7 path in open terrain has incorrect length");
	path.clear();
	ASSERT_NOT_EQUAL(path.size(), 0, "Path is non-empty");

	// Large map
	map.reInit(100, 100, 16, 16);

	// 5,5->7,8
	MGPathGenerator::calculatePathAStar(5, 5, 7, 8, map, path);
	ASSERT_NOT_EQUAL(path.size(), 3, "5,5->7,8 path in open terrain has incorrect length");
	path.clear();
	ASSERT_NOT_EQUAL(path.size(), 0, "Path is non-empty");

	// Long path
	MGPathGenerator::calculatePathAStar(5, 11, 93, 94, map, path);
	ASSERT_NOT_EQUAL(path.size(), 88, "Long path in open terrain has incorrect length");
	path.clear();
	ASSERT_NOT_EQUAL(path.size(), 0, "Path is non-empty");
}
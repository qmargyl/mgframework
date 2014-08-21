#ifndef _MG_CLASSTESTER_H
#define _MG_CLASSTESTER_H

#include <iostream>

#define MGCLASSTESTER_LOGLINE_MAXLENGTH 1024

#define ASSERT_NOT_EQUAL(x, y, msg)	do{ \
		if(x != y){ \
			std::cout << "[ASSERT] " << __FILE__ << ":" << __LINE__ << " MSG: " << msg << ", " << x << ", " << y << std::endl; \
			return; \
		} \
	} while(0)

#define RUNTEST(f) do{ \
		std::cout << "[TC] " << #f << std::endl; \
		f ## (); \
	} while(0)

#define FINISH_TESTSUITE() do{ \
		std::cout << "Exiting application..." << std::endl; \
	} while(0)



class MGClassTester
{
private:
	static void test_MGComponent();
	static void test_MGMap_initialize();
	static void test_MGAStarNode();
	static void test_MGPathGenerator_calculatePathAStar();


public:
	static void runAll()
	{
		RUNTEST(test_MGComponent);
		RUNTEST(test_MGMap_initialize);
		RUNTEST(test_MGAStarNode);
		RUNTEST(test_MGPathGenerator_calculatePathAStar);
		FINISH_TESTSUITE();
	}

	static void logEval(const char *logFileName, bool negativeTest);
};

#endif
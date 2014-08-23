#ifndef _MG_CLASSTESTER_H
#define _MG_CLASSTESTER_H

#include <iostream>
#include <string>

#define MGCLASSTESTER_LOGLINE_MAXLENGTH 1024

#define ASSERT_NOT_EQUAL(x, y, msg)	do{ \
		if(x != y){ \
			std::cout << "[ASSERT] " << __FILE__ << ":" << __LINE__ << " MSG: " << msg << ", " << x << ", " << y << std::endl; \
			return; \
		} \
	} while(0)

#ifndef UNITTEST_LINUX
  #define RUNTEST(f) do{ \
		std::cout << "[TC] " << #f << std::endl; \
		f ## (); \
	} while(0)
#else
  #define RUNTEST(f) do{ \
		std::cout << "[TC] " << #f << std::endl; \
		f(); \
	} while(0)
#endif

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

	// Log filtering/comparing/evaluation
	static void logEval(std::string logFileName, bool negativeTest);
	static std::string filterLine(const char* line);
	static void logFilter(std::string logFileName);
	static void logCompare(std::string logFileName1, std::string logFileName2);
};

#endif

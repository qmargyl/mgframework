#ifndef _MG_CLASSTESTER_H
#define _MG_CLASSTESTER_H

#include <iostream>

#define ASSERT_NOT_EQUAL(x, y, msg)	do{ \
		if(x != y){ \
			std::cout << "[ASSERT] " << __FILE__ << ":" << __LINE__ << " ERROR: " << msg << std::endl; \
			return; \
		} \
	} while(0)

#define RUNTEST(f) do{ \
		std::cout << "[TC] " << #f << std::endl; \
		f ## (); \
	} while(0)


class MGClassTester
{
private:
	static void test_MGMap_initialize();
	static void test_MGPathGenerator_calculatePathAStar();


public:
	static void runAll()
	{
		RUNTEST(test_MGMap_initialize);
		RUNTEST(test_MGPathGenerator_calculatePathAStar);
	}
};

#endif
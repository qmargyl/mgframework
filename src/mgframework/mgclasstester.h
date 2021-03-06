#ifndef _MG_CLASSTESTER_H
#define _MG_CLASSTESTER_H

#include <iostream>
#include <string>

#define MGCLASSTESTER_LOGLINE_MAXLENGTH 1024

#define ASSERT_EQ(x, y, msg) do{ \
		if((x) != (y)){ \
			std::cout << "[ASSERT] " << __FILE__ << ":" << __LINE__ << " MSG: " << msg << ", " << (x) << ", " << (y) << std::endl; \
			return; \
		} \
	} while(0)

#define RUNTEST(f) do{ \
		std::cout << "[TC] " << #f << std::endl; \
		f(); \
	} while(0)

#define FINISH_TESTSUITE() do{ \
		std::cout << "Exiting application..." << std::endl; \
	} while(0)

#define EXPECT_ERROR(f) do{ \
		std::cout << "[EE]" << f << std::endl; \
	} while(0)

class MGClassTester
{
private:

public:
	virtual void runAll() const = 0;

	// Log filtering/comparing/evaluation
	static void logEval(std::string logFileName);
	static std::string filterLine(const char* line);
	static void logFilter(std::string logFileName);
	static void logCompare(std::string logFileName1, std::string logFileName2);
};

#endif

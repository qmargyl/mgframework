#include "mgclasstester.h"
#include <vector>
#include <string>

void MGClassTester::logEval(const char *logFileName, bool negativeTest)
{
	FILE *lf = NULL;
	errno_t logError = fopen_s(&lf, logFileName, "rt");

	int nErrors = 0;
	int nWarnings = 0;
	int nExitApp = 0;
	int nAsserts = 0;
	std::string execTimeMS("");

	// Error list for printing
	std::vector<std::string> errors;
	errors.clear();

	if(lf == NULL)
	{
		std::cout << "ERROR: logEval failed to open log file " << logFileName << ", error(" << logError << ")" << std::endl;
	}
	else
	{
		std::cout << "Evaluating " << logFileName << " ... ";
		char logLine[MGCLASSTESTER_LOGLINE_MAXLENGTH] = "";
		char *neof = NULL;

		std::string errSubstr("ERROR");
		std::string warnSubstr("WARNING");
		std::string exitSubstr("Exiting application...");
		std::string tcSubstr("[TC]");
		std::string assertSubstr("[ASSERT]");

		while(true)
		{
			// Read until new line or end of file, whichever happens first..
			neof = fgets(logLine, MGCLASSTESTER_LOGLINE_MAXLENGTH, lf);
			if(neof == NULL)
			{
				break;
			}
			else
			{
				// An error defined as a log line containing at least one "ERROR".
				std::string line(logLine);

				std::size_t foundErr = line.find(errSubstr);
				std::size_t foundWarn = line.find(warnSubstr);
				std::size_t foundExit = line.find(exitSubstr);
				std::size_t foundTC = line.find(tcSubstr);
				std::size_t foundAssert = line.find(assertSubstr);

				if (foundErr != std::string::npos)
				{
					nErrors++;
					errors.push_back(line);
				}

				if (foundWarn != std::string::npos)
				{
					nWarnings++;
				}

				if (foundExit != std::string::npos)
				{
					nExitApp++;
				}

				if (foundTC != std::string::npos)
				{
					std::cout << "<br>" << line.c_str() << std::endl;
				}

				if (foundAssert != std::string::npos)
				{
					nAsserts++;
					std::cout << "<br><font color=red>" << line.c_str() << "</font>" << std::endl;
				}



				std::string exetimeSubstr("Execution time: ");
				std::size_t foundExecutionTime = line.find(exetimeSubstr);
				if (foundExecutionTime != std::string::npos)
				{
					execTimeMS = line.substr(exetimeSubstr.size(), line.size() - exetimeSubstr.size() -1);
					//C++11: int execTimeMS = std::stoi(execTime);
				}
			}
		}

		if(nErrors != 0 && !negativeTest)
		{
			std::cout << "<b><font color=red>FAIL</font></b> (" << nErrors << " errors, " << nWarnings << " warnings)";
		}
		else if(nExitApp == 0)
		{
			std::cout << "<b><font color=red>FAIL</font></b> (did not finish)";
		}
		else if(nAsserts != 0)
		{
			std::cout << "<br><b><font color=red>FAIL</font></b> (" << nAsserts << " asserts, " << nWarnings << " warnings)";
		}
		else
		{
			std::cout << "<b><font color=green>PASS</font></b>";
			// Also handle PASS for negative tests - count errors
			if(nWarnings > 0 || nErrors > 0)
			{
				std::cout << " (";
				if(nErrors != 0 && negativeTest)
				{
					std::cout << nErrors << " expected errors, ";
				}
				std::cout << nWarnings << " warnings)";
			}
		}
		std::cout << ", " << execTimeMS.c_str() << " ms<br>" << std::endl;

		if(nErrors != 0)
		{
			for(unsigned int i = 0; i < errors.size(); ++i)
			{
				if(negativeTest)
				{
					std::cout << "<font color=purple>" << errors[i] << "</font><br>" << std::endl;
				}
				else
				{
					std::cout << "<font color=red>" << errors[i] << "</font><br>" << std::endl;
				}
			}
		}

		if(lf != NULL)
		{
			fclose(lf);
		}
	}
}

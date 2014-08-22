#include "mgclasstester.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdio>

void MGClassTester::logEval(const char *logFileName, bool negativeTest)
{
	FILE *lf = NULL;
	int logError = fopen_s(&lf, logFileName, "rt");

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
					std::cout << "<font color=purple>" << errors[i].c_str() << "</font><br>" << std::endl;
				}
				else
				{
					std::cout << "<font color=red>" << errors[i].c_str() << "</font><br>" << std::endl;
				}
			}
		}

		if(lf != NULL)
		{
			fclose(lf);
		}
	}
}


void MGClassTester::logFilter(const char *logFileName)
{
	FILE *logf = NULL;
	FILE *filteredlf = NULL;
	std::string filteredFN = std::string(logFileName) + std::string(".filtered");
	
	int logError = fopen_s(&logf, logFileName, "rt");
	if(logf == NULL)
	{
		std::cout << "ERROR: logFilter failed to open log file " << logFileName << ", error(" << logError << ")" << std::endl;
		return;
	}
	logError = fopen_s(&filteredlf, filteredFN.c_str(), "w");
	if(filteredlf == NULL)
	{
		std::cout << "ERROR: logFilter failed to open log file " << filteredFN.c_str() << ", error(" << logError << ")" << std::endl;
		return;
	}

	std::cout << "Filtering " << logFileName << " ... ";
	char logLine[MGCLASSTESTER_LOGLINE_MAXLENGTH] = "";
	char *neof = NULL;

	while(true)
	{
		// Read until new line or end of file, whichever happens first..
		neof = fgets(logLine, MGCLASSTESTER_LOGLINE_MAXLENGTH, logf);
		if(neof == NULL)
		{
			break;
		}
		else
		{
			std::string line(logLine);
			std::string infoSubstr(" INFO: ");

			std::size_t foundInfo = line.find(infoSubstr);

			if (foundInfo != std::string::npos)
			{
				// Ignore all info prints - result should not depend on logging settings
			}
			else
			{
				fputs(filterLine(logLine).c_str(), filteredlf);
			}
		}
	}


	if(logf != NULL)
	{
		fclose(logf);
	}

	if(filteredlf != NULL)
	{
		fclose(filteredlf);
	}

	std::cout << "<b>DONE</b><br>" << std::endl;
}


void MGClassTester::logCompare(const char *logFileName1, const char *logFileName2)
{
	std::string filteredFN = std::string(logFileName1) + std::string(".filtered");
	std::cout << "Expected " << filteredFN.c_str() << " ... <b>";

	FILE *logf1 = NULL;
	FILE *logf2 = NULL;
	
	int logError = fopen_s(&logf1, filteredFN.c_str(), "rt");
	if(logf1 == NULL)
	{
		std::cout << "ERROR: logCompare failed to open log file " << filteredFN.c_str() << ", error(" << logError << ")" << std::endl;
		return;
	}
	logError = fopen_s(&logf2, logFileName2, "rt");
	if(logf2 == NULL)
	{
		std::cout << "ERROR: logCompare failed to open log file " << logFileName2 << ", error(" << logError << ")" << std::endl;
		return;
	}

	// ...

	char logLine1[MGCLASSTESTER_LOGLINE_MAXLENGTH] = "";
	char logLine2[MGCLASSTESTER_LOGLINE_MAXLENGTH] = "";
	char *neof1 = NULL;
	char *neof2 = NULL;
	bool resultOk = true;

	while(true)
	{
		// Read until new line or end of file, whichever happens first..
		neof1 = fgets(logLine1, MGCLASSTESTER_LOGLINE_MAXLENGTH, logf1);
		neof2 = fgets(logLine2, MGCLASSTESTER_LOGLINE_MAXLENGTH, logf2);
		if(neof1 == NULL && neof2 == NULL)
		{
			break;
		}
		else if(neof1 == NULL || neof2 == NULL)
		{
			resultOk = false;
			break;
		}
		else
		{
			if(std::string(logLine1) != std::string(logLine2))
			{
				resultOk = false;
				std::cout << "<font color=red>DIFF: " << logLine1 << " - " << logLine2 << "</font>" << std::endl;
				break;
			}
		}
	}

	if(logf1 != NULL)
	{
		fclose(logf1);
	}

	if(logf2 != NULL)
	{
		fclose(logf2);
	}
	if(resultOk)
	{
		std::cout << "<font color=green>PASS</font></b><br>" << std::endl;
	}
	else
	{
		std::cout << "<font color=red>FAIL</font></b><br>" << std::endl;
	}
}


std::string MGClassTester::filterLine(const char* line)
{
	std::string lineRes("");
	bool insideFilteredInt = false;
	char c[2] = {0, 0};

	for(unsigned int i=0; i < strlen(line); ++i)
	{
		if(line[i] >= '0' && line[i] <= '9')
		{
			if(i>0 && !insideFilteredInt && (line[i-1]=='[' || line[i-1]==':' || line[i-1]=='.'))
			{
				c[0]='X';
				lineRes += std::string(c);
				insideFilteredInt=true;
			}
			if(i>1 && !insideFilteredInt && line[i-1]==' ' && line[i-2]==':')
			{
				c[0]='X';
				lineRes += std::string(c);
				insideFilteredInt=true;
			}
			else if(insideFilteredInt)
			{
				// do nothing..
			}
			else
			{
				c[0]=line[i];
				lineRes += std::string(c);
				insideFilteredInt=false;
			}
		}
		else if(line[i] == '.' && insideFilteredInt)
		{
			// do nothing..
		}
		else
		{
			c[0]=line[i];
			lineRes += std::string(c);
			insideFilteredInt=false;
		}
	}
	return lineRes;
}

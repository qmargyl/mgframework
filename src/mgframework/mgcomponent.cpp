#include "mgcomponent.h"
#include <string>
#include <vector>

int MGComponent::m_IDGenerator = 0;

/*
eMGComponentConsoleCommand MGComponent::detectMGComponentConsoleCommand(const std::vector<std::string> &cmdvec)
{

	if(cmdvec.size() == 0)
	{
		return MGComponent_UNDEFINED;
	}
	else if(cmdvec.size() == 1)
	{
		if(cmdvec[0]=="help")
		{
			return MGComponent_HELP;
		}
		else if(cmdvec[0]=="exit")
		{
			return MGComponent_EXIT;
		}
		else
		{
			return MGComponent_UNDEFINED;
		}
	}




	// At this point, all commands not on the format <class> <int> <command> should already have been detected..
	if(cmdvec.size() < 3)
	{
		return MGComponent_UNDEFINED;
	}
	else if(cmdvec[0]=="pe" && cmdvec.size() == 3 && cmdvec[2]=="help")
	{
		return MGComponent_PE_INT_HELP;
	}
	else if(cmdvec[0]=="pe" && cmdvec.size() == 3 && cmdvec[2]=="activate")
	{
		return MGComponent_PE_INT_ACTIVATE;
	}
	else if(cmdvec[0]=="pe" && cmdvec.size() == 3 && cmdvec[2]=="deactivate")
	{
		return MGComponent_PE_INT_DEACTIVATE;
	}
	else if(cmdvec[0]=="pe" && cmdvec.size() == 4 && cmdvec[2]=="setuptimer")
	{
		return MGComponent_PE_INT_SETUPTIMER_INT;
	}
	else if(cmdvec[0]=="pe" && cmdvec.size() == 4 && cmdvec[2]=="logging" && cmdvec[3]=="on")
	{
		return MGComponent_PE_INT_LOGGING_ON;
	}
	else if(cmdvec[0]=="pe" && cmdvec.size() == 4 && cmdvec[2]=="logging" && cmdvec[3]=="off")
	{
		return MGComponent_PE_INT_LOGGING_OFF;
	}
	else if(cmdvec[0]=="pe" && cmdvec[1]=="all")
	{
		return MGComponent_PE_ALL_X;
	}

	return MGComponent_UNDEFINED;

}

*/
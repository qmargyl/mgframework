#ifndef _MGFRAMEWORKSTUB_H
#define _MGFRAMEWORKSTUB_H

#include "../mgpathitem.h"
#include "../mgsymboltable.h"
#include "../mgframework.h"

class MGFrameworkStub : public MGFramework
{
	private:
		unsigned int m_handleGameLogicsCounter;
		unsigned int m_drawCounter;
	
		// MGFramework
		void handleGameLogics(){ m_handleGameLogicsCounter++; }
		void draw(){ m_drawCounter++; }

	public:
		MGFrameworkStub()
		:	m_handleGameLogicsCounter(0),
			m_drawCounter(0)
		{}
		bool init(int w, int h, int tw, int th);

		// Hooks to MO related methods in MGFramework
		std::list<MGMovingObject> _m_MO(){ return m_MO; }
		void _deleteAllMO(){ deleteAllMO(); }
		void _addMO(int n){ addMO(n); }
		int _getNumberOfMO(){ return getNumberOfMO(); }
		void _deleteMO(std::list<MGMovingObject>::iterator it){ deleteMO(it); }
		bool _setupMO(std::list<MGMovingObject>::iterator it, int x, int y, unsigned int owner, int speed, int x1, int y1, int x2, int y2)
		{
			return setupMO(it, x, y, owner, speed, x1, y1, x2, y2);
		}
		unsigned int _getNumberOfMarkedMO(){ return getNumberOfMarkedMO(); }
		void _countMark(){ countMark(); }
		void _countUnMark(){ countUnMark(); }

		// Hooks to SO related methods in MGFramework
		std::list<MGStationaryObject> _m_SO(){ return m_SO; }
		void _deleteAllSO(){ deleteAllSO(); }
		void _addSO(int n){ addSO(n); }
		int _getNumberOfSO(){ return getNumberOfSO(); }
		void _deleteSO(std::list<MGStationaryObject>::iterator it){ deleteSO(it); }
		bool _setupSO(std::list<MGStationaryObject>::iterator it, int x, int y)
		{
			return setupSO(it, x, y);
		}

		// Hooks to PE related methods in MGFramework
		std::list<MGPeriodicEvent> _m_PE(){ return m_PE; }
		void _deleteAllPE(){ deleteAllPE(); }
		void _addPE(int n){ addPE(n); }
		int _getNumberOfPE(){ return getNumberOfPE(); }
		void _deletePE(std::list<MGPeriodicEvent>::iterator it){ deletePE(it); }

		// Hooks to other methods in MGFramework
		int _getFrameStartX(){ return getFrameStartX(); }
		int _getFrameStartY(){ return getFrameStartY(); }
		int _getFrameEndX(){ return getFrameEndX(); }
		int _getFrameEndY(){ return getFrameEndY(); }
		void _activateFraming(int x, int y){ activateFraming(x, y); }
		void _deactivateFraming(){ deactivateFraming(); }
		void _updateFraming(int x, int y){ updateFraming(x, y); }

		// Helper functions
		std::list<MGMovingObject>::iterator nthMO(unsigned int i)
		{
			unsigned int n = 0;
			for(std::list<MGMovingObject>::iterator it = m_MO.begin(); it != m_MO.end(); it++)
			{
				if(n == i)
				{
					return it;
				}
				n++;
			}
			return m_MO.end();
		}

		std::list<MGStationaryObject>::iterator nthSO(unsigned int i)
		{
			unsigned int n = 0;
			for(std::list<MGStationaryObject>::iterator it = m_SO.begin(); it != m_SO.end(); it++)
			{
				if(n == i)
				{
					return it;
				}
				n++;
			}
			return m_SO.end();
		}

		std::list<MGPeriodicEvent>::iterator nthPE(unsigned int i)
		{
			unsigned int n = 0;
			for(std::list<MGPeriodicEvent>::iterator it = m_PE.begin(); it != m_PE.end(); it++)
			{
				if(n == i)
				{
					return it;
				}
				n++;
			}
			return m_PE.end();
		}

		// Call counter getters
		unsigned int getHandleGameLogicsCounter(){ return m_handleGameLogicsCounter; }
		unsigned int getDrawCounter(){ return m_drawCounter; }


};

#endif

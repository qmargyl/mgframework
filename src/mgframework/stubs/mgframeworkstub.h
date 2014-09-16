#ifndef _MGFRAMEWORKSTUB_H
#define _MGFRAMEWORKSTUB_H

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

		// Call counter getters
		unsigned int getHandleGameLogicsCounter(){ return m_handleGameLogicsCounter; }
		unsigned int getDrawCounter(){ return m_drawCounter; }


};

#endif

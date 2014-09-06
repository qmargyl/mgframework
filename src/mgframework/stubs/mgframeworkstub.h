#ifndef _MGFRAMEWORKSTUB_H
#define _MGFRAMEWORKSTUB_H

#include "../mgframework.h"

class MGFrameworkStub : public MGFramework
{
	private:
		void handleGameLogics(){}
		void draw(){}

	public:
		MGFrameworkStub();
		bool init(int w, int h, int tw, int th);

		// Hooks to MO related methods in MGFramework
		void _deleteAllMO(){ deleteAllMO(); }
		int _addMO(int n){ return addMO(n); }
		int _getNumberOfMO(){ return getNumberOfMO(); }
		void _deleteMO(int index){ deleteMO(index); }
		bool _setupMO(int i, int x, int y, unsigned int owner, int speed, int x1, int y1, int x2, int y2)
		{
			return setupMO(i, x, y, owner, speed, x1, y1, x2, y2);
		}

};

#endif

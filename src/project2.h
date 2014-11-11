#ifndef _PROJECT_2_H
#define _PROJECT_2_H

#include "mgframework/mgframework.h"

class Project2 : public MGFramework
{
	private:
		virtual void handleGameLogics(); // MGFramework
		virtual void draw(); // MGFramework

		// Application specific graphics
		enum
		{
			TEX_GRASS = 0,
			TEX_PINKBLOB,
			TEX_TREE,
			TEX_TREE2,
			TEX_REDFRAME,
			NO_OF_TEXTURES
		} TextureIndex;

		std::vector<MGTexHandle> textures;

	public:
		Project2();
		virtual bool init(int w, int h, int tw, int th); // MGFramework
};

#endif

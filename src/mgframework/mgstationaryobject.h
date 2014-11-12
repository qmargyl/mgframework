#ifndef _MG_STATIONARYOBJECT_H
#define _MG_STATIONARYOBJECT_H
#include "mgcomponent.h"

class MGFramework;
struct MGTexHandle;

class MGStationaryObject :public MGComponent
{
private:
	int m_TileX;
	int m_TileY;
	int m_XOff;
	int m_YOff;
	int m_Type;
	MGTexHandle* m_texHandle;

public:

	MGStationaryObject();
	~MGStationaryObject();
	
	friend bool operator<(MGStationaryObject &lhs, MGStationaryObject &rhs);

	void setTileXY(int x, int y, MGFramework *world = NULL, bool occupyMap = true);
	void setOffsetXY(int xoff, int yoff){ m_XOff = xoff; m_YOff = yoff; }
	inline const int getTileX() const { return m_TileX; }
	inline const int getTileY() const { return m_TileY; }
	inline const int getXOffset() const { return m_XOff; }
	inline const int getYOffset() const { return m_YOff; }
	void setType(int t){ m_Type = t; }
	int getType() const { return m_Type; }
	void setTexHandle(MGTexHandle* tH){ m_texHandle = tH; }
	void setTexHandle(std::vector<MGTexHandle*> tHVec);
	const MGTexHandle* getTexHandle() const { return m_texHandle; }
	bool runConsoleCommand(const char *c, MGFramework *w, MGSymbolTable *s);
	eMGComponentConsoleCommand detectMGComponentConsoleCommand(const std::vector<std::string> &cmdvec) const;
};



#endif
#ifndef _MG_MOVINGOBJECT_H
#define _MG_MOVINGOBJECT_H
#include "mgcomponent.h"
#include <list>

class MGFramework;
class PathItem;

class MGMovingObject :public MGComponent
{
private:
	double m_Speed; // pixels per second
	int m_TimeOfLastUpdate;
	int m_TileX;
	int m_TileY;
	int m_DestTileX;
	int m_DestTileY;
	double m_X;
	double m_Y;
	static int m_TileSize;
	bool m_FinishingLastMove;
	int m_TempDestTileX;
	int m_TempDestTileY;

	int m_NextTileX;
	int m_NextTileY;
	

	double getDistance(int wx, int wy);
	int getTileSize(){ return m_TileSize;} //TODO: Make this static
	double getSpeed(){ return m_Speed;}

	//Marking related
	bool m_Marked;

	//State
	enum MOState{
		MOStateCreated = 0,
		MOStateIdle,
		MOStateMoving,
		MOStateStuck
	};

	MOState m_CurrentState;
	MOState getCurrentState(){ return m_CurrentState; }
	void changeState(MOState toState);
	const char* toString(MOState s);
	static int m_MovingMOCounter;

	std::list<PathItem> m_Path;
	std::vector<std::string> m_History;
	bool m_HistoryEnabled;

public:

	MGMovingObject();
	~MGMovingObject();

	void initialize();

	void setTileXY(int x, int y, MGFramework *world);
	void setNextXY(int x, int y, MGFramework *world);

	void setDestTileXY(int x, int y);
	void setPath(std::list<PathItem> p);

	void setSpeed(double s, int tileSize); // Seconds, Distance to next tile (pixels)
	void update(MGFramework *w);
	void copy(MGMovingObject *src);

	inline const int getTileX(){ return m_TileX; }
	inline const int getTileY(){ return m_TileY; }
	inline const int getDestTileX(){	if(m_FinishingLastMove) return m_TempDestTileX;	return m_DestTileX;	}
	inline const int getDestTileY(){	if(m_FinishingLastMove)	return m_TempDestTileY;	return m_DestTileY;	}

	inline const int getXOffset(){ return (int)(m_X + 0.5); } // Difference between actual coordinate and tile coordinate
	inline const int getYOffset(){ return (int)(m_Y + 0.5); }
	inline const int getCenterX(){ return getTileX() * getTileSize() + (int)(0.5 * getTileSize()); }
	inline const int getCenterY(){ return getTileY() * getTileSize() + (int)(0.5 * getTileSize()); }
	bool runConsoleCommand(const char *c, MGFramework *w, MGSymbolTable *s);
	eMGComponentConsoleCommand detectMGComponentConsoleCommand(const std::vector<std::string> &cmdvec);

	//Marking related
	inline void mark(){ m_Marked = true; }
	inline void unMark(){ m_Marked = false; }
	inline bool isMarked(){ return m_Marked; }

	//Update frequency related
	inline void setTimeOfLastUpdate(Uint32 ms){ m_TimeOfLastUpdate = ms; }
	inline const Uint32 getTimeOfLastUpdate(){ return m_TimeOfLastUpdate; }

	//State related
	inline const bool isCreated(){ return getCurrentState() == MOStateCreated; }
	inline const bool isIdle(){ return getCurrentState() == MOStateIdle; }
	inline const bool isMoving(){ return getCurrentState() == MOStateMoving; }
	inline const bool isStuck(){ return getCurrentState() == MOStateStuck; }
	inline const static bool anyMovingMO(){ return (m_MovingMOCounter > 0); }
	inline const static int nMovingMO(){ return m_MovingMOCounter; }

	// History related
	void enableHistory(){ if(!m_HistoryEnabled) m_HistoryEnabled = true; }
	void disableHistory(){ if(m_History) m_History.clear(); m_HistoryEnabled = false; }
	void printHistory();
	void addToHistory(const char *str); //TODO: Make this private?

};

#endif
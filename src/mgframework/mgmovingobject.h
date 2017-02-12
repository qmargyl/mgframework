#ifndef _MG_MOVINGOBJECT_H
#define _MG_MOVINGOBJECT_H
#include "mgcomponent.h"
#include <list>

class MGFramework;
class MGPathItem;
enum eMGFPathType;

class MGMovingObject :public MGComponent
{
private:
	double m_VPixelsPerSecond;
	int m_TimeOfLastUpdate;
	int m_TileX;
	int m_TileY;
	int m_LastX;
	int m_LastY;
	int m_NextTileX;
	int m_NextTileY;
	double m_X;
	double m_Y;
	bool m_FinishingMove;
	int m_TempNextTileX;
	int m_TempNextTileY;
	static int m_TileSize;
	eMGFPathType m_PathFindingAlgorithm;
	

	double getDistance(int wx, int wy);
	static int getTileSize(){ return m_TileSize; }
	inline double getSpeed() const { return m_VPixelsPerSecond; }

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
	MOState getCurrentState() const { return m_CurrentState; }
	void changeState(MOState toState);
	const char* toString(MOState s) const;
	static int m_MovingMOCounter;

	std::list<MGPathItem> m_Path;
	std::vector<std::string> m_History;
	bool m_HistoryEnabled;

	void addToHistory(const char *str);

public:

	MGMovingObject();
	~MGMovingObject();

	void initialize();
	std::string toString() const { return std::string(toString(getCurrentState())); }

	void setTileXY(int x, int y, MGFramework *world);
	void setNextTileXY(int x, int y, MGFramework *world);

	void setPath(std::list<MGPathItem> p);

	void setSpeed(double s, int tileSize); // Seconds, Tile size in pixels
	void update(MGFramework *w);

	inline const int getTileX() const { return m_TileX; }
	inline const int getTileY() const { return m_TileY; }
	inline const int getNextTileX() const { if(m_FinishingMove) return m_TempNextTileX; return m_NextTileX;	}
	inline const int getNextTileY() const { if(m_FinishingMove) return m_TempNextTileY; return m_NextTileY;	}
	inline const int getXOffset() const { return (int)(m_X + 0.5); } // Difference between actual coordinate and tile coordinate
	inline const int getYOffset() const { return (int)(m_Y + 0.5); }
	inline const int getCenterX() { return getTileX() * getTileSize() + (int)(0.5 * getTileSize()); }
	inline const int getCenterY() { return getTileY() * getTileSize() + (int)(0.5 * getTileSize()); }

	// MGComponent
	bool runConsoleCommand(const char *c, MGFramework *w, MGSymbolTable *s);
	eMGComponentConsoleCommand detectMGComponentConsoleCommand(const std::vector<std::string> &cmdvec) const;

	//Marking related
	inline void mark(){ m_Marked = true; }
	inline void unMark(){ m_Marked = false; }
	inline bool isMarked() const { return m_Marked; }

	//Update frequency related
	inline void setTimeOfLastUpdate(unsigned int ms){ m_TimeOfLastUpdate = ms; }
	inline const unsigned int getTimeOfLastUpdate(){ return m_TimeOfLastUpdate; }

	//State related
	inline const bool isCreated() const { return getCurrentState() == MOStateCreated; }
	inline const bool isIdle() const { return getCurrentState() == MOStateIdle; }
	inline const bool isMoving() const { return getCurrentState() == MOStateMoving; }
	inline const bool isStuck() const { return getCurrentState() == MOStateStuck; }
	inline const static bool anyMovingMO() { return (m_MovingMOCounter > 0); }
	inline const static int nMovingMO() { return m_MovingMOCounter; }

	// History related
	void enableHistory(){ if(!m_HistoryEnabled) m_HistoryEnabled = true; }
	void disableHistory(){ if(m_HistoryEnabled) m_History.clear(); m_HistoryEnabled = false; }
	void printHistory();
};

#endif
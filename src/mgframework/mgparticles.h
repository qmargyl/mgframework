#ifndef _MG_PARTICLES_H
#define _MG_PARTICLES_H


class MG2DParticle
{
private:
	double x; // Position X
	double y; // Position Y
	double vx; // Velocity X
	double vy; // Velocity Y
	Uint32 timeOfBirth; // Ticks at creation.
	Uint32 lifeSpan; //  Milliseconds between timeOfBirth and time of death.
	bool aging; // Set to false to give the particle eternal life.
	bood alive; // True if the particle is alive.

public:
	MG2DParticle();
	~MG2DParticle();

	bool isAlive(); // returns alive flag.
	bool isAging(); // returns aging flag.
	void kill(); // Sets alive to false.
	void reSpawn(); // resets timeOfBirth.
	void updatePosition(int frameRate, double gravityX, double gravityY, double forceX, double forceY); // Updates the position and velocity given frame rate and applied forces. Also updates status flags. This is typically done once every frame.
	void rotate(double angle); // Rotate position and velocity around {0,0}.
	void rotate(double angle, double rx, double ry); // Rotate position and velocity around {rx,ry}.
};


class MG2DParticles
{
private:
	

public:
	MG2DParticles();
	~MG2DParticles();

};


#endif








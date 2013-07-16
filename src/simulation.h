#ifndef __SIMULATION_H__
#define __SIMULATION_H__

#include <vector>
#include <ostream>

#include "element.h"
#include "particle.h"

class Simulation;

//=====================================================================
/** Describes a point in computation (time, energy, particle states) */
class Frame
{
public:
	const double time;

	double kineticEnergy;
	double potentialEnergy;

	const Simulation* const simulation;

	/** Construct a frame from the current state of the simulation.
	*   Note: The frame is not copying the particle states, so it must be
	*         written out before simulation proceeds 
	*/
	Frame(double time, const Simulation* simulation) :
		time(time), simulation(simulation) {};
};

/** Output the frame to a stream */
std::ostream& operator<<(std::ostream& os, const Frame& frame);


//=====================================================================
/** Contains all information needed to run a simulation */
class Simulation
{
	double time;          //< Current simulation time

public:
	const double dt;      //< Time step
	const double boxSize; //< Size of the simulation box
	const double boxK;    //< Force constant for box spring

	// See http://en.wikipedia.org/wiki/Verlet_integration#Applications
	double f1;            //< Verlet friction constant
	double f2;            //< Verlet friction constant

	std::vector<Particle> particles; //< Particles in simulation
	std::vector<Element>  elements;  //< All used elements in simulation

	/** Construct a simulation */
	Simulation(double dt, double boxSize, double boxK) :
		dt(dt), boxSize(boxSize), boxK(boxK), f1(1.0), f2(2.0), time(0.0)
	{ }

	/** Add a particle to the simulation */
	Particle& addParticle(const char* elementName, double x, double vx, double y, double vy);

	/** Make a computation step and return the resulting frame. */
	Frame step(int nMicroSteps);

private:
	// Box forces and potential
	void   forceBox    (Particle* const particle);
	double potentialBox(Particle* const particle);

	// Pairwise (LJ, bond, electrostatic) forces and potential
	void   forcePairwise    (Particle* const pi, Particle* const pj);
	double potentailPairwise(Particle* const pi, Particle* const pj);

	// Implementation of the step function
	void   computeForces();
	void   microstepFirst();
	void   microstep();
	double kineticEnergyStep();
};

#endif /*__SIMULATION_H__*/

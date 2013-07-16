#ifndef __SIMULATION_H__
#define __SIMULATION_H__

#include <vector>
#include <istream>

#include "element.h"
#include "particle.h"
#include "frame.h"

//=====================================================================
/** Contains all information needed to run a simulation */
class Simulation
{
	double time;          //< Current simulation time

public:
	double dt;      //< Time step
	double boxSize; //< Size of the simulation box
	double boxK;    //< Force constant for box spring

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
	Particle& addParticle(const std::string& elementName, 
		double x, double vx, double y, double vy);

	/** Make a computation step and return the resulting frame. */
	Frame step(int nMicroSteps);

	/** Read a simulation setup from the given stream */
	// Note: implemented in frame.cpp to keep reading code close to writing code.
	//       Couldn't make 'Simulation Frame::read()' because of circular deps
	//       between Frame and Simulation.
	std::istream& operator<< (std::istream& is);

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

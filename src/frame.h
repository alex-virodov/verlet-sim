#ifndef __FRAME_H__
#define __FRAME_H__

#include <vector>
#include <ostream>
#include <istream>

#include "element.h"
#include "particle.h"

class Simulation;

//=====================================================================
/** An internal helper class to read and parse tokens */
class Token
{
public:
	std::string name;
	std::string value;

	/** Construct a single name:value token */
	Token(const std::string& name, const std::string& value) :
		name(name), value(value) {};

	/** Read a token with expected name, or throw an exception */
	static std::string readExpected(std::istream& is, const std::string& expected);

	/** Read a token with expected name and convert to double, 
	 *  or throw an exception */
	static double readExpectedDouble(std::istream& is, const std::string& expected);

	/** Read the next name:value token from the stream */
	static Token read(std::istream& is);
};

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



#endif /*__FRAME_H__*/

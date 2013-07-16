#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "element.h"
#include "bond.h"

//=====================================================================
/** Contains the description and dynamic variables of a single 
 *  particle (atom) */
struct Particle
{
	/** Iterator types for vector iteration */
	typedef std::vector<Particle>::iterator       Itr;
	typedef std::vector<Particle>::const_iterator CItr;

	const Element* element; ///< The element description of this particle

	double x;  ///< Position
	double y;  
	double px; ///< Previous position (and initial velocity)
	double py;

	double fx; ///< Force acting on this particle
	double fy;

	/** Bond information */
	int         nbonds; 
	const Bond* bonds;

	/** Construct a particle of a given element */
	Particle(const Element* element, double x, double vx, double y, double vy) : 
		element(element), x(x), y(y), px(vx), py(vy) {};
};



#endif /*__PARTICLE_H__*/

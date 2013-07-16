#ifndef __ELEMENT_H__
#define __ELEMENT_H__

#include <vector>
#include <string>

/** To compute Lennard-Jones sigma from radius, we need a constant 2^(1/6) */
#define CONST_2_TO_1_OVER_SIX /*2^(1/6)=*/1.1224620483093729814335330496792

//=====================================================================
/** Describes an element from a periodic table (and possibly its 
 *  charged or other variations) */
class Element
{
public:
	/** Iterator types for vector iteration */
	typedef std::vector<Element>::iterator       Itr;
	typedef std::vector<Element>::const_iterator CItr;

	std::string name;     ///< Element name like H, Li, or O+2
	double      mass;     ///< Atomic mass
	double      charge;   ///< Coloumb charge
	double      radius;   ///< Element radius for LJ and drawing

	double      ljSigma;  ///< Lennard-Jones potential sigma value
	double      ljEps;    ///< Lennard-Jones potential epsilon value

	/** Construct an element entry */
	Element(const std::string& name, 
		double mass, double charge, double radius, double ljEps) :
			name(name), mass(mass), charge(charge), radius(radius),
			ljEps(ljEps), ljSigma(2*radius/CONST_2_TO_1_OVER_SIX)
	{}
};

#endif /*__ELEMENT_H__*/

#ifndef __BOND_H__
#define __BOND_H__

#include <vector>

//=====================================================================
/** Describes a linear spring-like bond constraint between two atoms. */
struct LinearBond
{
	/** Iterator types for vector iteration */
	typedef std::vector<LinearBond>::iterator       Itr;
	typedef std::vector<LinearBond>::const_iterator CItr;

	typedef std::vector<std::vector<LinearBond> >::iterator       VItr;
	typedef std::vector<std::vector<LinearBond> >::const_iterator VCItr;

	int    otherParticle;
	double dist;
	double k;
};

#if 0
struct AngleBond 
{
	/** Iterator types for vector iteration */
	typedef std::vector<AngleBond>::iterator       Itr;
	typedef std::vector<AngleBond>::const_iterator CItr;

	int    otherParticle;
	int    cornerParticle;
	double angle;
	double k;
};

struct CompactedLinearBond
{
	int count;
	LinearBond bonds[1];

	static int size(int nbonds)
	{
		if (nbonds == 0) {
			return sizeof(CompactedLinearBond().count);
		} else {
			return sizeof(CompactedLinearBond) 
					+ sizeof(CompactedLinearBond().bonds[0])*(nbonds-1);
		}
	}

	void advance(CompactedLinearBond*& ptr)
	{
		ptr = reinterpret_cast<CompactedLinearBond*>(
				reinterpret_cast<char*>(ptr) + size(count));
	}
};

struct CompactedAngleBond
{
	int count;
	AngleBond  bonds[1];
};
#endif

#endif /*__BOND_H__*/

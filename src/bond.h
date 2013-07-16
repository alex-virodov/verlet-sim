#ifndef __BOND_H__
#define __BOND_H__

//=====================================================================
/** Describes a bond constraint between two atoms, including angle 
 *  constraint. */
struct Bond
{
	enum Type {
		LINEAR, ANGLE
	};

	Type type;
	int  otherAtom;
	
	union {
		struct {
			double dist;
			double k;
		} linear;

		struct {
			int    cornerAtom;
			double angle;
			double k;
		} angle;
	};
};

#endif /*__BOND_H__*/

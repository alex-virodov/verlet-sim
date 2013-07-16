#ifndef __BOND_H__
#define __BOND_H__

//=====================================================================
/** Describes a bond constraint between two atoms, including angle 
 *  constraint. */
struct Bond
{
	typedef enum Type {
		LINEAR, ANGLE
	};

	Type type;
	int  otherAtom;
	
	union {
		struct Linear {
			double dist;
			double k;
		};

		struct Angle {
			int    cornerAtom;
			double angle;
			double k;
		};
	};
};

#endif /*__BOND_H__*/

#include <string>
#include <math.h>
#include <algorithm>

#include "simulation.h"

//=====================================================================
/** Add a particle to the simulation */
Particle& Simulation::addParticle(const std::string& elementName, 
	double x, double vx, double y, double vy)
{
	for (Element::Itr element = elements.begin(); element != elements.end(); element++) {
		if (element->name == elementName) {
			// Found the element, add particle
			particles.push_back(Particle(&*element, x, vx, y, vy));
			linearBonds.push_back(std::vector<LinearBond>());

			return particles[particles.size()-1];
		}
	}

	throw std::string("Cannot find element [").append(elementName).append("]");
}

//=====================================================================
/** Add a bond between particles */
LinearBond& Simulation::addLinearBond(int a, int b, double dist, double k)
{
	LinearBond bond;
	bond.otherParticle = b;
	bond.k             = k;
	bond.dist          = dist;

	linearBonds[a].push_back(bond);
	return linearBonds[a][linearBonds[a].size()-1];
}

//=====================================================================
/** Compute the forces on a particle due to the box */
void   Simulation::forceBox    (Particle* const p)
{
	if (p->x > boxSize) { p->fx +=-boxK * ( p->x - boxSize); }
	if (p->y > boxSize) { p->fy +=-boxK * ( p->y - boxSize); }
	if (p->x <-boxSize) { p->fx += boxK * (-p->x - boxSize); }
	if (p->y <-boxSize) { p->fy += boxK * (-p->y - boxSize); }
}

//=====================================================================
/** Compute the potential energy on a particle due to the box */
double Simulation::potentialBox(Particle* const p)
{
	return 0;
}

//=====================================================================
/** Compact the linear bonds into an array */
void Simulation::makeCompactedBonds()
{
	int linearSize = 0;

	int lastThisParticle = -1;

/*	for (std::vector<std::vector<Bond> >::iterator from = bonds.begin(); from != bonds.end(); from++)
	{
		int numLinear = 0;
		int numAngle  = 0;

		for (std::vector<Bond>::iterator itr = from->begin(); itr != from->end(); itr++) {
			if      (itr->type == Bond::LINEAR) { numLinear++; }
			else if (itr->type == Bond::ANGLE)  { numAngle++;  }
		}
	}*/
}


void Simulation::forceLJ(Particle& pi, Particle& pj, double dx, double dy)
{
	// TODO: if (invr < ljCutoffInv) continue;

	// TODO: if (atomi->element == atomj->element)....


	double invr = 1.0 / sqrt(dx*dx + dy*dy);
	double sigma =     (pi.element->ljSigma + pj.element->ljSigma)/2;
	double eps   = sqrt(pi.element->ljEps   + pj.element->ljEps);

	double sigma2 = sigma*sigma;
	double sigma6 = sigma2*sigma2*sigma2;
	double sigma12 = sigma6*sigma6;

	double invr2 = invr*invr;
	double invr6 = invr2*invr2*invr2;
	double invr12 = invr6*invr6;

	double invr14 = invr12*invr2;
	double invr8  = invr6*invr2;

	double lj = eps * (-48 * sigma12 * invr14 + 24 * sigma6 * invr8);

	pi.fx -= lj * dx;
	pi.fy -= lj * dy;
	pj.fx += lj * dx;
	pj.fy += lj * dy;
}

void Simulation::forceLinearBond(Particle& pi, Particle& pj, double dx, double dy, const LinearBond& bond)
{
	double r = sqrt(dx*dx + dy*dy);

	double f = bond.k * (1 - bond.dist / r);

	pi.fx -= f * dx;
	pi.fy -= f * dy;
	pj.fx += f * dx;
	pj.fy += f * dy;
}

#if 0
//=====================================================================
/** Compute the pairwise forces between two particles */
void   Simulation::forcePairwise    (int i, int j, Particle* const pi, Particle* const pj)
{
	// Assuming index(pi) < index(pj)

}

//=====================================================================
/** Compute the pairwise potential energy between two particles */
double Simulation::potentailPairwise(Particle* const pi, Particle* const pj)
{
	return 0;
}
#endif

//=====================================================================
/** Do the first step in simulation, assuming v(0) is in 'px' field */
void  Simulation::microstepFirst()
{
	double dt_squared_over2 = dt * dt * 0.5;

	for (unsigned int i = 0; i < particles.size(); ++i) {
		double vx = particles[i].px;
		double vy = particles[i].py;
		
		particles[i].px = particles[i].x;
		particles[i].py = particles[i].y;

		double forceScale = dt_squared_over2 / particles[i].element->mass;

		// Approximate x(dt) ~= x(0) + v(0)*dt + [A(x(0))*dt^2]/2
		particles[i].x = particles[i].x + vx * dt + forceScale * particles[i].fx;
		particles[i].y = particles[i].y + vy * dt + forceScale * particles[i].fy;
	}

}

//=====================================================================
/** Compute forces on all particles */
void   Simulation::computeForces()
{
	int nParticles = particles.size();

	for (int i = 0; i < nParticles; i++) 
	{
		particles[i].fx = 0.0;
		particles[i].fy = 0.0;
	}

	for (int i = 0; i < nParticles; i++) 
	{
		forceBox(&particles[i]);

		std::vector<LinearBond>::iterator linearItr   = linearBonds[i].begin();
		int                               linearLeft  = linearBonds[i].size();

		for (int j = i+1; j < nParticles; j++) 
		{
			Particle& pi = particles[i];
			Particle& pj = particles[j];

			double dx = pi.x - pj.x;
			double dy = pi.y - pj.y;

			if (linearLeft > 0 && linearItr->otherParticle == j) {
				forceLinearBond(pi, pj, dx, dy, *linearItr++);
			} else {
				forceLJ(pi, pj, dx, dy);
			}

			// bonditr = bonds[i].begin();
			//forcePairwise(i, j, &particles[i], &particles[j]);
		}
	}
}

//=====================================================================
/** Do a single integration step, including forces */
void   Simulation::microstep()
{
	int nParticles = particles.size();

	computeForces();

	// Verlet integration
	double dt_squared = dt * dt;

	for (int i = 0; i < nParticles; ++i) {
		double ppx = particles[i].px;
		double ppy = particles[i].py;
		
		particles[i].px = particles[i].x;
		particles[i].py = particles[i].y;

		double forceScale = dt_squared / particles[i].element->mass;

		particles[i].x = f2 * particles[i].x - f1 * ppx + forceScale * particles[i].fx;
		particles[i].y = f2 * particles[i].y - f1 * ppy + forceScale * particles[i].fy;
	}
}

//=====================================================================
/** Do a single integration step, and compute kinetic energy at this 
 *  step, where v(particle) ~ x(before) - x(after) */
double Simulation::kineticEnergyStep()
{
	/*for (int i = 0; i < natoms; ++i) {
		kineticPosition[i*2+0] = atoms[i].px;
		kineticPosition[i*2+1] = atoms[i].py;
	}*/

	microstep();

	double energy = 0;
	/*
	// V = (dx/2dt, dy/2dt) `dot` 1/2 m v^2 
	//   = 1/2 * 1/4 * 1/dt^2 * (dx^2 + dy^2)
	double inv_8dt2 = 1.0 / (2*4*dt*dt);

	for (int i = 0; i < nParticles; ++i) 
	{
		double dx = kineticPosition[i*2+0] - particles[i].x;
		double dy = kineticPosition[i*2+1] - particles[i].y;

		energy += inv_8dt2 * particles[i].mass * (dx * dx + dy * dy);
	}*/

	return energy;
}

//=====================================================================
/** Make a computation step and return the resulting frame. */
Frame Simulation::step(int nMicroSteps)
{
	if (time == 0.0) {
		// Do the first microstep to convert initial velocities
		// to previous positions, as needed by verlet
		computeForces();
		microstepFirst();
	}

	time += dt * nMicroSteps;

	Frame frame(time, this);

	for (int i = 0; i < nMicroSteps-1; ++i) {
		microstep();
	}

	frame.kineticEnergy = kineticEnergyStep();

	/*if (thermostat) {
		setFriction(thermostat->getFriction(ke));
	}*/

	return frame;
}

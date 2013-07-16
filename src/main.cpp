#include <iostream>
#include <stdlib.h>

#include "element.h"
#include "bond.h"
#include "particle.h"
#include "simulation.h"

using namespace std;

int main(int argc, char** argv)
{
	Simulation sim(/*dt=*/0.0001, /*boxSize=*/10, /*boxK=*/10.0);

	sim.elements.push_back(Element("X", /*mass=*/5.0, /*charge=*/0.0, /*radius=*/1.0, /*ljEps=*/0.1));

	srand(0);
	int n = 2;
	double scale = 0.1;
	double dn = sim.boxSize / (n*2+2);
	for (int i = -n; i <= n; i++) {
		for (int j = -n; j <= n; j++) {
			double vx = scale*(((double)rand() / RAND_MAX) - 0.5);
			double vy = scale*(((double)rand() / RAND_MAX) - 0.5);
			sim.addParticle("X", i*dn, vx, j*dn, vy);
		}
	}

	cout << sim.step(1);
	/*for (;;) {
		if (!cout.good()) { break; }
		cout << sim.step(10*(int)(1/sim.dt/30));
	}*/

	return 0;
}


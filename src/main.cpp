#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <fstream>

#include "element.h"
#include "bond.h"
#include "particle.h"
#include "simulation.h"

using namespace std;




int main(int argc, char** argv)
{
	double dt_override = -1;
	char*  filename = 0;

	for (int i = 1; i < argc; i++) {
		if (argv[i][0] != '-' && filename == 0) {
			filename = argv[i];
		}

		if (string(argv[i]) == "--dt" && i+1 < argc) {
			dt_override = atof(argv[i+1]); i++;
		}
	}

	Simulation sim(0, 0, 0);

	try {
		if (filename == 0) {
			sim << cin;
		} else {
			std::ifstream file(filename);
			sim << file;
		}
	} catch (const std::string& error) {
		cerr << "Exception: " << error << endl;
		return 1;
	}

	if (dt_override > 0) {
		sim.dt = dt_override;
	}

	cout << sim.step(1);

	for (;;) {
		if (!cout.good()) { break; }
		cout << sim.step(10*(int)(1/sim.dt/60));
	}
}



// TODO: Remove!
#if 0
/*


	std::string token;
	cout << "frame value:[" << Token::readExpected(ss, "frame") << "]" << endl;

	try {
		cout << "frame value:[" << Token::readExpected(ss, "frame") << "]" << endl;
	} catch (const std::string& s) {
		cout << "Exception: " << s << endl;
	}

	while (ss.good()) {
		Token token = Token::read(ss);
		cout << "Token:[" << token.name << "]:[" << token.value << "]" << endl;
	}

	*/
	return 0;
}

int zzmain(int argc, char** argv)
{
	Simulation sim(/*dt=*/0.0000001, /*boxSize=*/10, /*boxK=*/10.0);

	sim.elements.push_back(Element("X", /*mass=*/1.0, /*charge=*/0.0, /*radius=*/0.5, /*ljEps=*/0.1));

	/*srand(0);
	int n = 2;
	double scale = 0.1;
	double dn = sim.boxSize / (n*2+2);
	for (int i = -n; i <= n; i++) {
		for (int j = -n; j <= n; j++) {
			double vx = scale*(((double)rand() / RAND_MAX) - 0.5);
			double vy = scale*(((double)rand() / RAND_MAX) - 0.5);
			sim.addParticle("X", i*dn, vx, j*dn, vy);
		}
	}*/
	sim.addParticle(sim.elements[0].name, -2,  1, 0, 0);
	sim.addParticle(sim.elements[0].name,  2, -1, 0, 0);

	cout << sim.step(1);
	/*for (;;) {
		if (!cout.good()) { break; }
		cout << sim.step(10*(int)(1/sim.dt/60));
	}*/

	return 0;
}

#endif
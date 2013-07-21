#include <string>
#include <stdlib.h>

#include "frame.h"
#include "simulation.h"

//=====================================================================
/** Read a token with expected name, or throw an exception */
std::string Token::readExpected(std::istream& is, const std::string& expected)
{
	Token first = read(is);
	Token token = first;

	// Search ahead to see if some next token is the right one
	// (skip unexpected tokens)
	while (token.name != expected && is.good()) {
		token = read(is);
	}
	
	if (token.name == expected) { return token.value; }

	throw std::string("Expected token [").append(expected).append("]")
		.append(", got:[").append(first.name)
		.append("]:[")    .append(first.value).append("]");
}

/** Read a token with expected name and convert to double, 
	*  or throw an exception */
double Token::readExpectedDouble(std::istream& is, const std::string& expected)
{
	return atof(readExpected(is, expected).c_str());
}

//=====================================================================
/** Read the next name:value token from the stream */
Token Token::read(std::istream& is)
{
	char name [128] = {0};
	char value[128] = {0};
	char* ptr;
	char  ch;

	// Read name
	ptr = name;
	while ((ch = is.get()) && is.good()) 
	{
		if (ch <= ' ') { 
			/* whitespace, skip*/ 
			continue; 
		}
		else if (ch == ':') {
			// Delimiter, change state to value reading
			*ptr = 0;
			break;
		}
		else {
			// Part of name, record
			*ptr = ch; ptr++;
		}
	}

	// Read value
	ptr = value;
	while ((ch = is.get()) && is.good()) 
	{
		if (ch <= ' ') {
			// Whitespace, complete
			*ptr = 0;
			break;
		}
		else {
			// Part of value, record
			*ptr = ch; ptr++;
		}
	}

	return Token(name, value);
}



//=====================================================================
/** Output the frame to a stream */
std::ostream& operator<<(std::ostream& os, const Frame& frame)
{
	os << "frame:" << frame.time << std::endl
	   << "dt:"    << frame.simulation->dt      << std::endl
	   << "box:"   << frame.simulation->boxSize << std::endl
	   << "box_k:" << frame.simulation->boxK    << std::endl;

	os << "elements:" << frame.simulation->elements.size() << std::endl;

	for (Element::CItr elem  = frame.simulation->elements.begin(); 
		               elem != frame.simulation->elements.end(); 
				       elem++) 
	{
		os << "name:"   << elem->name   << std::endl
		   << "mass:"   << elem->mass   << std::endl
		   << "charge:" << elem->charge << std::endl
		   << "radius:" << elem->radius << std::endl
		   << "ljeps:"  << elem->ljEps  << std::endl;
	}
	os << std::endl;



	os << "particles:" << frame.simulation->particles.size() << std::endl;

	for (Particle::CItr p  = frame.simulation->particles.begin(); 
		                p != frame.simulation->particles.end(); 
						p++) 
	{
		os << "element:" << p->element->name << std::endl
		   << "x:"  << p->x  << std::endl
		   << "px:" << p->px << std::endl
		   << "y:"  << p->y  << std::endl
		   << "py:" << p->py << std::endl;
	}
	os << std::endl;

	int nbonds = 0;
	for (unsigned int a = 0; a < frame.simulation->linearBonds.size(); a++) {
		for (LinearBond::CItr b  = frame.simulation->linearBonds[a].begin(); 
			                  b != frame.simulation->linearBonds[a].end(); 
							  b++) {
			nbonds++;
		}
	}

	os << "lbonds:" << nbonds << std::endl;

	for (unsigned int a = 0; a < frame.simulation->linearBonds.size(); a++) {
		for (LinearBond::CItr b  = frame.simulation->linearBonds[a].begin(); 
			                  b != frame.simulation->linearBonds[a].end(); 
							  b++)
		{
			os << "a:"    << a                << std::endl
			   << "b:"    << b->otherParticle << std::endl
			   << "dist:" << b->dist          << std::endl
			   << "k:"    << b->k             << std::endl;
		}
	}
	os << std::endl;

	return os;
}

//=====================================================================
/** Read a simulation setup from the given stream */
std::istream& Simulation::operator<< (std::istream& is)
{
	time    = Token::readExpectedDouble(is, "frame");
	dt      = Token::readExpectedDouble(is, "dt");
	boxSize = Token::readExpectedDouble(is, "box");
	boxK    = Token::readExpectedDouble(is, "box_k");

	elements.clear();
	particles.clear();

	int nelements = atoi(Token::readExpected(is, "elements").c_str());

	for (int i = 0; i < nelements; i++) {
		std::string name   = Token::readExpected      (is, "name");
		double      mass   = Token::readExpectedDouble(is, "mass");
		double      charge = Token::readExpectedDouble(is, "charge");
		double      radius = Token::readExpectedDouble(is, "radius");
		double      ljEps  = Token::readExpectedDouble(is, "ljeps");

		elements.push_back(Element(name, mass, charge, radius, ljEps));
	}

	int nparticles = atoi(Token::readExpected(is, "particles").c_str());

	for (int i = 0; i < nparticles; i++) {
		std::string element = Token::readExpected      (is, "element");
		double      x       = Token::readExpectedDouble(is, "x");
		double      px      = Token::readExpectedDouble(is, "px");
		double      y       = Token::readExpectedDouble(is, "y");
		double      py      = Token::readExpectedDouble(is, "py");

		addParticle(element, x, px, y, py);
	}

	int nlbonds = atoi(Token::readExpected(is, "lbonds").c_str());

	for (int i = 0; i < nlbonds; i++) {
		double      a       = Token::readExpectedDouble(is, "a");
		double      b       = Token::readExpectedDouble(is, "b");
		double      dist    = Token::readExpectedDouble(is, "dist");
		double      k       = Token::readExpectedDouble(is, "k");

		addLinearBond((int)a, (int)b, dist, k);
	}

	return is;
}

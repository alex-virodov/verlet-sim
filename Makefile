all: integrate.exe

run:
	@ make > /dev/null
	@ ./integrate.exe

integrate.exe: integrate.cpp constraint.h coordinate.h particle.h simulation.h bond.h
	g++ -O3 -o $@ $^

clean:
	rm -f integrate.exe

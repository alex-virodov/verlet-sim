DEPS=$(wildcard src/*.[ch]*)

all: integrate.exe

run:
	@ make > /dev/null
	@ ./integrate.exe

integrate.exe: $(DEPS)
	g++ -O3 -o $@ $^

clean:
	rm -f integrate.exe

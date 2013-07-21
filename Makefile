DEPS=$(wildcard src/*.[ch]*)

all: integrate.exe

run:
	@ make > /dev/null
ifdef DT
	@ ./integrate.exe --dt $(DT)
else
	@ ./integrate.exe
endif

integrate.exe: $(DEPS)
	g++ -O3 -o $@ $^

clean:
	rm -f integrate.exe

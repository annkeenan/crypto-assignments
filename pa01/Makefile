CPP=g++
CPPFLAGS=-Wall -std=c++11 -g
INCLUDE=/usr/local/Cellar/gmp/6.1.2_1/include/gmp.h
LD=g++
LDFLAGS=-lgmpxx -lgmp
OBJECTS=main.o

main: $(OBJECTS)
	$(LD) $^ $(LDFLAGS) -o $@

%.o: %.cpp
	$(CPP) -I$(INCLUDE) $(CPPFLAGS) -c $<

.PHONY: clean
clean:
	rm -f *.o main

CPP=g++
CPPFLAGS=-Wall -std=c++11 -g
INCLUDE=/usr/local/include/pbc
LDFLAGS=-lgmpxx -lgmp -lpbc
OBJECTS=main.o

main: $(OBJECTS)
	$(CPP) $^ $(LDFLAGS) -o $@

%.o: %.cpp
	$(CPP) -I$(INCLUDE) $(CPPFLAGS) -c $<

.PHONY: clean
clean:
	rm -f *.o main

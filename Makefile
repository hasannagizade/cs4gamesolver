default:
	make takeaway

takeaway: takeaway.o TakeawayState.o Solver.h.gch
	g++ -Wall -o takeaway takeaway.o TakeawayState.o Solver.h

%.o: %.h %.cpp
	g++ -Wall -c $*.cpp

%.h.gch: %.h %.t.h
	g++ -Wall -c $*.h

clean:
	- rm *.o *.h.gch

realclean: clean
	- rm takeaway

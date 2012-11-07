CXX=g++ -Wall -Wextra -Wundef -Wcast-qual -Wcast-align -Wold-style-cast -Wsign-promo -Wctor-dtor-privacy -Woverloaded-virtual -Wnon-virtual-dtor -Wfloat-equal -Wpointer-arith -Wunreachable-code -Wmissing-declarations -Wmissing-noreturn

default: takeaway kayles connect3 crossout

debug: CXX+=-DDEBUG -ggdb
debug: takeaway kayles connect3 crossout

takeaway: takeaway.o TakeawayState.o Solver.h.gch
	$(CXX) -o takeaway takeaway.o TakeawayState.o

kayles: kayles.o KaylesState.o Solver.h.gch
	$(CXX) -o kayles kayles.o KaylesState.o

connect3: connect3.o Connect3State.o Solver.h.gch
	$(CXX) -o connect3 connect3.o Connect3State.o

crossout: crossout.o CrossoutState.o Solver.h.gch
	$(CXX) -o crossout crossout.o CrossoutState.o

%.o: %.h %.cpp Solver.h.gch
	$(CXX) -c $*.cpp

%.h.gch: %.h %.t.h
	$(CXX) -c $*.h

clean:
	- rm *.o *.h.gch

realclean: clean
	- rm takeaway kayles connect3 crossout

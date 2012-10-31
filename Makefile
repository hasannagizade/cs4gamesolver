CXX=g++ -Wextra

default: takeaway kayles connect3

debug: CXX+=-DDEBUG
debug: takeaway kayles connect3

takeaway: takeaway.o TakeawayState.o Solver.h.gch
	$(CXX) -o takeaway takeaway.o TakeawayState.o Solver.h

kayles: kayles.o KaylesState.o Solver.h.gch
	$(CXX) -o kayles kayles.o KaylesState.o Solver.h

connect3: connect3.o Connect3State.o Solver.h.gch
	$(CXX) -o connect3 connect3.o Connect3State.o Connect3State.h

%.o: %.h %.cpp Solver.h.gch
	$(CXX) -c $*.cpp

%.h.gch: %.h %.t.h
	$(CXX) -c $*.h

clean:
	- rm *.o *.h.gch

realclean: clean
	- rm takeaway kayles connect3

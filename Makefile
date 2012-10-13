CXX=g++ -Wall

default: takeaway kayles

debug: CXX+=-DDEBUG
debug: takeaway kayles

takeaway: takeaway.o TakeawayState.o Solver.h.gch
	$(CXX) -o takeaway takeaway.o TakeawayState.o Solver.h

kayles: kayles.o KaylesState.o Solver.h.gch
	$(CXX) -o kayles kayles.o KaylesState.o Solver.h

%.o: %.h %.cpp Solver.h.gch
	$(CXX) -c $*.cpp

%.h.gch: %.h %.t.h
	$(CXX) -c $*.h

clean:
	- rm *.o *.h.gch

realclean: clean
	- rm takeaway kayles

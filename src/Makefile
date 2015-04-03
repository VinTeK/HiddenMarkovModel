CPP=g++
CFLAGS=-Wall -pedantic -std=c++11 -g
OBJS=main.o HiddenMarkovModel.o
EXE=hmm

$(EXE): $(OBJS)
	$(CPP) $(CFLAGS) -o $(EXE) $^

%.o: %.cpp
	$(CPP) $(CFLAGS) -c $<

clean:
	rm -f *.o $(EXE)

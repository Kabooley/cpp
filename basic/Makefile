gcc_options = -std=c++17 -Wall --pedantic-errors -g

program : main.cpp all.h all.h.gch
	g++ $(gcc_options) -include all.h $< -o $@

all.h.gch : all.h
	g++ $(gcc_options) -x c++-header -o $@ $<

run : program
	./program all.h std

clean :
	rm -f ./program
	rm -f ./all.h.gch

.PHONY : run clean
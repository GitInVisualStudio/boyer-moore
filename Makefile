run: compile
	./main

compile:
	gcc -o main main.c -Wall -Werror -O2

plots: compile
	./main naive > naive.csv
	./main boyermoore > boyer_moore.csv
	python3 plots.py
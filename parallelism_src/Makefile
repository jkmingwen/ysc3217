default : main

OBJS = sudoku_main.o srcs/sudoku_seq.o srcs/sudoku_proc.o srcs/sudoku_pthreads.o srcs/sudoku_openmp.o
CFLAGS = -pthread -fopenmp -I./include

main : $(OBJS)
	gcc -o main $(OBJS) $(CFLAGS)

# VPATH sets alternative directories if file not found in current dir
VPATH = include:../srcs
sudoku_main.o : sudoku_seq.h sudoku_proc.h
sudoku_seq.o : sudoku_seq.h
sudoku_proc.o : sudoku_proc.h
sudoku_pthreads.o : sudoku_pthreads.h
sudoku_openmp.o : sudoku_openmp.h

.PHONY : clean
clean :
	-rm -f main *.o srcs/*.o

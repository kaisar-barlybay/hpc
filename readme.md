For each experiment helpers.c, mmio.c, mmio.h and <file_name>.mtx file should be present in the directory. If you want to debug the program compile with `--DDEBUG` for debug outputs, but please only with small matrix inputs.

# CRS experiment
To compile the program for the experiment execute in terminal: `gcc -g -Wall -fopenmp crs.c -o crs mmio.c`. To run the program execute `./crs <file_name>.mtx` in the terminal window, where <file_name>.mtx is the name of the matrix you want to multiply. 

# COO arrowhead matrix experiment
To compile the program for the experiment execute in terminal: `gcc -g -Wall -fopenmp coo.c -o coo mmio.c`. To run the program execute `./coo <file_name>.mtx` in the terminal window, where <file_name>.mtx is the name of the matrix you want to multiply. 

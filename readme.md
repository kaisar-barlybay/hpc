# 8.1

## static
- compilation: `gcc -g -Wall -fopenmp static.c -o static mmio.c`
- run: `./static <file_name>.mtx`

## dynamic
- compilation: `gcc -g -Wall -fopenmp dynamic.c -o dynamic mmio.c`
- run: `./dynamic <file_name>.mtx`

## static_coo
- compilation: `gcc -g -Wall -fopenmp static_coo.c -o static_coo mmio.c`
- run: `./static_coo <file_name>.mtx`
- compile with `--DDEBUG` for debug outputs, but please only with small matrix inputs


264zuj8m
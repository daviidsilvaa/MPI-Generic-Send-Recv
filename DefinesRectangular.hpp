
#ifndef DEFINESRECTANGULAR_HPP
#define DEFINESRECTANGULAR_HPP

#define DIMX_REC 100
#define DIMY_REC 100
#define NWORKERS_REC 4
#define CONSUMER_REC NWORKERS_REC+1
#define PROC_DIMX_REC DIMX_REC/(NWORKERS_REC/2)
#define PROC_DIMY_REC DIMY_REC/(NWORKERS_REC/2)

#endif

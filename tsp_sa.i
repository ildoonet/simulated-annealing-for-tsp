%module pytspsa

%{
    #define SWIG_FILE_WITH_INIT
    #include "tsp_log.h"
    #include "tsp_sa.h"
%}

%include <std_string.i>
%include "./modules/numpy.i"

%init %{
import_array();
%}

%apply (float* IN_ARRAY2, int DIM1, int DIM2) {(float* c, int n, int m)};
%apply (int* IN_ARRAY1, int DIM1) {(int* solution, int size)};

%include "tsp_sa.h"

enum LogLevel { debug, info, warning, error };
void setLogLevel(LogLevel l);
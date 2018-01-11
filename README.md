# TSP-SA : Traveling Salesman Problem Solver using Simulated Annealing Algorithm

![TSP Image from gurobi.com](tsp.png)

Implementation of TSP Solver based on the paper [Solving the traveling salesman problem
 based on an adaptive simulated annealing algorithm with greedy search](http://www.sciencedirect.com/science/article/pii/S1568494611000573) using Simulated Annealing(SA) Algorithm..

Purpose of this implementation is to provide a package to solve TSPs with simple codes.

## Install

### Pip install

If you use this project only in your python project, installation sing pip is recommended.

```
$ pip install pytspsa
```

### Compile using Cmake

If you want to use this project in your c++ project or modify source codes, this is the recommended way to install.

This project can be used under

+ c++11
+ cmake 3.7>
+ (optional) python2.7> or 3 with numpy

```
(optional)
$ swig -python -c++ tsp_sa.i
(cmake build...)
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## Usage

### Python

```python
import numpy
import pytspsa


solver = pytspsa.Tsp_sa()
c = [
    [0, 0],
    [0, 1],
    [0, 2],
    [0, 3]
]
c = numpy.asarray(c, dtype=numpy.float32)
solver.set_num_nodes(4)
solver.add_by_coordinates(c)
solver.set_t_v_factor(4.0)

# solver.sa() or sa_auto_parameter() will solve the problem.
solver.sa_auto_parameter(12)

# getting result
solution = solver.getBestSolution()

print('Length={}'.format(solution.getlength()))
print('Path= {}'.format(solution.getRoute()))
```

This will generate the result as 

```
Length=6.0
Path= 0-1-2-3-0
```


### CLI

If your tsp problem is fommatted in 'tsplib' format, CLI can be used for simple usage.

```
$ tspsa_exe
[07-05-2017 10:46:29][INFO] usage: ./tspsa_exe [-h] -t tspfile -l loglevel
[07-05-2017 10:46:29][INFO] 	-t --tspfile			input tspfile path(tsplib format).
[07-05-2017 10:46:29][INFO] 	-p --optfile			optimal path file(tsplib format).
[07-05-2017 10:46:29][INFO] 	-l --loglevel			loglevel(debug, info, warn, error), default is info.

$ tspsa_exe -t {tsplib file path} -o {opt file path}
[07-05-2017 10:41:09][INFO] Read ./tsplib/ch150.tsp
[07-05-2017 10:41:09][INFO] Dimension: 150
[07-05-2017 10:41:12][INFO] Currnet Best Solution Value is 8425.699219
[07-05-2017 10:41:14][INFO] Currnet Best Solution Value is 6807.241211
[07-05-2017 10:41:26][INFO] Currnet Best Solution Value is 6723.510742
[07-05-2017 10:41:37][INFO] Currnet Best Solution Value is 6655.667480
[07-05-2017 10:41:48][INFO] Currnet Best Solution Value is 6640.214844

[07-05-2017 10:42:14][INFO] <Optimization Reports>
[07-05-2017 10:42:14][INFO] Solution & Value Validation... Good
[07-05-2017 10:42:14][INFO] Best Solution Value : 6640.214844
[07-05-2017 10:42:14][INFO] time elapsed : 63.887827 seconds

[07-05-2017 10:42:14][INFO] Read Optimal Solution... ./tsplib/ch150.opt.tour
[07-05-2017 10:42:14][INFO] Optimal Solution Value :6532.277832 (101.652359%)
```

### Sample Codes

Please see 

+ python : [tsp_sa.py](./tsp_sa.py)
+ c++ : [main.cpp](./main.cpp)

## Performance Test

### TSPLib

| Set Name | Known Optimal | Best Solution | Gap | Running Time(sec)|
|----------|---------------|---------------|-----|------------------|
| att48 | 33523.71 | 33588.34 | 0.193% | 1.71 |
| berlin52 | 7544.37 | 7544.37 | 0.000% | 1.94 |
| ch130 | 6110.86 | 6192.72 | 1.340% | 14.73 |
| ch150 | 6532.28 | 6733.51 | 3.081% | 21.67 |
| eil51 | 429.98 | 430.86 | 0.204% | 1.72 |
| eil76 | 545.39 | 546.41 | 0.187% | 4.52 |
| eil101 | 642.31 | 659.70 | 2.708% | 5.27 |
| kroA100 | 21285.45 | 21316.38 | 0.145% | 7.90 |
| kroC100 | 20750.76 | 20863.14 | 0.542% | 6.99 |
| kroD100 | 21294.29 | 21648.81 | 1.665% | 9.23 |
| lin105 | 14383.00 | 14586.53 | 1.415% | 6.16 |
| pr76 | 108159.44 | 108159.44 | 0.000% | 3.54 |
| rd100 | 7910.40 | 7944.32 | 0.429% | 8.73 |
| st70 | 678.60 | 679.37 | 0.114% | 4.23 |
| tsp225 | 3859.00 | 4035.36 | 4.570% | 95.90 |
| ulysses16 | 74.11 | 73.99 | -0.163% | 0.43 |
| ulysses22 | 75.67 | 75.31 | -0.470% | 0.91 |
| a280 | 2586.77 | 2750.55 | 6.331% | 230.55 |

* You can compare this result with other commercial packages like [Concorde](http://www.math.uwaterloo.ca/tsp/concorde/benchmarks/bench99.html).


## License : [GPL License](LICENSE.md)

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

# Result should be ..
"""
Length=6.0
Path= 0-1-2-3-0
"""
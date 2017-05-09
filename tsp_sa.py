import pytspsa
import numpy
import time


def do_tsplib(tsp_path, opt_path, t_v_factor, num_runover):
    # open input file.
    with open(tsp_path) as f:
        lines = f.readlines()

    # ready tsplib input file
    solver = pytspsa.Tsp_sa()
    dimension = 0
    c = []
    for num, line in enumerate(lines):
        if 'DIMENSION' in line:
            dimension = int(line.split(':')[-1])
            solver.set_num_nodes(dimension)
        if 'NODE_COORD_SECTION' in line:
            coords = lines[num + 1 : num + dimension + 1]
            for coord in coords:
                idx, x, y = coord.split()
                c.append([float(x), float(y)])

    c = numpy.asarray(c, dtype=numpy.float32)
    solver.add_by_coordinates(c)

    if len(c) <= 100:
        solver.set_t_v_factor(min(t_v_factor, 200.0 / len(c)))
    else:
        solver.set_t_v_factor(min(t_v_factor, 60.0 / len(c)))

    solver.sa_auto_parameter(num_runover)

    # getting result
    solution = solver.getBestSolution()
    copied_solution = pytspsa.Solution(solution)
    opt_solution = pytspsa.Solution(solution)

    if opt_path:
        with open(opt_path) as f:
            lines = f.readlines()

        idxs = []
        for num, line in enumerate(lines):
            if 'TOUR_SECTION' in line:
                tmp_idxs = lines[num + 1 : num + 1 + dimension]
                idxs = []
                for l in tmp_idxs:
                    idxs.extend(l.split())
                idxs = [int(x) for x in idxs if x.isdigit()]

        opt_solution.setSolution(idxs)

    return copied_solution, opt_solution


if __name__ == '__main__':
    __benchmark_set = [
        'att48',
        'berlin52',
        'ch130',
        'ch150',
        'eil51',
        'eil76',
        'eil101',
        # 'gr96',     # geo?
        # 'gr202',    # geo?
        # 'gr666',    # geo?
        'kroA100',
        'kroC100',
        'kroD100',
        'lin105',
        'pr76',
        'rd100',
        'st70',
        'tsp225',
        'ulysses16',
        'ulysses22',
        'a280',
        # 'pcb442',
        # 'pla33810',
        # 'pr1002',
        # 'pr2392',
    ]

    __mode = 'accurate'

    pytspsa.setLogLevel(1)
    if __mode == 'middle':
        t_v_factor = 1
        num_runover = 12
    elif __mode == 'accurate':
        t_v_factor = 8
        num_runover = 24
    elif __mode == 'fast':
        t_v_factor = 0.2
        num_runover = 4


    print('| Set Name | Known Optimal | Best Solution | Gap | Running Time(sec) |')
    print('|--------------------------------------------------------------------|')
    for name in __benchmark_set:
        start_time = time.time()
        solution, optimal = do_tsplib('./tsplib/{}.tsp'.format(name), './tsplib/{}.opt.tour'.format(name), t_v_factor, num_runover)
        elapsed_time = time.time() - start_time
        gap = solution.getlength() / optimal.getlength() * 100 - 100
        print('| %s | %.2f | %.2f | %.3f%% | %.2f |' % (name, optimal.getlength(), solution.getlength(), gap, elapsed_time))

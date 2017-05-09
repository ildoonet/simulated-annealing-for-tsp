#include <iostream>
#include <cmath>
#include <fstream>
#include <getopt.h>
#include "tsp_sa.h"
#include "Clock.h"
#include "tsp_log.h"

using namespace std;

const string PREFIX_DIMENSION = "DIMENSION:";
const string PREFIX_DIMENSION2 = "DIMENSION";
const string PREFIX_SECTION_S = "NODE_COORD_SECTION";
const string PREFIX_TOUR_SECTION = "TOUR_SECTION";


void print_help(string executable) {
    tsplog(info) << "usage: " << executable << " [-h] -t tspfile -l loglevel" << TspLogger::endl;
    tsplog(info) << "\t" << "-t --tspfile" << "\t\t\t" << "input tspfile path(tsplib format)." << TspLogger::endl;
    tsplog(info) << "\t" << "-p --optfile" << "\t\t\t" << "optimal path file(tsplib format)." << TspLogger::endl;
    tsplog(info) << "\t" << "-l --loglevel" << "\t\t\t" << "loglevel(debug, info, warn, error), default is info." << TspLogger::endl;
}

int main (int argc, char* argv[]) {
    static struct option longOptions[] = {
            {"help",           no_argument,       0, 'h'},
            {"tspfile",        required_argument, 0, 't'},
            {"optfile",        optional_argument, 0, 'p'},
            {"loglevel",       optional_argument, 0, 'l'},
            {"v_factor",       optional_argument, 0, 'v'},
            {"num_runover",    optional_argument, 0, 'r'},
    };

    string filename;
    string optname;
    string logLevel = "info";
    float v_factor = 0.2;
    int num_runover = 12;

    int argopt, optionIndex = 0;
    int argcnt = 0;
    while ((argopt = getopt_long(argc, argv, "h:t:l:p:v:r:", longOptions, &optionIndex)) != -1) {
        argcnt ++;
        switch (argopt) {
            case 't': filename = string(optarg); break;
            case 'p': optname = string(optarg); break;
            case 'v': v_factor = (float) atof(optarg); break;
            case 'r': num_runover = atoi(optarg); break;
            case 'l':
                logLevel = string(optarg);
                for (int i = 0; i < LogLevelString->size(); i ++) {
                    if (LogLevelString[i] == logLevel) {
                        setLogLevel(static_cast<LogLevel>(i));
                        break;
                    }
                }

                break;
            case 'h':
            default:
                print_help(argv[0]);
                return(0);
        }
    }

    if (argcnt == 0) {
        print_help(argv[0]);
        return(0);
    }

    ifstream fin;
    try {
        tsplog(info) << "Read " << filename << TspLogger::endl;
        fin.open(filename.data());
    } catch (exception e) {
        tsplog(error) << "File open error. Terminate." << TspLogger::endl;
        exit(-1);
    }

    if (fin.fail()) {
        tsplog(error) << "file open fail. Terminate." << TspLogger::endl;;
        exit(-1);
    }

    // Start to read tsp file
    int dimension = 0;
    float** xy = NULL;
    while (!fin.eof()) {
        if (xy == NULL) {
            // Read meta data
            string segment;
            fin >> segment;

            if (segment == PREFIX_DIMENSION) {
                fin >> dimension;
            } else if (segment == PREFIX_DIMENSION2) {
                fin >> segment;
                fin >> dimension;
            } else if (segment == PREFIX_SECTION_S) {
                xy = new float *[dimension];
            }
        } else {
            // Read coordinates
            int idx;
            fin >> idx;     //Index number

            xy[idx - 1] = new float[2];
            fin >> xy[idx - 1][0] >> xy[idx - 1][1];

            if (idx == dimension) break;
        }
    }

    tsplog(info) << "Dimension: " << dimension << TspLogger::endl;

    srand((unsigned int) time(0));
    Tsp_sa solver;
    solver.set_num_nodes(dimension);
    solver.add_by_coordinates(xy);
    solver.set_t_v_factor(v_factor);

    Clock timer;
    
    solver.sa_auto_parameter(num_runover);
    Solution& best_solution = solver.getBestSolution();

    cout << endl;
    tsplog(info) << "<Optimization Reports>" << TspLogger::endl;
    tsplog(info) << "Solution & Value Validation... ";

    if (best_solution.verify())
        tsplog(info) << "Good" << TspLogger::endl;
    else
        tsplog(info) << "Bad(Not feasible solution" << TspLogger::endl;

    tsplog(info) << "Best Solution Value : " << best_solution.getlength() << TspLogger::endl;
    tsplog(info) << "time elapsed : " << timer.getseconds() << " seconds" << TspLogger::endl;

    fin.close();

    // if optimal solution provided
    if (optname.size() > 0) {
        cout << endl;

        try {
            tsplog(info) << "Read Optimal Solution... " << optname << TspLogger::endl;
            fin.open(optname.data());
        } catch (exception e) {
            tsplog(error) << "File open error. Terminate." << TspLogger::endl;
            exit(-1);
        }

        if (fin.fail()) {
            tsplog(error) << "file open fail. Terminate." << TspLogger::endl;;
            exit(-1);
        }

        // Start to read tsp file
        int solution[dimension];
        while (!fin.eof()) {
            // Read meta data
            string segment;
            fin >> segment;

            if (segment == PREFIX_TOUR_SECTION) {
                for (int i = 0; i < dimension; i++) {
                    fin >> solution[i];
                }

                break;
            }
        }

        // Print Optimal Solution Information
        Solution opt_solution(dimension, solver.get_distance_matrice());
        opt_solution.setSolution(solution, dimension);
        tsplog(info) << "Optimal Solution Value :" << opt_solution.getlength() << " (" << best_solution.getlength() / opt_solution.getlength() * 100 << "%)" << TspLogger::endl;

        fin.close();
    }

    return 0;
}
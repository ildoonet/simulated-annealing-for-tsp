#include <iostream>
#include <fstream>
#include <cmath>
#include "tsp_sa.h"
#include "tsp_log.h"
#include "Clock.h"

using namespace std;


void Tsp_sa::add_by_coordinates(float** c) {
    /* Dynamically Memory Allocation and Construct Distance Matrix */
    float** dists_f = new float*[num_nodes];

    for (int i = 0; i < num_nodes; i++) {
        dists_f[i] = new float[num_nodes];

        for (int j = 0; j < num_nodes; j++) {

            if (i == j) {
                dists_f[i][j] = BIGM;
                continue;
            }

            float tmp = (float) (pow((double) c[i][0] - c[j][0], 2.0) + pow((double) c[i][1] - c[j][1], 2.0));
            dists_f[i][j] = (float) pow((double) tmp, 0.5);
        }
    }

    this->add_by_distances(dists_f);

    for (int i = 0; i < num_nodes; i++) {
        delete[] dists_f[i];
    }
    delete[] dists_f;
}

void Tsp_sa::add_by_coordinates(float* c, int n, int m) {
    srand((unsigned int) time(0));

    int i, j;
    float **arr = (float **) malloc(n * sizeof(float *));
    for (i = 0; i < n; i++) {
        arr[i] = (float *) malloc(m * sizeof(float));
        for (j = 0; j < m; j++) {
            arr[i][j] = c[i * m + j];
        }
    }

    add_by_coordinates(arr);

    for (i = 0; i < n; i++) {
        delete[] arr[i];
    }
    delete[] arr;
}

void Tsp_sa::set_num_nodes(int size) {
    this->num_nodes = size;
    this->dists_f = new float*[size];
    for (int i = 0; i < size; i++) {
        this->dists_f[i] = new float[size];
    }
}

void Tsp_sa::add_by_distances(float **c) {
    for (int i = 0; i < this->num_nodes; i ++) {
        for (int j = 0; j < this->num_nodes; j ++) {
            this->dists_f[i][j] = c[i][j];
        }
    }

    /* Solution Space Allocation */
    current_solution = new Solution(num_nodes, dists_f);
    temp_solution = new Solution(num_nodes, dists_f);
    best_solution = new Solution(num_nodes, dists_f);

    /* Initialization */
    optimal_value = BIGM;
}

void Tsp_sa::add_by_distances(float* c, int n, int m) {
    srand((unsigned int) time(0));

    int i, j;
    float **arr = (float **) malloc(n * sizeof(float *));
    for (i = 0; i < n; i++) {
        arr[i] = (float *) malloc(m * sizeof(float));
        for (j = 0; j < m; j++) {
            arr[i][j] = c[i * m + j];
        }
    }

    add_by_distances(arr);

    for (i=0; i < n; i++) {
        delete[] arr[i];
    }
    delete[] arr;
}

void Tsp_sa::init_random()
{
    current_solution->setInit();
    
    /* Print */
    tsplog(debug) << "New Solution Constructed randomly" << TspLogger::endl;
    tsplog(debug) << "New Random Solution Value : " << current_solution->getlength() << TspLogger::endl;
}

float Tsp_sa::get_alpha() {
    return this->alpha;
}

void Tsp_sa::set_alpha(float alpha) {
    this->alpha = alpha;
}

float Tsp_sa::get_beta() {
    return this->beta;
}

void Tsp_sa::set_beta(float beta) {
    this->beta = beta;
}

void Tsp_sa::set_current_temperature(double t) {
    this->t_current = t;
}

void Tsp_sa::set_cooling_rate(double t) {
    this->t_cool = t;
}

void Tsp_sa::set_end_temperatue(double t) {
    this->t_end = t;
}

void Tsp_sa::set_temperature_greedy(double t) {
    this->t_greedy = t;
}

void Tsp_sa::set_t_v_factor(double t) {
    this->t_v_factor = t;
}

float** Tsp_sa::get_distance_matrice() {
    return this->dists_f;
}

void Tsp_sa::sa() {
    sa(INT32_MAX);
}

void Tsp_sa::sa(const int maxtime_sec)
{
    Clock timer;

    double t_current = this->t_current;
    double t_cool = this->t_cool;
    double t_end = this->t_end;
    double t_greedy = this->t_greedy;

    double improve;

    int t_v = (int) (num_nodes * this->t_v_factor);
    int cnt_div = 0;
    
    int G = 0;
    
    double sqrt_n = pow(num_nodes, 0.5);

    while (t_current > t_end && cnt_div < t_v && timer.getseconds() < maxtime_sec)
    {
        t_cool = (alpha * sqrt_n - 1.0) / (alpha * sqrt_n);
        t_greedy = num_nodes * beta;

        double best_improve = -1 * BIGM;
        do {
            double rand_to_select_alg = (double)rand() / (double)RAND_MAX;
            
            if(rand_to_select_alg<=0.9)
                improve = current_solution->getNewSolution_br();
            else
                improve = current_solution->getNewSolution_vi();
            
            //G+1
            G++;

            if(improve>0)
                break;
            
            /* Select best */
            if (best_improve < improve)
            {
                best_improve = improve;
                temp_solution->copy(*current_solution);
            }
            
            /* Termination */
            if (G >= t_greedy)
            {
                double upper = improve / t_current;
                upper *= 10.0 * num_nodes / optimal_value;
                double rho = exp(upper);
                double probability = (double)rand() / RAND_MAX;

                if(probability < rho)
                {
                    tsplog(debug) << "Diversification! with rho of " << rho << ", prob " << probability << TspLogger::endl;
                    temp_solution->setNext();
                    current_solution->copy(*temp_solution);
                    
                }
                
                /* Compulsive Accept */
                cnt_div ++;
                break;
            }
            
        } while (improve < 0 && timer.getseconds() < maxtime_sec);
    
        if(improve > 0)
        {
            
            current_solution->setNext();
            
            if(optimal_value > current_solution->getlength())
            {
                cnt_div = 0;
                
                optimal_value = current_solution->getlength();

                tsplog(debug) << "Optimal Value is updated, " << optimal_value << TspLogger::endl;
                
                best_solution->copy(*current_solution);

                continue;
            }//end if(optimality)
        }//end if
        
        t_current *= t_cool;
        G = 0;
    }//end while
    
    //Ends with Local Search
    best_solution->localSearch();
}

void Tsp_sa::sa_auto_parameter(const int num_runover) {
    const float init_alpha = this->get_alpha();
    const float init_beta = this->get_beta();

    Solution best_solution(this->num_nodes, this->get_distance_matrice());
    float current_best = BIGM;

    tsplog(debug) << "t_v_factor=" << t_v_factor << ", num_runover=" << num_runover << TspLogger::endl;

    for (int new_init_sol = 0; new_init_sol < num_runover; new_init_sol++) {
        init_random();
        sa(20);     // TODO

        if (new_init_sol % 4 == 0) {
            set_alpha(init_alpha);
            set_beta(init_beta);
        } else {
            set_alpha((float) (get_alpha() * -1.0 / 4.0 * exp(-1 * new_init_sol) + 1));
            set_beta((float) (get_beta() * 7.0 / 2.0 * exp(-1 * new_init_sol) + 1));
        }

        if (current_best > getBestSolution().getlength()) {
            current_best = getBestSolution().getlength();
            best_solution.copy(getBestSolution());

            tsplog(debug) << "Currnet Best Solution Value is " << current_best << TspLogger::endl;
        }

        setBestSolution_init();
    }

    getBestSolution().copy(best_solution);
}

float Tsp_sa::getvalue() {
    return optimal_value;
}

Solution& Tsp_sa::getBestSolution() {
    return *best_solution;
}

void Tsp_sa::setBestSolution_init() {
    optimal_value = BIGM;
}

Tsp_sa::~Tsp_sa() {
    delete current_solution;
    delete best_solution;
    delete temp_solution;

    for (int i = 0; i < num_nodes; i++) {
        delete[] dists_f[i];
    }
    delete[] dists_f;
}

#ifndef tsp_sa_tsp_sa_h
#define tsp_sa_tsp_sa_h

#include <cstring>

using namespace std;

const int BIGM = 9999999;
const int NUMOPT = 2;
const int NUMVI = 3;

class Solution{
private:
    
    int num_nodes;
    float length;
    
    /* Block Reverse Mutation */
    bool *make_zero_xy[NUMOPT];
    bool *make_one_xy[NUMOPT];
    int reverse_start = 0;
    float pluslength;
    
    /* Vertex Insert Mutation */
    int index_node[2][NUMVI];
    
    void (Solution::*psetNext)();
    
public:
    bool ** solution;
    
    float ** p_dists_f;

    // Functions
    Solution(int n, float ** dists_f);
    Solution(Solution& Other);

    void _init(const int n, const float** dists_f);

    float getlength();
    void setlength();

    string getRoute();

    /* New Solution Generating Functions */
    void setInit();
    
    /* Block Reverse Mutation Function */
    int getNewSolution_br();
    void setNewSolution_br();
    
    /* Vertex Insert Mutation Functions */
    int getNewSolution_vi();
    void setNewSolution_vi();
    
    /* */
    void setNext();
    void reverse(const int idx);
    void localSearch();
    
    /* Operator */
    void copy(const Solution &Other);
    void setSolution(int* solution, int size);
    
    /* Verifier */
    bool verify();
};

class Tsp_sa
{
private:
    int num_nodes;
    float ** dists_f;

    float alpha = 160000;
    float beta = 0.333333;
    double t_current = 100.0;
    double t_cool = 0.99;
    double t_end = 0.0025;
    double t_greedy = 100.0;
    double t_v_factor = 10.0;

    Solution *current_solution;
    Solution *temp_solution;
    Solution *best_solution;

    float optimal_value;
    
public:
    ~Tsp_sa();

    void set_num_nodes(int size);
    void add_by_coordinates(float** c);
    void add_by_coordinates(float* c, int n, int m);
    void add_by_distances(float** c);
    void add_by_distances(float* c, int n, int m);

    void init_random();

    float get_alpha();
    void set_alpha(float alpha);
    float get_beta();
    void set_beta(float beta);
    void set_current_temperature(double t);
    void set_cooling_rate(double t);
    void set_end_temperatue(double t);
    void set_temperature_greedy(double t);
    void set_t_v_factor(double t);

    float** get_distance_matrice();

    void sa();
    void sa(const int maxtime_sec);
    void sa_auto_parameter(const int num_runover);

    float getvalue();
    Solution& getBestSolution();
    void setBestSolution_init();
};

#endif

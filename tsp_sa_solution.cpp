#include <iostream>
#include <cmath>
#include <sstream>
#include "tsp_sa.h"

Solution::Solution(int n, float ** dists_f)
{
    _init(n, (const float **) dists_f);

    this->setInit();
}

Solution::Solution(Solution &Other) {
    _init(Other.num_nodes, (const float **) Other.p_dists_f);

    copy(Other);
}

void Solution::_init(const int n, const float** dists_f) {
    this->num_nodes = n;
    p_dists_f = new float*[n];
    for (int i = 0; i < n; i++) {
        p_dists_f[i] = new float[n];
        for (int j = 0; j < n; j++) {
            p_dists_f[i][j] = dists_f[i][j];
        }
    }

    /* Memory Allocation */
    solution = new bool*[n];
    for(int i=0;i<n;i++)
    {
        solution[i] = new bool[n];
    }//end for i
}



void Solution::setInit()
{
    /* Initial Solution */
    int *init = new int[num_nodes];
    for (int i = 0; i < num_nodes; i++) {
        init[i] = i;
    }
    
    /* Shuffling Initial Solution */
    for (int i = 0; i < num_nodes; i++) {
        int k = rand() % num_nodes;
        int l = rand() % num_nodes;

        int tmp = init[k];
        init[k] = init[l];
        init[l] = tmp;
    }//end for i
    
    /* Save Initial Solution and calculate length */
    for (int i = 0; i < num_nodes; i++) {
        for (int j = 0; j < num_nodes; j++) {
            solution[i][j] = false;
        }
    }

    for (int i = 0; i < num_nodes; i++) {
        int x = init[i];
        int y = init[(i + 1) % num_nodes];

        solution[x][y] = true;
    }

    setlength();
    
    //Memory Free
    delete [] init;
}

float Solution::getlength()
{
    return length;
}

void Solution::setlength() {
    length = 0;

    for (int i = 0; i < num_nodes; i++) {
        for (int j = 0; j < num_nodes; j++) {
            if (solution[i][j]) {
                length += p_dists_f[i][j];
                break;
            }
        }
    }
}

void Solution::setNewSolution_br()
{
    /* Update length */
    //length -= pluslength;
    
    for(int i=0;i<2;i++)
    {
        *make_zero_xy[i] = false;
    }
    
    /* One part should be reverse ordered */
    reverse(reverse_start);

    /* Set one */
    for(int i=0;i<2;i++)
    {
        *make_one_xy[i] = true;
    }
    
    setlength();
    
    //getchar();
}

void Solution::reverse(const int idx)
{
    for(int j=0;j<num_nodes;j++)
    {
        if(solution[idx][j] == true)
        {
            solution[idx][j] = false;
            reverse(j);
            solution[j][idx] = true;
            
            return;
        }
    }//end for j
}

int Solution::getNewSolution_br()
{
    /* Randomly Pick Two Number */
    int xy[2][2];
    xy[0][0] = rand()%num_nodes;
    
    do {
        xy[1][0] = rand()%num_nodes;
    } while (xy[0][0]==xy[1][0]);
    
    /* Calculate Improvement */
    for(int j=0;j<num_nodes;j++)
    {
        for(int i=0;i<2;i++)
        {
            if(solution[xy[i][0]][j]==true)
            {
                xy[i][1] = j;
                
            }
        }//end for i
    }//end for j
    
    float improve =
                    p_dists_f[xy[0][0]][xy[0][1]] 
                    + p_dists_f[xy[1][0]][xy[1][1]];
    improve -=      p_dists_f[xy[0][0]][xy[1][0]]
                    + p_dists_f[xy[0][1]][xy[1][1]];
    
    make_zero_xy[0] = &solution[xy[0][0]][xy[0][1]];
    make_zero_xy[1] = &solution[xy[1][0]][xy[1][1]];
    
    make_one_xy[0] = &solution[xy[0][0]][xy[1][0]];
    make_one_xy[1] = &solution[xy[0][1]][xy[1][1]];
    
    reverse_start = xy[0][1];
    
    
    /* Set Next Function */
    psetNext = &Solution::setNewSolution_br;
    
    /* return improvements */
    pluslength = improve;
    return improve;
}

void Solution::copy(const Solution &Other)
{
    if (this != &Other) {
        num_nodes = Other.num_nodes;
        reverse_start = Other.reverse_start;
        pluslength = Other.pluslength;
        length = Other.length;
        psetNext = Other.psetNext;
        p_dists_f = Other.p_dists_f;

        for (int i = 0; i < num_nodes; i++) {
            for (int j = 0; j < num_nodes; j++) {
                solution[i][j] = Other.solution[i][j];

                for (int k = 0; k < NUMOPT; k++) {
                    if (Other.make_zero_xy[k] == &(Other.solution[i][j])) {
                        make_zero_xy[k] = &solution[i][j];
                    }
                    if (Other.make_one_xy[k] == &(Other.solution[i][j])) {
                        make_one_xy[k] = &solution[i][j];
                    }

                }//end for k

            }//end for j
        }//end for i

        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < NUMVI; j++) {
                index_node[i][j] = Other.index_node[i][j];
            }
        }
    }
}

void Solution::setSolution(int *solution, int size) {
    for (int i = 0; i < num_nodes; i++) {
        for (int j = 0; j < num_nodes; j++) {
            this->solution[i][j] = false;
        }
    }

    for (int i = 0; i < size; i++) {
        this->solution[solution[i] - 1][solution[(i+1) % size] - 1] = true;
    }
    setlength();
}

/* Vertex Insert Mutation Functions */
int Solution::getNewSolution_vi()
{
    /* Randomly Pick Two Number */
    int xy[2];
    
    xy[0] = rand()%num_nodes;
    
    do {
        xy[1] = rand()%num_nodes;
        
        if(xy[0]!=xy[1] && solution[xy[0]][xy[1]]==false && solution[xy[1]][xy[0]]==false)
            break;
        
    } while (1);
    
    index_node[0][1] = xy[0];
    index_node[1][1] = xy[1];
    
    for(int j=0;j<num_nodes;j++)
    {
        for(int k=0;k<2;k++)
        {
            int i = xy[k];
            
            if(solution[i][j] == true)
            {
                index_node[k][2] = j;
            }
            
            if(solution[j][i] == true)
            {
                index_node[k][0] = j;
            }
        }//end for k
        
    }//end for j
    
    
    /* Improvements */
    int improvement = 0;
    improvement += p_dists_f [ index_node[0][1] ][ index_node[0][2] ]
    + p_dists_f[ index_node[1][0] ][ index_node[1][1] ]
    + p_dists_f[ index_node[1][1] ][ index_node[1][2] ];
    
    improvement -= p_dists_f [ index_node[0][1] ][ index_node[1][1] ]
    + p_dists_f[ index_node[1][0] ][ index_node[1][2] ]
    + p_dists_f[ index_node[1][1] ][ index_node[0][2] ];
    
    psetNext = &Solution::setNewSolution_vi;
                                    
    return improvement;
    
}

void Solution::setNewSolution_vi()
{
    /* 새로운 해 공간으로의 이동 - Vertex Insertion */
    solution[ index_node[0][1] ][ index_node[0][2] ] = false;
    solution[ index_node[1][0] ][ index_node[1][1] ] = false;
    solution[ index_node[1][1] ][ index_node[1][2] ] = false;
    
    solution[ index_node[0][1] ][ index_node[1][1] ] = true;
    solution[ index_node[1][0] ][ index_node[1][2] ] = true;
    solution[ index_node[1][1] ][ index_node[0][2] ] = true;
    
    bool a =verify()
    ;
    
    if(a==false)
        getchar();
    
    setlength();
}

/* etc */
bool Solution::verify()
{
    for(int i=0;i<num_nodes;i++)
    {
        int cnt=0;
        int cnt2=0;
        for(int j=0;j<num_nodes;j++)
        {
            if(solution[i][j]==true)
                cnt++;
            
            if(solution[j][i]==true)
                cnt2++;
            
            if(i-j==0 && solution[i][j]==true)
                return false;
            
        }
        
        if(cnt!=1 || cnt2!=1)
            return false;
    }
    
    return true;
}

void Solution::setNext(){
    (this->*psetNext)();
}

void Solution::localSearch() {
    while(1)
    {
        float best_improve;
        best_improve = -0.1;
        
        //Vertext Insert Search
        {
            int xy[2];
            int tmp_index_node[2][NUMVI];

            for (int i = 0; i < num_nodes; i++) {
                for (int j = 0; j < num_nodes; j++) {
                xy[0] = i;
                xy[1] = j;

                if (xy[0] == xy[1] || solution[xy[0]][xy[1]] || solution[xy[1]][xy[0]])
                    continue;
                
                tmp_index_node[0][1] = xy[0];
                tmp_index_node[1][1] = xy[1];

                for (int l = 0; l < num_nodes; l++) {
                    for (int k = 0; k < 2; k++) {
                        int p = xy[k];

                        if (solution[p][l]) {
                            tmp_index_node[k][2] = l;
                        }

                        if (solution[l][p]) {
                            tmp_index_node[k][0] = l;
                        }
                    }//end for k
                }//end for l
            
                /* Improvements */
                float improvement = 0.0f;
                improvement += (p_dists_f [ tmp_index_node[0][1] ][ tmp_index_node[0][2] ]
                + p_dists_f[ tmp_index_node[1][0] ][ tmp_index_node[1][1] ]
                + p_dists_f[ tmp_index_node[1][1] ][ tmp_index_node[1][2] ]);
                
                improvement -= (p_dists_f [ tmp_index_node[0][1] ][ tmp_index_node[1][1] ]
                + p_dists_f[ tmp_index_node[1][0] ][ tmp_index_node[1][2] ]
                + p_dists_f[ tmp_index_node[1][1] ][ tmp_index_node[0][2] ]);
                
                if(improvement > best_improve)
                {
                    best_improve = improvement;
                    
                    for(int k=0;k<2;k++)
                    {
                        for(int l=0;l<NUMVI;l++)
                        {
                            this->index_node[k][l] = tmp_index_node[k][l];
                        }
                    }
                    
                    psetNext = &Solution::setNewSolution_vi;
                }

            }//end for j
        }//end for i
        }
        
        //Block Reverse Search
        
        
        if(best_improve<=0.1)
            return;
        else {
            //cout << "!!!" << best_improve << endl;
            setNext();
            //cout << getlength() << endl;
        }
    }
    
    
}

string Solution::getRoute() {
    stringstream route;
    route << "0";
    int curr_p = 0;
    for (int i = 0; i < this->num_nodes; i++) {
        for (int j = 0; j < this->num_nodes; j++) {
            if (this->solution[curr_p][j]) {
                route << "-" << j;
                curr_p = j;
                break;
            }
        }
    }

    return string(route.str());
}
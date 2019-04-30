#ifndef MAKEMODESHEXAGONAL_H
#define MAKEMODESHEXAGONAL_H
#include <math.h> 
#include "parameters.h"
#include <armadillo>

using namespace arma;
 
extern void makemodesHexagonal(mat& cs,mat& Ras,vec& Ka); 
bool part_of(const int n,const vec& indices,const int len_indices,const int upper_limit);
double periodic_boundariesx(double d);
double periodic_boundariesy(double d);

#endif

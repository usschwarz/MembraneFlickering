/* 
 * File:   matrix.h
 * Author: Julia Jäger
 *
 * Here the entries of the matrix, that describes how the energy in all modes, is calculated.  
 */

#ifndef MATRIX_H
#define MATRIX_H
#include "parameters.h"
#include <armadillo>

using namespace arma;

extern void matrix(mat& m,const mat& cs,const mat& Ras,const vec& Ka);
double dot(const mat& vectors,int first,int second);
double dot_el(double v1x, double v1y, double v2x, double v2y);
void addition(vec& vect, double v1x, double v1y, double v2x, double v2y, int sign);

#endif

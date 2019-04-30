/* 
 * File:   parameters.h
 * Author: Julia Jäger
 *
 * Here the parameters are initialized from the parameter file and some are calculated.   
 */

#ifndef PARAMETERS_H
#define PARAMETERS_H
#include <math.h> 
#include <string>

using namespace std;

namespace parameters
{
    	extern double lengthunit;
	extern double kBT;
	extern int n;
	extern int nx;
	extern int ny;
	extern int N;
	extern int Nx;
	extern int Ny;
	extern double Lx;
	extern double Ly;
	extern int indqs;
	extern int realmodes;
	extern int allindqs;
	extern int Msize;
	extern double a; 	
	extern int xn; 
	extern int yn; 
	extern double h;
	extern double c;
	extern bool midpoint;
	extern double k1;
	extern double k2;
	extern double kknob;
	extern int lattice;
	extern double delta;
	extern int number_springs;  
	extern double kappaLxLy;
	extern double sigmaLxLy;
	extern int knobs;
	extern int number_midpoints;
	extern double knobdensity;
	extern double meshsize;

	extern string runfolder;
	extern string springFile;
	extern string paraFile;
	extern string amplitudesFile;
	extern string ampsFile;
	extern string diagampsFile;
	extern string avampsFile;

	extern void initParameters(int argc, char* argv[]); 
	extern void writeToFile(string fileName);

}  

#endif

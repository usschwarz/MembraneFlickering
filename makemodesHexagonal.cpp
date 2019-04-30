#include "makemodesHexagonal.h"
#include <iostream>
#define _USE_MATH_DEFINES 
#include <cmath>
#include "parameters.h"
#include <armadillo>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

using namespace std;
using namespace arma;

void makemodesHexagonal(mat& cs,mat& Ras,vec& Ka)
{
	
	mat qs(2,parameters::allindqs, fill::zeros);

//Define qs: independent k-modes (includes the 4 real modes)

	for (int nx = 0; nx < parameters::nx+1; nx++) {
		for (int ny = 0; ny < parameters::ny+1; ny++) {
			qs(0,nx*(parameters::ny+1)+ny)=2*M_PI*nx/parameters::Lx;
            		qs(1,nx*(parameters::ny+1)+ny)=2*M_PI*ny/parameters::Ly;
		}
	}
	for (int nx = 1; nx < parameters::nx; nx++) {
		for (int ny = 1; ny < parameters::ny; ny++) {
			qs(0,(parameters::nx+1)*(parameters::ny+1)+(nx-1)*(parameters::ny-1)+ny-1)=-2*M_PI*nx/parameters::Lx;
            		qs(1,(parameters::nx+1)*(parameters::ny+1)+(nx-1)*(parameters::ny-1)+ny-1)=2*M_PI*ny/parameters::Ly;
		}
	}

//Bring modes in form of c-vector: 4 real modes first and then complex
//    q-modes twice (once for aq and then for bq)

	cs(0,0)=qs(0,0);
	cs(1,0)=qs(1,0);
        cs(0,1)=qs(0,parameters::nx*(parameters::ny+1));
        cs(1,1)=qs(1,parameters::nx*(parameters::ny+1));
        cs(0,2)=qs(0,parameters::ny);
        cs(1,2)=qs(1,parameters::ny);
        cs(0,3)=qs(0,parameters::nx*(parameters::ny+1)+parameters::ny);
        cs(1,3)=qs(1,parameters::nx*(parameters::ny+1)+parameters::ny);
        int qindex=1;

	for (int i =parameters::realmodes; i<parameters::Msize; i++){
		if (qindex==parameters::nx*(parameters::ny+1) or qindex==parameters::ny or qindex==parameters::nx*(parameters::ny+1)+parameters::ny) {
		    qindex=qindex+1; 
		}
		if (qindex>parameters::allindqs-1){
		    qindex=1;
		}
		cs(0,i)=qs(0,qindex);
		cs(1,i)=qs(1,qindex);
		qindex=qindex+1;
	}


//Define springs and spring constants for normal spectrin connections
	int hexagons=parameters::xn*parameters::yn*2;
	mat midpoints(2,hexagons);
	int m=0;
	int c=0;
	double nx;
	double ny;

	for (int y=0; y< parameters::yn; y++){
		for (int x=0; x< parameters::xn; x++){
			//line at even h/2
			nx=x*3*parameters::a;
			ny=y*parameters::h;

			Ras(0,c)=nx+parameters::c;
			Ras(1,c)=ny;
			Ka(c)=parameters::k1;
			c++;

			Ras(0,c)=nx+parameters::c+parameters::a;
			Ras(1,c)=ny;
			Ka(c)=parameters::k1;
			c++;


			midpoints(0,m)=nx+parameters::c+2*parameters::a;
			midpoints(1,m)=ny;
			m++;

			//line at odd h/2
			nx=x*3*parameters::a;
			ny=(y+0.5)*parameters::h;

			Ras(0,c)=nx;
			Ras(1,c)=ny;
			Ka(c)=parameters::k1;
			c++;

			Ras(0,c)=nx+2*parameters::c+parameters::a;
			Ras(1,c)=ny;
			Ka(c)=parameters::k1;
			c++;

			midpoints(0,m)=nx+parameters::a;
			midpoints(1,m)=ny;
			m++;
		}
	}               

	vec choose_n(parameters::number_midpoints);
	choose_n.fill(hexagons);
	srand (time(NULL));
	int nn;
	if (parameters::number_midpoints>=0 and parameters::number_midpoints<hexagons){
		for (int i=0; i<parameters::number_midpoints; i++){
			nn = rand() % hexagons;
			while (part_of(nn,choose_n,parameters::number_midpoints,hexagons)){	
				nn = rand() % hexagons;
			}
			choose_n(i)=nn;
			Ras(0,c)=midpoints(0,nn);
			Ras(1,c)=midpoints(1,nn);
			Ka(c)=parameters::k2;
			c++;
		}
	}else if (parameters::number_midpoints==hexagons){
		for (int i=0; i<parameters::number_midpoints; i++){
			Ras(0,c)=midpoints(0,i);
			Ras(1,c)=midpoints(1,i);
			Ka(c)=parameters::k2;
			c++;
		}
	}else{
		cout << "Error with meshsize." << endl;
	}


//Define springs that represent knobs (49 springs per knob)

//choose points for knobs from already existing springs 
	vec indices(parameters::knobs);
	indices.fill(parameters::number_springs);
	mat knob_midpoints(2,parameters::knobs);
	int n;
	double mx,my;

	if (parameters::knobs>parameters::number_springs){
		cout << "Knob density too high" << endl;
	}

	for (int i=0; i<parameters::knobs; i++){
		n = rand() % c;
		while (part_of(n,indices,parameters::knobs,parameters::number_springs)){	
			n = rand() % c;
		}
		indices(i)=n;
		knob_midpoints(0,i)=round(Ras(0,n)/parameters::lengthunit)*parameters::lengthunit;
		knob_midpoints(1,i)=round(Ras(1,n)/parameters::lengthunit)*parameters::lengthunit;
	}

//add 49 springs for each knob
	for (int i=0; i<parameters::knobs; i++){
		mx=knob_midpoints(0,i);
		my=knob_midpoints(1,i);

		Ras(0,c)=periodic_boundariesx(mx);
		Ras(1,c)=periodic_boundariesy(my);
		Ka(c)=parameters::kknob;
		c++;
		for (int j=1; j<5; j++){
			Ras(0,c)=periodic_boundariesx(mx+j*parameters::lengthunit);
			Ras(1,c)=periodic_boundariesy(my);
			Ka(c)=parameters::kknob;
			c++;
			Ras(0,c)=periodic_boundariesx(mx-j*parameters::lengthunit);
			Ras(1,c)=periodic_boundariesy(my);
			Ka(c)=parameters::kknob;
			c++;
			Ras(0,c)=periodic_boundariesx(mx);
			Ras(1,c)=periodic_boundariesy(my+j*parameters::lengthunit);
			Ka(c)=parameters::kknob;
			c++;
			Ras(0,c)=periodic_boundariesx(mx);
			Ras(1,c)=periodic_boundariesy(my-j*parameters::lengthunit);
			Ka(c)=parameters::kknob;
			c++;
		}
		for (int j=1; j<4; j++){
			Ras(0,c)=periodic_boundariesx(mx+parameters::lengthunit);
			Ras(1,c)=periodic_boundariesy(my+j*parameters::lengthunit);
			Ka(c)=parameters::kknob;
			c++;
			Ras(0,c)=periodic_boundariesx(mx-parameters::lengthunit);
			Ras(1,c)=periodic_boundariesy(my+j*parameters::lengthunit);
			Ka(c)=parameters::kknob;
			c++;
			Ras(0,c)=periodic_boundariesx(mx+parameters::lengthunit);
			Ras(1,c)=periodic_boundariesy(my-j*parameters::lengthunit);
			Ka(c)=parameters::kknob;
			c++;
			Ras(0,c)=periodic_boundariesx(mx-parameters::lengthunit);
			Ras(1,c)=periodic_boundariesy(my-j*parameters::lengthunit);
			Ka(c)=parameters::kknob;
			c++;

			Ras(0,c)=periodic_boundariesx(mx+2*parameters::lengthunit);
			Ras(1,c)=periodic_boundariesy(my+j*parameters::lengthunit);
			Ka(c)=parameters::kknob;
			c++;
			Ras(0,c)=periodic_boundariesx(mx-2*parameters::lengthunit);
			Ras(1,c)=periodic_boundariesy(my+j*parameters::lengthunit);
			Ka(c)=parameters::kknob;
			c++;
			Ras(0,c)=periodic_boundariesx(mx+2*parameters::lengthunit);
			Ras(1,c)=periodic_boundariesy(my-j*parameters::lengthunit);
			Ka(c)=parameters::kknob;
			c++;
			Ras(0,c)=periodic_boundariesx(mx-2*parameters::lengthunit);
			Ras(1,c)=periodic_boundariesy(my-j*parameters::lengthunit);
			Ka(c)=parameters::kknob;
			c++;
		}
		for (int j=1; j<3; j++){
			Ras(0,c)=periodic_boundariesx(mx+3*parameters::lengthunit);
			Ras(1,c)=periodic_boundariesy(my+j*parameters::lengthunit);
			Ka(c)=parameters::kknob;
			c++;
			Ras(0,c)=periodic_boundariesx(mx-3*parameters::lengthunit);
			Ras(1,c)=periodic_boundariesy(my+j*parameters::lengthunit);
			Ka(c)=parameters::kknob;
			c++;
			Ras(0,c)=periodic_boundariesx(mx+3*parameters::lengthunit);
			Ras(1,c)=periodic_boundariesy(my-j*parameters::lengthunit);
			Ka(c)=parameters::kknob;
			c++;
			Ras(0,c)=periodic_boundariesx(mx-3*parameters::lengthunit);
			Ras(1,c)=periodic_boundariesy(my-j*parameters::lengthunit);
			Ka(c)=parameters::kknob;
			c++;
		}


	}

	cout << c << " " << parameters::number_springs << endl;

}

bool part_of(const int n,const vec& indices,const int len_indices,const int upper_limit){
	vec sorted=sort(indices);
	int i=0;
	bool in=false;
	while (n>=sorted(i) and i<len_indices and sorted(i)<upper_limit){
		if (sorted(i)==n){
			in=true;
			break;
		}
		i++;
	}
	return in;
}

double periodic_boundariesx(double d){
	double new_d=d;
	double maxX=parameters::xn*3*parameters::a;
	if (d>maxX){
		new_d=d-maxX;
	}
	if (d<0){
		new_d=d+maxX;
	}
	return new_d;
}


double periodic_boundariesy(double d){
	double new_d=d;
	double maxY=parameters::yn*parameters::h;
	if (d>maxY){
		new_d=d-maxY;
	}
	if (d<0){
		new_d=d+maxY;
	}
	return new_d;
}

























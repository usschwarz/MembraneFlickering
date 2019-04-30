#include <iostream>
#include <fstream>
#include <math.h>  
#include "parameters.h"
#include "matrix.h"
#include <armadillo>

using namespace std;
using namespace arma;

void matrix(mat& m,const mat& cs,const mat& Ras,const vec& Ka)
{
    double summation;
    vec ad(2);
    int lK=parameters::number_springs;

    for (int i=0; i<parameters::realmodes; i++){
        for (int j=0; j<parameters::realmodes; j++){
            if (cs(0,i)==cs(0,j) and cs(1,i)==cs(1,j)){
                summation=parameters::kappaLxLy*pow(dot(cs,i,i),2)+parameters::sigmaLxLy*dot(cs,i,i);
            } else {
                summation=0.0;
            }
            for (int a=0;a<lK;a++){
		addition(ad,cs(0,i),cs(1,i),cs(0,j),cs(1,j),-1);
                summation=summation+Ka(a)*cos(dot_el(ad(0),ad(1),Ras(0,a),Ras(1,a)));
            }
            m(i,j)=summation;
        }
    }

    cout << "sector (1,1) done" << endl;
    
    for (int i=0; i<parameters::realmodes;i++){
        for (int j=parameters::realmodes;j<parameters::realmodes+parameters::indqs;j++){
            summation=0.0;
            for (int a=0;a<lK;a++){
		addition(ad,cs(0,i),cs(1,i),cs(0,j),cs(1,j),1);
                summation=summation+sqrt(2.0)*Ka(a)*cos(dot_el(ad(0),ad(1),Ras(0,a),Ras(1,a)));
            }
            m(i,j)=summation;
        }
    }
   
    cout << "sector (1,2) done" << endl;

    for (int i=0; i<parameters::realmodes; i++){
        for (int j=parameters::realmodes+parameters::indqs;j<parameters::Msize;j++){
            summation=0.0;
            for (int a=0; a<lK; a++){
		addition(ad,cs(0,i),cs(1,i),cs(0,j),cs(1,j),1);
                summation=summation-sqrt(2.0)*Ka(a)*sin(dot_el(ad(0),ad(1),Ras(0,a),Ras(1,a)));
            }
            m(i,j)=summation;
        }
    }

    cout << "sector (1,3) done" << endl;
    

    for (int i=parameters::realmodes; i<parameters::realmodes+parameters::indqs; i++){
        for (int j=0;j<parameters::realmodes;j++){
            summation=0.0;
            for (int a=0;a<lK;a++){
		addition(ad,cs(0,i),cs(1,i),cs(0,j),cs(1,j),1);
                summation=summation+sqrt(2.0)*Ka(a)*cos(dot_el(ad(0),ad(1),Ras(0,a),Ras(1,a)));
            }
            m(i,j)=summation;
        }
    }

    cout << "sector (2,1) done" << endl;

    for (int i=parameters::realmodes;i<parameters::realmodes+parameters::indqs; i++){
        for (int j=parameters::realmodes;j<parameters::realmodes+parameters::indqs;j++){
            if (cs(0,i)==cs(0,j) and cs(1,i)==cs(1,j)){
                summation=parameters::kappaLxLy*pow(dot(cs,i,i),2)+parameters::sigmaLxLy*dot(cs,i,i);
            }else{
                summation=0.0;
            }
            for (int a=0;a<lK;a++){
                summation=summation+Ka(a)*2*(cos(dot_el(cs(0,i),cs(1,i),Ras(0,a),Ras(1,a)))*cos(dot_el(cs(0,j),cs(1,j),Ras(0,a),Ras(1,a))));
            }
            m(i,j)=summation;
        }
    }
    
    cout << "sector (2,2) done" << endl;

    for (int i=parameters::realmodes;i<parameters::realmodes+parameters::indqs;i++){
        for (int j=parameters::realmodes+parameters::indqs;j<parameters::Msize;j++){
            summation=0.0;
            for (int a=0;a<lK;a++){
                summation=summation-Ka(a)*2*(cos(dot_el(cs(0,i),cs(1,i),Ras(0,a),Ras(1,a)))*sin(dot_el(cs(0,j),cs(1,j),Ras(0,a),Ras(1,a))));
            }
            m(i,j)=summation; 
        }
    }

    cout << "sector (2,3) done" << endl;
    
    for (int i=parameters::realmodes+parameters::indqs;i<parameters::Msize;i++){
        for (int j=0;j<parameters::realmodes;j++){
            summation=0.0;
            for (int a=0;a<lK;a++){
		addition(ad,cs(0,i),cs(1,i),cs(0,j),cs(1,j),1);
                summation=summation-sqrt(2.0)*Ka(a)*sin(dot_el(ad(0),ad(1),Ras(0,a),Ras(1,a)));
            }
            m(i,j)=summation;
        }
    }

    cout << "sector (3,1) done" << endl;


    for (int i=parameters::realmodes+parameters::indqs; i<parameters::Msize; i++){
        for (int j=parameters::realmodes; j<parameters::realmodes+parameters::indqs; j++){
            summation=0.0;
            for (int a=0;a<lK;a++){
                summation=summation-Ka(a)*2*(sin(dot_el(cs(0,i),cs(1,i),Ras(0,a),Ras(1,a)))*cos(dot_el(cs(0,j),cs(1,j),Ras(0,a),Ras(1,a))));
            }
            m(i,j)=summation;
        }
    }

    cout << "sector (3,2) done" << endl;

    for (int i=parameters::realmodes+parameters::indqs; i<parameters::Msize; i++){
        for (int j=parameters::realmodes+parameters::indqs; j<parameters::Msize; j++){
            if (cs(0,i)==cs(0,j) and cs(1,i)==cs(1,j)){
                summation=parameters::kappaLxLy*pow(dot(cs,i,i),2)+parameters::sigmaLxLy*dot(cs,i,i);
            }else{
                summation=0.0;
            }
            for (int a=0;a<lK;a++){
                summation=summation+Ka(a)*2*(sin(dot_el(cs(0,i),cs(1,i),Ras(0,a),Ras(1,a)))*sin(dot_el(cs(0,j),cs(1,j),Ras(0,a),Ras(1,a))));
            }
            m(i,j)=summation;
        }
    }

    cout << "sector (3,3) done" << endl;

}


double dot(const mat& vectors,int first,int second){
	double d=vectors(0,first)*vectors(0,second)+vectors(1,first)*vectors(1,second);
	return d;
}

double dot_el(double v1x, double v1y, double v2x, double v2y){
	double d=v1x*v2x+v1y*v2y;
	return d;
}

void addition(vec& vect, double v1x, double v1y, double v2x, double v2y, int sign){
	vect(0)=v1x+sign*v2x;
	vect(1)=v1y+sign*v2y;
}




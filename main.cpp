//This is the main file that calculates the mean square amplitues of a membrane patch with the parameters specified in the 
//parameters file and a randomly chosen hexagonal spring configuration. The data is written into output files.

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>
#include "matrix.h"
#include "makemodesHexagonal.h"
#include "parameters.h"
#include <armadillo>
#define _USE_MATH_DEFINES 
#include <cmath>
#include <string>

using namespace std;
using namespace arma;
using namespace parameters;

int main(int argc, char* argv[])
{

	parameters::initParameters(argc,argv);
	parameters::writeToFile(parameters::paraFile);

	cout << "Matrix Size: " << parameters::Msize << endl;

///////////////////////////////////////////////////////////////////////
// Initialize spring positions and Fourier modes
	mat cs(2, parameters::Msize, fill::zeros);
	mat Ras(2, parameters::number_springs, fill::zeros);
	vec Ka(parameters::number_springs, fill::zeros);
	mat M(parameters::Msize, parameters::Msize, fill::zeros);

	makemodesHexagonal(cs, Ras, Ka);

	ofstream myfile (parameters::springFile.c_str());
	cout << "Writing springs to file" << endl;
	for(int i=0; i<parameters::number_springs; i++){
		myfile << std::setprecision(20) << Ras(0,i) << " " << Ras(1,i) << endl;
	}
	myfile.close();

///////////////////////////////////////////////////////////////////////
// Set up energy matrix, invert it and calculate the mean squared displacement in Fourier space
	matrix(M, cs, Ras, Ka);

	cout << "Inverting matrix" << endl;

	mat IM = inv(M);
	
	//real modes
	mat amps(parameters::Nx,parameters::Ny);
	amps(parameters::nx-1,parameters::ny-1)=parameters::kBT*(IM(0,0));   //The zero mode is in the middle: amps(n-1,n-1)
	amps(parameters::Nx-1,parameters::ny-1)=parameters::kBT*(IM(1,1));
	amps(parameters::nx-1,parameters::Ny-1)=parameters::kBT*(IM(2,2));    
	amps(parameters::Nx-1,parameters::Ny-1)=parameters::kBT*(IM(3,3));

	int cindex=1;
	//independent complex modes first quadrant
	for (int nx=0; nx<parameters::nx+1; nx++){
		for (int ny=0; ny<parameters::ny+1; ny++){
			if ((nx==0 and ny==0) or (nx==0 and ny==parameters::ny) or (nx==parameters::nx and ny==0) or (nx==parameters::nx and ny==parameters::ny)){}
			else{
				amps(parameters::nx+nx-1,parameters::ny+ny-1)=parameters::kBT/2.0*(IM(parameters::realmodes+cindex-1,parameters::realmodes+cindex-1)+IM(parameters::realmodes+parameters::indqs+cindex-1,parameters::realmodes+parameters::indqs+cindex-1));
				cindex++;
			}
		}
	}

	//independent complex modes second quadrant
	for (int nx=1; nx<parameters::nx; nx++){
	    for (int ny=1; ny<parameters::ny; ny++){
		amps(parameters::nx-nx-1,parameters::ny+ny-1)=parameters::kBT/2.0*(IM(parameters::realmodes+cindex-1,parameters::realmodes+cindex-1)+IM(parameters::realmodes+parameters::indqs+cindex-1,parameters::realmodes+parameters::indqs+cindex-1));
		cindex++;
	    }
	}

	//dependent modes
	for (int nxy=1; nxy<parameters::nx; nxy++){
	    amps(parameters::nx-nxy-1,parameters::ny-1)=amps(parameters::nx+nxy-1,parameters::ny-1);
	    amps(parameters::nx-nxy-1,parameters::Ny-1)=amps(parameters::nx+nxy-1,parameters::Ny-1);
	}
	for (int nxy=1; nxy<parameters::ny; nxy++){
	    amps(parameters::nx-1,parameters::ny-nxy-1)=amps(parameters::nx-1,parameters::ny+nxy-1);
	    amps(parameters::Nx-1,parameters::ny-nxy-1)=amps(parameters::Nx-1,parameters::ny+nxy-1); // was not here before					
	}
	for (int nx=1; nx<parameters::nx; nx++){
	    for (int ny=1; ny<parameters::ny; ny++){
		amps(parameters::nx+nx-1,parameters::ny-ny-1)=amps(parameters::nx-nx-1,parameters::ny+ny-1);
	    }
	}
	for (int nx=1; nx<parameters::nx; nx++){
	    for (int ny=1; ny<parameters::ny; ny++){
		amps(parameters::nx-nx-1,parameters::ny-ny-1)=amps(parameters::nx+nx-1,parameters::ny+ny-1);
	    }
	}

	//define Fourier modes
	vec kx(parameters::Nx);
	vec ky(parameters::Ny);
	for (int i=1; i<parameters::nx; i++){
	    kx(i-1)=-2*M_PI*(parameters::nx-i)/parameters::Lx; 
	    kx(parameters::nx+i-1)=2*M_PI*i/parameters::Lx; 
	}
	for (int i=1; i<parameters::ny; i++){
	    ky(i-1)=-2*M_PI*(parameters::ny-i)/parameters::Ly;
	    ky(parameters::ny+i-1)=2*M_PI*i/parameters::Ly;
	}
	kx(parameters::nx-1)=0;
	ky(parameters::ny-1)=0;
	kx(parameters::Nx-1)=2*M_PI*parameters::nx/parameters::Lx;
	ky(parameters::Ny-1)=2*M_PI*parameters::ny/parameters::Ly;


//////////////////////////////////////////////////////////////////////////	
//Convert mean square amplitudes to 1D plottable data
	
//extract diagonal values and plot as function of |k|
	int xoffset;
	int yoffset;
	if (parameters::Nx<parameters::Ny){
		xoffset=0;
		yoffset=(parameters::Ny-parameters::Nx)/2;
	}else if (parameters::Nx>parameters::Ny){
		xoffset=(parameters::Nx-parameters::Ny)/2;
		yoffset=0;
	}else{
		xoffset=0;
		yoffset=0;
	}

	cout << xoffset << " " << yoffset << endl;

	vec diagamps(min(parameters::Nx,parameters::Ny));
	vec kdiag(min(parameters::Nx,parameters::Ny));
	for (int i=0; i<min(parameters::Nx,parameters::Ny); i++){
	   diagamps(i)=amps(xoffset+i,yoffset+i);
	   if (i<min(parameters::nx,parameters::ny)-1){
	       kdiag(i)=-sqrt(pow(kx(xoffset+i),2)+pow(ky(yoffset+i),2));
	   }else if (i==min(parameters::nx,parameters::ny)-1){
	       kdiag(i)=0;
	   } else{
	       kdiag(i)=sqrt(pow(kx(xoffset+i),2)+pow(ky(yoffset+i),2));
	   }
	}
        
//Average over azimuthal angle as function of |k|

	//write all amps into one vector
	vec averageamps(parameters::Nx*parameters::Ny);
	vec ks(parameters::Nx*parameters::Ny);
	for (int x=0; x<parameters::Nx; x++){
	    for (int y=0; y<parameters::Ny; y++){
		averageamps(x*parameters::Ny+y)=amps(x,y);
		ks(x*parameters::Ny+y)=sqrt(pow(kx(x),2)+pow(ky(y),2));
	    } 
	}

	//sort average amps into bins
	vec sortedks= sort(ks);
	uvec indices = sort_index(ks);
	vec sortedamps(parameters::Nx*parameters::Ny);
	for (int i=0; i<parameters::Nx*parameters::Ny; i++){
		sortedamps(i)=averageamps(indices(i));
	}
	double kmin=min(sortedks);
	double kmax=max(sortedks)+1;
	double deltak=(kmax-kmin)/parameters::N;

	vec avks(parameters::N+1,fill::zeros);
	vec avamps(parameters::N+1,fill::zeros);
	avks(0)=0;
	avamps(0)=amps(parameters::nx-1,parameters::ny-1);
	vec sumamps(parameters::N,fill::zeros);	
	vec bbins(parameters::N,fill::zeros);
	int bbin;
	double temp;
	int bb;

	for (int i=1; i<parameters::Nx*parameters::Ny; i++){   
	
		bbin=floor((sortedks(i)-kmin)/deltak);
		bb=int(bbins(bbin))+1;
		bbins(bbin)=bb;
		temp=sumamps(bbin)+sortedamps(i);
		sumamps(bbin)=temp;	
	}

	for (int i=0; i<parameters::N; i++){	
	    avks(i+1)=kmin+(i+1/2)*deltak;
	    avamps(i+1)=sumamps(i)/bbins(i);
	}


///////////////////////////////////////////////////////////////////////
// Write data to file
	
	ofstream myfile5 (parameters::amplitudesFile.c_str());
	if (myfile5.is_open())
	{
	    cout << "Writing amplitudes to file" << endl;
	    for(int count1 = 0; count1 < parameters::Nx; count1 ++){
		    for(int count2 = 0; count2 < parameters::Ny; count2 ++){
			myfile5 << std::setprecision(20) << amps(count1,count2) << " " ;
		    }
		myfile5 << endl;
	  }
	    myfile5.close();
	}
	else cout << "Unable to open file" << endl;

	ofstream myfile1 (parameters::ampsFile.c_str());
	ofstream myfile2 (parameters::diagampsFile.c_str());
	ofstream myfile3 (parameters::avampsFile.c_str());
	if (myfile1.is_open() and myfile2.is_open() and myfile3.is_open())
	{
	    cout << "Writing data to file" << endl;
	    for(int count = parameters::nx-1; count < parameters::Nx; count ++){
		myfile1 << std::setprecision(20) << kx(count) << " " << amps(count,parameters::nx-1);
  		myfile1 << endl;
	    }
	    for(int count = 0; count < min(parameters::Nx,parameters::Ny); count ++){
		myfile2 << std::setprecision(20) << kdiag(count) << " " << diagamps(count);    
		myfile2 << endl;
	    }
	    myfile1.close();
	    myfile2.close();
	    for(int count = 0; count < parameters::N+1; count ++){
		myfile3 << std::setprecision(20) << avks(count) << " " << avamps(count);
		myfile3 << endl;
	    }
	    myfile3.close();
	}
	else cout << "Unable to open file" << endl;

return 0;
}




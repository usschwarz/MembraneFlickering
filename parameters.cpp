

#include <math.h> 
#include "parameters.h"
#include "ParaHandler.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <sys/stat.h>

using namespace std;

namespace parameters
{

	double kBT=1.38064852*pow(10,-23);
	int realmodes= 4;

	double lengthunit=0;
	int n=0;
	int nx=0;
	int ny=0;
	int Nx=0;
	int Ny=0;
	int N=0;
	double Lx=0;
	double Ly=0;
	int indqs=0;
	int allindqs=0;
	int Msize=0;
	double a=0; 	
	int xn=0; 
	int yn=0; 
	double h=0;
	double c=0;
	bool midpoint=true;
	double k1=0;
	double k2=0;
	double kknob=0;
	int lattice=0;
	double delta=0;
	int number_springs=0;  
	double kappaLxLy=0;
	double sigmaLxLy=0;
	int knobs=0;
	int number_midpoints=0;
	double knobdensity=0;
	double meshsize=0;
	string runfolder=" ";
	string springFile=" ";
	string paraFile=" ";
	string amplitudesFile=" ";
	string ampsFile=" ";
	string diagampsFile=" ";
	string avampsFile=" ";



	void initParameters(int argc, char* argv[])
	{

		//Set parameters from file
		ParaHandler para_handler(argc, argv);

		parameters::n=para_handler.ReturnIntBehind("n");
		parameters::lengthunit=para_handler.ReturnDoubleBehind("lengthunit")*pow(10,-9);
		parameters::a=para_handler.ReturnDoubleBehind("a")*pow(10,-9);
		parameters::xn=para_handler.ReturnIntBehind("xn");
		parameters::yn=para_handler.ReturnIntBehind("yn");
		//parameters::midpoint=para_handler.ReturnBoolBehind("midpoint");
		parameters::k1=para_handler.ReturnDoubleBehind("K1");
		parameters::k2=para_handler.ReturnDoubleBehind("K2");
		parameters::kknob=para_handler.ReturnDoubleBehind("Kknob");
		parameters::lattice=para_handler.ReturnIntBehind("lattice");
		parameters::runfolder=para_handler.ReturnStringBehind("runfolder");
		parameters::springFile=parameters::runfolder+para_handler.ReturnStringBehind("springFile");
		parameters::paraFile=parameters::runfolder+para_handler.ReturnStringBehind("paraFile");
		parameters::amplitudesFile=parameters::runfolder+para_handler.ReturnStringBehind("amplitudesFile");
		parameters::ampsFile=parameters::runfolder+para_handler.ReturnStringBehind("ampsFile");
		parameters::avampsFile=parameters::runfolder+para_handler.ReturnStringBehind("avampsFile");
		parameters::diagampsFile=parameters::runfolder+para_handler.ReturnStringBehind("diagampsFile");

		double kappa=para_handler.ReturnDoubleBehind("kappa")*pow(10,-19);
		double sigma=para_handler.ReturnDoubleBehind("sigma")*pow(10,-7);
		parameters::knobdensity=para_handler.ReturnDoubleBehind("knobdensity")*pow(10,12);
		parameters::meshsize=0;//para_handler.ReturnDoubleBehind("meshsize")*pow(10,-12);
		parameters::number_midpoints=para_handler.ReturnIntBehind("midpoints");

		parameters::h=sqrt(3)*parameters::a;
		parameters::c=parameters::a*0.5;

		parameters::nx=(int)ceil(parameters::xn*3*parameters::a/parameters::lengthunit/2.0);
		parameters::ny=(int)ceil(parameters::yn*parameters::h/parameters::lengthunit/2.0);

		//Calculate remaining variables
		parameters::N= 2*parameters::n;
		parameters::Nx= 2*parameters::nx;
		parameters::Ny= 2*parameters::ny;
		parameters::Lx=parameters::Nx*parameters::lengthunit;
		parameters::Ly=parameters::Ny*parameters::lengthunit;
		parameters::indqs= int(4*parameters::nx*parameters::ny/2.0-2);
		parameters::allindqs= parameters::indqs+parameters::realmodes;
		parameters::Msize=parameters::realmodes+2*parameters::indqs;
		parameters::delta=parameters::Lx/parameters::lattice;  
		parameters::kappaLxLy=kappa*parameters::Lx*parameters::Ly;
		parameters::sigmaLxLy=sigma*parameters::Lx*parameters::Ly;
		parameters::knobs=(int)round(knobdensity*parameters::Lx*parameters::Ly);
		int hexagons=parameters::xn*parameters::yn*2;
		if(parameters::number_midpoints<0){
			parameters::number_midpoints=0;
		}else if (parameters::number_midpoints>hexagons){
			parameters::number_midpoints=hexagons;
			printf("Mesh size too small!");
		}
		parameters::number_springs=4*parameters::xn*parameters::yn+parameters::number_midpoints+parameters::knobs*49;

		const int dir_err = mkdir(parameters::runfolder.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		if (-1 == dir_err)
		{
		    printf("Error creating directory!n");
		}

	}

	void writeToFile(string fileName)
	{

		ofstream myfile (fileName.c_str());
		if (myfile.is_open())
		{
			myfile << "knobs " << std::setprecision(4) << parameters::knobs << endl;
			myfile << "knob density " << std::setprecision(4) << parameters::knobdensity << endl;
			myfile << "lengthunit " << std::setprecision(4) << parameters::lengthunit << endl;
			myfile << "kBT " << std::setprecision(4) << parameters::kBT << endl;
			myfile << "N " << std::setprecision(4) << 2*parameters::n << endl;
			myfile << "Nx " << std::setprecision(4) << 2*parameters::nx << endl;
			myfile << "Ny " << std::setprecision(4) << 2*parameters::ny << endl;
			myfile << "Lx " << std::setprecision(4) << parameters::Lx << endl;
			myfile << "Ly " << std::setprecision(4) << parameters::Ly << endl;
			myfile << "area " << std::setprecision(4) << parameters::Lx*parameters::Ly << endl;
			myfile << "indqs " << std::setprecision(4) << parameters::indqs << endl;
			myfile << "realmodes " << std::setprecision(4) << parameters::realmodes << endl;
			myfile << "allindqs " << std::setprecision(4) << parameters::allindqs << endl;
			myfile << "Msize " << std::setprecision(4) << parameters::Msize << endl;
			myfile << "a " << std::setprecision(4) << parameters::a << endl;
			myfile << "xn " << std::setprecision(4) << parameters::xn << endl;
			myfile << "yn " << std::setprecision(4) << parameters::yn << endl;
			myfile << "h " << std::setprecision(4) << parameters::h << endl;
			myfile << "c " << std::setprecision(4) << parameters::c << endl;
			myfile << "meshsize " << std::setprecision(4) << parameters::meshsize << endl;
			myfile << "midpoints " << std::setprecision(4) << parameters::number_midpoints << endl;
			myfile << "K1 " << std::setprecision(4) << parameters::k1 << endl;
			myfile << "K2 " << std::setprecision(4) << parameters::k2 << endl;
			myfile << "Kknob " << std::setprecision(4) << parameters::kknob << endl;
			myfile << "lattice " << std::setprecision(4) << parameters::lattice << endl;
			myfile << "delta " << std::setprecision(4) << parameters::delta << endl;
			myfile << "number_springs " << std::setprecision(4) << parameters::number_springs << endl;
			myfile << "kappaLxLy " << std::setprecision(4) << parameters::kappaLxLy << endl;
			myfile << "kappa " << std::setprecision(4) << parameters::kappaLxLy/parameters::Lx/parameters::Ly << endl;
			myfile << "sigmaLxLy " << std::setprecision(4) << parameters::sigmaLxLy << endl;
			myfile << "sigma " << std::setprecision(4) << parameters::sigmaLxLy/parameters::Lx/parameters::Ly << endl;
			myfile.close();
		}
	}
}




























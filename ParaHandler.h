/* 
 * File:   ParaHandler.h
 * Author: bq_palbert
 *
 * Created on 30 June 2012, 14:07
 *
 * Parses parameters defined in a file and stores them.
 * Arguments given to the program override existing parameters
 *
 * If additional shell parameters are used, the parameter file must be the
 * first argument to the program.
 * If a request for an undefined parameter is found the program terminates.
 *
 * Comments in the parameter file start with #
 * A parameter consists of a name and a value, both are stored as strings
 */

#ifndef PARAMETERHANDLER_H
#define	PARAMETERHANDLER_H


#include <iostream>
#include <map>
#include <sstream>
#include <vector>

using namespace std;

#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

const int MAX_LENGHT_OF_PARAMETER=1024;     
const char *const DEFAULT_PARAMETER_FILE="ParameterFiles/Default.dat";


class ParaHandler {
public:
    
    
    ParaHandler();
    ParaHandler(int argc, char* argv[]);
    void HandleInputArguments(int argc, char* argv[]);
    bool SetParameter(const char* parameter_name,bool &b_parameter);
    bool SetParameter(const char* parameter_name,int  &int_parameter);
    bool SetParameter(const char* parameter_name,double &double_parameter);
    bool SetParameter(const char* parameter_name,string &s_parameter);
    
    
    string ReturnStringBehind(const char* parameter_name);
    int ReturnIntBehind(const char* parameter_name);
    double ReturnDoubleBehind(const char* parameter_name);
    bool ReturnBoolBehind(const char* parameter_name);
    bool ReturnBoolIfExists(const char* parameter_name,
                            bool default_if_not_ex=false);
    vector <double> ReturnDoubleListBehind(const char* parameter_name);

    bool CheckIfExists(string parameter_name,bool strict=false);
    bool CheckIfExists(const char* parameter_name,bool strict=false);
    
    ~ParaHandler();
    
    /**
     * Adds a parameter with \a name and \a value
     * @param name
     * @param value
     */
    template <class T> void AddParameter(string name,T value)
    {
        s_parameter new_parameter;
        new_parameter.name=name;
        new_parameter.value=SSTR(value);
        new_parameter.used=false;
        AddParameter(new_parameter);
    }
   
 
    
    /**
     * Puts the value of \a parameter_name into \a parameter if \a parameter_name exists.
     * If \a output a message is printed. 
     * @return true if value set, otherwise false
     */
    template<class PARAMETER>
    bool SetParameterIfExist(const char* parameter_name,
                               PARAMETER &parameter,
                               bool output=false)
    {
        if(CheckIfExists(parameter_name))
        {
            SetParameter(parameter_name,parameter);
            if(b_quiet==false && output)
            {
                cout<<"SetParameterIfExist::parameter '"
                    <<parameter_name<<"' exist"<<endl;
                cout<<"value set to: "<<parameter<<endl;
            }
            return true;
        }
        else
        {
            if(b_quiet==false && output)
            {
                cout<<"SetParameterIfExist:: parameter '"
                    <<parameter_name<<"' does not exist"<<endl;
                cout<<"Use preset value: "<<parameter<<endl;
            }
            return false;
        }
    }
    
private:
    
    
    struct s_parameter
    {
       string name;   // name / key of the parameter
       string value;  // stored as string an converted later
       bool used;     // flag to shop if used
    };
    map <string, s_parameter> m_parameters; // holds the parameters
   
    void AddParameter(s_parameter &new_parameter);
    bool ParseString(string input,s_parameter &new_parameter);
    bool b_quiet; //flag to print messages
    

};

#endif	/* PARAMETERHANDLER_H */


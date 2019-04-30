
/* 
 * File:   ParaHandler.cpp
 * Author: bq_palbert
 *
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

#include "ParaHandler.h"
#include <fstream>
#include <string.h>
#include <iomanip>
#include <vector>
#include <stdlib.h>


ParaHandler::ParaHandler()
{
  
}


ParaHandler::ParaHandler(int argc , char* argv[])
{

  cout<<"----------parsing parameter file and arguments--------------"<<endl;
  ifstream DataFile;
  string file_name;
  if ( argc>=2 ) // is there more than one argument
  {
    DataFile.open(argv[1]);     //the first one needs to be the parameter file, try to open it.
    if ( !DataFile )
    {
      cout<<"Could not open parameter file '"<<argv[1]<<"'"<<endl;
      exit(1);
    }
    file_name = argv[1];
  }
  else
  {
    DataFile.open(DEFAULT_PARAMETER_FILE);
    if ( !DataFile )
    {

      cout<<"Could not open default parameter file '"<<DEFAULT_PARAMETER_FILE<<"'"<<endl;
      exit(1);
    }
    else
    {
      cout<<"Using default parameter file"<<endl;
      file_name = DEFAULT_PARAMETER_FILE;
    }
  }


  cout<<"parameter file: "<<file_name<<endl;
  cout<<"executable: "<<argv[0]<<endl;
  cout<<"number of additional arguments="<<argc-2<<endl;


  string line_in_file;
  s_parameter new_parameter;
  while (getline(DataFile , line_in_file))       //until end of file is reached
  {
    if ( ParseString(line_in_file , new_parameter) )
    {
      AddParameter(new_parameter);
    }
  }
  HandleInputArguments(argc , argv);

  DataFile.close();
  cout<<"----------parsing finished----------------------------------"<<endl;
}


ParaHandler::~ParaHandler() { }


/**
 * Adds \a new_parameter
 * @param new_parameter
 */
void ParaHandler::AddParameter(s_parameter &new_parameter)
{

  {
    if ( CheckIfExists(new_parameter.name) && b_quiet==false )
    {
      cout << "override " << m_parameters[new_parameter.name].name
        << "=" << m_parameters[new_parameter.name].value << endl;
      cout << "by       " << new_parameter.name
        << "=" << new_parameter.value << endl;
    }
    m_parameters[new_parameter.name] = new_parameter;
  }
}


/**
 * Parses \a input (usually a line in a file) and puts the retrieved parameter 
 * into \a new_parameter. If it fails to retrieve a parameter or the string is a 
 * comment started by # it returns false.
 * @param input
 * @param new_parameter
 * @return 
 */
bool ParaHandler::ParseString(string input , s_parameter &new_parameter)
{
  size_t found_at;

  if ( input[0]=='#'  || input.empty() )  //check if comment or empty
  {
    //nothing
    return false;
  }
  //truncating string after #
  found_at = input.find("#");
  if ( found_at!=string::npos )
  {
    input = input.substr(0 , found_at);
  }

  if ( input.find("=")==string::npos )   //check if there is an equal sign
  {
    //there is no equal sign
    //cout<<"Ignoring the following line in parameter file"<<filename<<endl;
    //cout<<"'"<<input<<"'"<<endl;
    return false;
  }
  else
  {
    found_at = input.find("=");
    if ( found_at==input.length()-1 )
    {
      cout<<"parameter '"<<input<<"' has no value"<<endl;
      return false;
    }
    else
    {
      new_parameter.name = input.substr(0 , found_at);
      new_parameter.value = input.substr(found_at+1 , string::npos);

      //remove trailing and leading whitespaces or tabs from both strings
      size_t found_at_end;
      found_at = new_parameter.name.find_first_not_of(" \t");
      found_at_end = new_parameter.name.find_last_not_of(" \t");
      new_parameter.name = new_parameter.name.substr(found_at , found_at_end-found_at+1);

      found_at = new_parameter.value.find_first_not_of(" \t'\";");
      found_at_end = new_parameter.value.find_last_not_of(" \t'\";");
      new_parameter.value = new_parameter.value.substr(found_at , found_at_end-found_at+1);


      new_parameter.used = false;
      return true;
    }

  }
}


/**
 * Parses the input arguments given to the program
 * @param argc
 * @param argv
 */
void ParaHandler::HandleInputArguments(int argc , char* argv[])
{

  for (int i = 2; i < argc; i++)
  {
    s_parameter new_parameter;
    if ( ParseString(argv[i] , new_parameter) )
    {
      AddParameter(new_parameter);
    }
  }

}



/**
 * Returns true if a parameter with key @parameter_name exists. If \a strict the
 * program exits and the paramete name is copied to the clipboard
 */
bool ParaHandler::CheckIfExists(string parameter_name , bool strict)
{
  bool exists = m_parameters.find( parameter_name ) != m_parameters.end();
  if ( strict && !exists)
  {
    cout<<"Parameter with name '"<<parameter_name<<"' not found"<<endl;
    exit(1);
  }
  return exists;
}


bool ParaHandler::CheckIfExists(const char* name , bool strict)
{
  string to_be_checked(name);
  return CheckIfExists(to_be_checked,strict);
}


/**
 * Puts the value of \a name into \a int_parameter. Returns true if a parameter
 * \a name exists, otherwise false.
 * @param name
 * @param int_parameter
 * @return 
 */
bool ParaHandler::SetParameter(const char* name , int& int_parameter)
{
  if ( CheckIfExists(name,true) )
  {
    int_parameter = atoi(m_parameters[name].value.c_str());
    m_parameters[name].used = true;

    return true;
  }
  return false;
}


/**
 * Returns the value of parameter \a name as integer
 * @param name
 * @return 
 */
int ParaHandler::ReturnIntBehind(const char* name)
{
  int to_be_returned = 0;
  SetParameter(name , to_be_returned);
  return to_be_returned;
}


/**
 * Same as other SetValueBehin() function
 * @param name
 * @param double_parameter
 * @return 
 */
bool ParaHandler::SetParameter(const char* name , double& double_parameter)
{
  if ( CheckIfExists(name,true) )
  {
    double_parameter = atof(m_parameters[name].value.c_str());
    m_parameters[name].used = true;

    return true;
  }
  return false;
}

//see above


double ParaHandler::ReturnDoubleBehind(const char* name)
{
  double to_be_returned = 0;
  SetParameter(name , to_be_returned);
  return to_be_returned;
}

//see above


vector <double> ParaHandler::ReturnDoubleListBehind(const char* parameter_name)
{
  vector <double> par_list;
  string list;
  SetParameter(parameter_name , list);
  istringstream list_stream(list);
  double value;
  while (list_stream>>value)
  {
    par_list.push_back(value);
  }
  return par_list;

}

//see above


bool ParaHandler::SetParameter(const char* name , bool& b_parameter)
{
  if ( CheckIfExists(name,true) )
  {
    if (     m_parameters[name].value.find("true")!=string::npos
         || m_parameters[name].value.find("True")!=string::npos
         || atoi(m_parameters[name].value.c_str())==1 )
    {
      b_parameter = true;
      m_parameters[name].used = true;
      return true;
    }
    else if ( m_parameters[name].value.find("false")!=string::npos
              || m_parameters[name].value.find("False")!=string::npos
              || atoi(m_parameters[name].value.c_str())==0 )
    {
      b_parameter = false;
      m_parameters[name].used = true;
      return true;
    }
    else
    {
      cout<<"Cannot set boolean parameter '"<<m_parameters[name].name<<"'"<<endl;
      cout<<"the value '"<<m_parameters[name].value<<"' is not boolean"<<endl;
      cout<<"'"<<m_parameters[name].name<<"' is set to false"<<endl;
      b_parameter = false;
      m_parameters[name].used = true;
      return false;
    }


  }
  return false;
}


//see above


bool ParaHandler::ReturnBoolBehind(const char* name)
{
  bool to_be_returned = false;
  SetParameter(name , to_be_returned);
  return to_be_returned;
}


/**
 * Not as strict as other Return functions. Returns the value of @parameter_name
 * if its exist. Otherwise \a default_if_not_ex is returned
 * @param parameter_name
 * @param default_if_not_ex
 * @return 
 */
bool ParaHandler::ReturnBoolIfExists(const char* parameter_name ,
                                     bool default_if_not_ex )
{
  if ( CheckIfExists(parameter_name) )
  {
    bool value = ReturnBoolBehind(parameter_name);
    if ( b_quiet==false )
    {
      printf("set %s to %d\n" , parameter_name , value);
    }
    return value;
  }
  else
  {
    return default_if_not_ex;
  }

}

//see above


bool ParaHandler::SetParameter(const char* name , string &s_parameter)
{
  if ( CheckIfExists(name,true) )
  {
    int found_first = m_parameters[name].value.find_first_of("'\"");
    int found_last = m_parameters[name].value.find_last_of("'\"");
    if ( found_first!=string::npos && found_last!=string::npos )
    {
      s_parameter = m_parameters[name].value.substr(found_first+1 , found_last-found_first-1);
    }
    else
    {
      s_parameter = m_parameters[name].value;
      if ( m_parameters[name].value.find(" ")!=string::npos )
      {
        cout<<"Warning: The string '"<<s_parameter<<"' contains white spaces."<<endl;
        cout<<"It should be put in single quotes for safety"<<endl;
      }
    }
    m_parameters[name].used = true;
    return true;

  }
  return false;
}



//see above


string ParaHandler::ReturnStringBehind(const char* name)
{
  string to_be_returned;
  SetParameter(name , to_be_returned);
  return to_be_returned;
}



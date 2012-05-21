/*! \if exist This is -*- C++ -*- from nbg \endif

    \file  InstanceReader.cc

    \par Last Author: Martin Loesch (<loesch@@ira.uka.de>)
    \par Date of last change: 11.04.10

    \author   Martin Loesch (<loesch@@ira.uka.de>)
    \date     2010-04-08
    \par Copyright:
              Martin Loesch, Chair Prof. Dillmann (IAIM)\n
              Institute for Computer Science and Engineering (CSE)\n
	      University of Karlsruhe. All rights reserved\n
	      http://wwwiaim.ira.uka.de
*/

/* system includes */
#include <assert.h>
#include <iostream>
#include <fstream>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/trim.hpp>

/* my includes */
#include "InstanceReader.h"

using namespace std;


NBL::InstanceReader::InstanceReader(string filename)
{
  _filename = filename;
  _isValid = false;
  
  loadFile();
}

NBL::InstanceReader::~InstanceReader()
{
}

void NBL::InstanceReader::loadFile()
{
  ifstream file(_filename.c_str());

  if (!file.is_open()){ return; }
  
  // read file header: node descriptions
  int lineCount=0;
  char typeLine[256];
  char nameLine[256];
  char valNumberLine[256];
  while (!file.eof() && lineCount<3){

    if (lineCount==0){
      file.getline(typeLine, 256);
      if (typeLine[0]=='#'){ continue; }
      lineCount++;
    } else if (lineCount==1){
      file.getline(nameLine, 256);
      if (nameLine[0]=='#'){ continue; }
      lineCount++;
    } else if (lineCount==2){
      file.getline(valNumberLine, 256);
      if (valNumberLine[0]=='#'){ continue; }
      lineCount++;
    }
  }
    
  parseNodeDescriptions(typeLine, nameLine, valNumberLine);

  // read file main part: instances
  char line[256];
  vector<string> parts;
  while (!file.eof()){
    file.getline(line, 256);
    boost::algorithm::split(parts, line, boost::algorithm::is_any_of(","));

    Instance temp;
    temp.m_rootValue = boost::algorithm::trim_copy(parts[0]);
    if (_rootNodeDescription.type==DISCRETE){
      _rootNodeDescription.discreteValues.insert(temp.m_rootValue);
    }

    for (unsigned int i=1; i<parts.size(); i++){
      temp.m_attribValues.push_back(boost::algorithm::trim_copy(parts[i]));
    }
    if (temp.m_attribValues.size()==_endnodeDescriptions.size()){
      for (unsigned int i=0; i<temp.m_attribValues.size(); i++){
	_endnodeDescriptions[i].discreteValues.insert(temp.m_attribValues[i]);
      }
      _instances.push_back(temp);
    }
  }
  
  _isValid = true;
}

void NBL::InstanceReader::parseNodeDescriptions(char* typeLine, char* nameLine, char* valNumberLine)
{
  vector<string> typeParts, nameParts, valNumberParts;
  boost::algorithm::split(typeParts, typeLine, boost::algorithm::is_any_of(","));
  boost::algorithm::split(nameParts, nameLine, boost::algorithm::is_any_of(","));
  boost::algorithm::split(valNumberParts, valNumberLine, boost::algorithm::is_any_of(","));

  _rootNodeDescription.type = parseTypeCode(boost::algorithm::trim_copy(typeParts[0]));
  _rootNodeDescription.name = boost::algorithm::trim_copy(nameParts[0]);
  _rootNodeDescription.numberOfValues = atoi(boost::algorithm::trim_copy(valNumberParts[0]).c_str());
  
  NodeDescription temp;
  for (unsigned i=1; i<typeParts.size(); i++){
    temp.type = parseTypeCode(boost::algorithm::trim_copy(typeParts[i]));
    temp.name = boost::algorithm::trim_copy(nameParts[i]);
    temp.numberOfValues = atoi(boost::algorithm::trim_copy(valNumberParts[i]).c_str());

    _endnodeDescriptions.push_back(temp);
  }
}

NBL::NodeValueType NBL::InstanceReader::parseTypeCode(string code)
{
  if (code=="b"){
    return BOOLEAN;
  } else if (code=="d"){
    return DISCRETE;
  } else {
    return NONODETYPE;
  }
}

NBL::NodeDescription NBL::InstanceReader::getRootNodeDescription() const
{
  return _rootNodeDescription;
}

vector<NBL::NodeDescription> NBL::InstanceReader::getEndNodeDescriptions() const
{
  return _endnodeDescriptions;
}

vector<NBL::Instance>* NBL::InstanceReader::getInstances()
{
  return &_instances;
}

bool NBL::InstanceReader::isValid() const
{
  return _isValid;
}


#if InstanceReader_test
#include <stdio.h>
int main(int argc, char **argv)
{
  // This is a module-test block. You can put code here that tests
  // just the contents of this C file, and build it by saying
  //             make InstanceReader_test
  // Then, run the resulting executable (InstanceReader_test).
  // If it works as expected, the module is probably correct. ;-)

  fprintf(stderr, "Testing InstanceReader\n");

  return 0;
}
#endif /* InstanceReader_test */

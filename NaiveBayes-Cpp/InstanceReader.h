/*! \if exist This is -*- C++ -*- from nbg \endif

    \file  InstanceReader.h
    \brief 

    Please put your documentation for this file here.

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

#ifndef INSTANCEREADER_H
#define INSTANCEREADER_H

/* system includes */
#include <string>
#include <set>

/* my includes */
#include "Instance.h"
#include "Nodes.h"


namespace NBL {

  typedef struct {
    NodeValueType type;
    string name;
    int numberOfValues;
    set<string> discreteValues;
  } NodeDescription;
  
  /*!
    \class InstanceReader
    \brief
    
    
  */
  class InstanceReader {
  private:
    string _filename;

    NodeDescription _rootNodeDescription;

    vector<NodeDescription> _endnodeDescriptions;

    vector<Instance> _instances;

    bool _isValid;
    
    void loadFile();
    void parseNodeDescriptions(char* typeLine, char* nameLine, char* valNumberLine);
    NodeValueType parseTypeCode(string code);
    
  protected:
    
  public:
    InstanceReader(string filename);
    ~InstanceReader();

    NodeDescription getRootNodeDescription() const;
    vector<NodeDescription> getEndNodeDescriptions() const;
    vector<Instance>* getInstances();

    bool isValid() const;
  };

};

#endif /* INSTANCEREADER_H */

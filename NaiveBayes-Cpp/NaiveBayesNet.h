/*! \if exist This is -*- C++ -*- from nbg \endif

    \file  NaiveBayesNet.h
    \brief 

    Please put your documentation for this file here.

    \par Last Author: Martin Loesch (<martin.loesch@@kit.edu>)
    \par Date of last change: 21.05.12

    \author   Martin Loesch (<loesch@@ira.uka.de>)
    \date     2010-04-08
    \par Copyright:
               Martin Loesch, Chair Prof. Dillmann (IAIM)\n
               Institute for Computer Science and Engineering (CSE)\n
	       University of Karlsruhe. All rights reserved\n
	       http://wwwiaim.ira.uka.de
*/

#ifndef NAIVEBAYESNET_H
#define NAIVEBAYESNET_H

/* system includes */
#include <string>
#include <map>
#include <vector>

/* my includes */
#include "Nodes.h"
#include "Instance.h"
#include "InstanceReader.h"

using namespace std;


namespace NBL {

  typedef pair<string, EndNode* > NodeListEntry;
  typedef map<string, EndNode* > NodeList;
  
  /*!
    \class NaiveBayesNet
    \brief Representation for a simple naive bayesian net.
    
    
  */
  class NaiveBayesNet {
  private:
    RootNode _root;
    NodeList _endnodeList;

    bool _isValid;

    bool addEndNode(string name, NodeValueType type, unsigned int numberOfValues=2);
    void learnRootProbabilities(vector<Instance>& trainData);
    void learnEndnodeProbabilities(vector<Instance>& trainData, vector<string> nodeOrder);
    bool constructNetFromReader(InstanceReader& reader);
    
  protected:
    
  public:
    NaiveBayesNet();
    ~NaiveBayesNet();

    //@{
    //! \name Construction methods
    void initRootBool(string rootName);
    void initRootDiscrete(string rootName, unsigned int numberOfValues);

    bool addBoolNode(string name);
    bool addDiscreteNode(string name, unsigned int numberOfValues);

    RootNode* getRootNode();
    EndNode* getNodeByName(string name);
    //@}

    //@{
    //! \name Setting evidence methods
    void setNodeValue(string name, bool value);
    void setNodeValue(string name, int value);
    void setNodeValue(string name, string value);
    //@}

    //@{
    //! \name Result calculation methods
    void calcClassificationResult(bool& res);
    void calcClassificationResult(int& res);

    void calcResultDistribution(float* res, unsigned int length);
    //@}
    
    //@{
    //! \name Learning methods
    void learnParameters(vector<Instance>& trainData, vector<string> nodeOrder);
    bool learnFromFile(string filename);
    //@}
  };

};

#endif /* NAIVEBAYESNET_H */

/*! \if exist This is -*- C++ -*- from nbg \endif

    \file  Nodes.cc

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

/* system includes */
#include <assert.h>
#include <boost/algorithm/string.hpp>

/* my includes */
#include "Nodes.h"


bool NBL::stringToBool(string text)
{
  boost::to_lower(text);
  
  if (text=="ja" || text=="yes"){
    return true;
  } else {
    return false;
  }
}


// ***** Node stuff ************************************************************
NBL::Node::Node(string name)
{
  _name = name;
  m_type = NONODETYPE;
  m_numberOfDiscreteValues = 0;
}

NBL::Node::~Node()
{
}

void NBL::Node::setName(string name)
{
  _name = name;
}

string NBL::Node::getName() const
{
  return _name;
}

NBL::NodeValueType NBL::Node::getType() const
{
  return m_type;
}

unsigned int NBL::Node::getNumberOfDiscreteValues() const
{
  return m_numberOfDiscreteValues;
}

bool NBL::Node::addDiscreteValue(string value)
{
  if (m_type!=DISCRETE || _translTableN2I.size()==m_numberOfDiscreteValues){ return false; }

  if (_translTableN2I.find(value)!=_translTableN2I.end()){ return false; }
  
  int newIndex = _translTableN2I.size();
  _translTableN2I[value] = newIndex;
  _translTableI2N[newIndex] = value;

  return true;
}

int NBL::Node::translate(string value)
{
  int res;
  
  if (_translTableN2I.find(value)==_translTableN2I.end()){
    res = -1;
  } else {
    res = _translTableN2I[value];
  }

  return res;
}

string NBL::Node::translate(int value)
{
  string res;
  
  if (value>=static_cast<int>(_translTableI2N.size())){
    res = "";
  } else {
    res = _translTableI2N[value];
  }

  return res;
}

void NBL::Node::initBoolNode()
{
  m_type = BOOLEAN;
  m_numberOfDiscreteValues = 2;
}

void NBL::Node::initIntegerNode(int begin, int end)
{
  m_type = INTEGER;
  m_numberOfDiscreteValues = end - begin + 1;

  m_intervalBegin = begin;
  m_intervalEnd = end;
}

void NBL::Node::initDiscreteNode(unsigned int numberOfValues)
{
  m_type = DISCRETE;
  m_numberOfDiscreteValues = numberOfValues;
}

// ***** RootNode stuff ********************************************************
NBL::RootNode::RootNode(string name) : Node(name)
{
  _relProbabilities = NULL;
}

NBL::RootNode::~RootNode()
{
  if (_relProbabilities){
    delete[] _relProbabilities;
  }
}

void NBL::RootNode::initBoolNode()
{
  Node::initBoolNode();

  _relProbabilities = new float[m_numberOfDiscreteValues];
}

void NBL::RootNode::initIntegerNode(int begin, int end)
{
  Node::initIntegerNode(begin, end);

  _relProbabilities = new float[m_numberOfDiscreteValues];
}

void NBL::RootNode::initDiscreteNode(unsigned int numberOfValues)
{
  Node::initDiscreteNode(numberOfValues);
  
  _relProbabilities = new float[m_numberOfDiscreteValues];
}

void NBL::RootNode::setProbabilities(float* probabilities, unsigned int length)
{
  if (length!=m_numberOfDiscreteValues){ return; }

  for (unsigned int i=0; i<m_numberOfDiscreteValues; i++){
    _relProbabilities[i] = probabilities[i];
  }
}

float NBL::RootNode::getProbability(int v)
{
  return _relProbabilities[v];
}



// ***** EndNode stuff *********************************************************
NBL::EndNode::EndNode(NBL::RootNode* root, string name) : Node(name)
{
  _root = root;
  
  _condProbabilities = NULL;
}

NBL::EndNode::~EndNode()
{
  if (_condProbabilities){
    for (unsigned int i=0; i<m_numberOfDiscreteValues; i++){
      delete[] _condProbabilities[i];
    }
    delete[] _condProbabilities;
  }
}

void NBL::EndNode::initBoolNode()
{
  Node::initBoolNode();

  _condProbabilities = new float*[m_numberOfDiscreteValues];
  for (unsigned int i=0; i<m_numberOfDiscreteValues; i++){
    _condProbabilities[i] = new float[_root->getNumberOfDiscreteValues()];
  }
}

void NBL::EndNode::initIntegerNode(int begin, int end)
{
  Node::initIntegerNode(begin, end);

  _condProbabilities = new float*[m_numberOfDiscreteValues];
  for (unsigned int i=0; i<m_numberOfDiscreteValues; i++){
    _condProbabilities[i] = new float[_root->getNumberOfDiscreteValues()];
  }
}

void NBL::EndNode::initDiscreteNode(unsigned int numberOfValues)
{
  Node::initDiscreteNode(numberOfValues);

  _condProbabilities = new float*[m_numberOfDiscreteValues];
  for (unsigned int i=0; i<m_numberOfDiscreteValues; i++){
    _condProbabilities[i] = new float[_root->getNumberOfDiscreteValues()];
  }
}

void NBL::EndNode::setCondProbabilities(float* probabilities, unsigned int rows, unsigned int columns)
{
  if (rows!=m_numberOfDiscreteValues || columns!=_root->getNumberOfDiscreteValues()){ return; }
  
  for (unsigned int r=0; r<rows; r++){
    for (unsigned int c=0; c<columns; c++){
      _condProbabilities[r][c] = probabilities[r*columns+c];
    }
  }
}

float NBL::EndNode::getCondProbForRootValue(int v)
{
  float res=0.0;
  
  if (m_type==BOOLEAN){
    if (_boolValue){
      res = _condProbabilities[0][v];
    } else {
      res = _condProbabilities[1][v];
    }
  } else if (m_type==DISCRETE || m_type==INTEGER){
    res = _condProbabilities[_discreteValue][v];
  } else {
    res = -1.0;
  }

  return res;
}

void NBL::EndNode::setValue(bool value)
{
  assert (m_type==BOOLEAN);
  
  _boolValue = value;
}

void NBL::EndNode::setValue(int value)
{
  assert (m_type==DISCRETE || m_type==INTEGER);

  _discreteValue = value;
}

void NBL::EndNode::setValue(string value)
{
  assert (m_type==DISCRETE);

  _discreteValue = translate(value);
}


#if Nodes_test
#include <stdio.h>
int main(int argc, char **argv)
{
  // This is a module-test block. You can put code here that tests
  // just the contents of this C file, and build it by saying
  //             make Nodes_test
  // Then, run the resulting executable (Nodes_test).
  // If it works as expected, the module is probably correct. ;-)

  fprintf(stderr, "Testing Nodes\n");

  return 0;
}
#endif /* Nodes_test */

/*! \if exist This is -*- C++ -*- from nbg \endif

    \file  NaiveBayesNet.cc

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

/* my includes */
#include "NaiveBayesNet.h"


NBL::NaiveBayesNet::NaiveBayesNet()
{
  _isValid = false;
}

NBL::NaiveBayesNet::~NaiveBayesNet()
{
  for (NodeList::iterator it=_endnodeList.begin(); it!=_endnodeList.end(); it++){
    delete it->second;
  }
}

bool NBL::NaiveBayesNet::addEndNode(string name, NBL::NodeValueType type, unsigned int numberOfValues)
{
  if (_endnodeList.find(name)!=_endnodeList.end()){ return false; }

  EndNode* newNode = new EndNode(&_root, name);

  if (type==BOOLEAN){
    newNode->initBoolNode();
  } else if (type==DISCRETE){
    newNode->initDiscreteNode(numberOfValues);
  } else {
    delete newNode;
    return false;
  }

  _endnodeList[name] = newNode;
  
  return true;
}

void NBL::NaiveBayesNet::learnRootProbabilities(vector<Instance>& trainData)
{
  int numberOfProbs=_root.getNumberOfDiscreteValues();
  float rootProbs[numberOfProbs];
  
  if (_root.getType()==BOOLEAN){
    int posExCount=0, negExCount=0;
    for (vector<Instance>::iterator it=trainData.begin(); it!=trainData.end(); it++){
      if (stringToBool(it->m_rootValue)){
	posExCount++;
      } else {
	negExCount++;
      }
    }
    rootProbs[0] = static_cast<double>(posExCount) / static_cast<double>(posExCount + negExCount);
    rootProbs[1] = static_cast<double>(negExCount) / static_cast<double>(posExCount + negExCount);
  } else if (_root.getType()==DISCRETE){
    int counts[numberOfProbs];
    int countSum=0;
    for (vector<Instance>::iterator it=trainData.begin(); it!=trainData.end(); it++){
      counts[_root.translate(it->m_rootValue)]++;
      countSum++;
    }
    for (int i=0; i<numberOfProbs; i++){
      rootProbs[i] = static_cast<double>(counts[i]) / static_cast<double>(countSum);
    }
  } else {
    return;
  }

  _root.setProbabilities(rootProbs, numberOfProbs);
}

void NBL::NaiveBayesNet::learnEndnodeProbabilities(vector<Instance>& trainData, vector<string> nameOrder)
{  
  for (unsigned int i=0; i<nameOrder.size(); i++){
    if (_endnodeList[nameOrder[i]]->getType()==BOOLEAN){
      int rows=2, cols=2;
      float depProbs[rows][cols];
      int count[rows][cols];
      for (int r=0; r<rows; r++){
	for (int c=0; c<cols; c++){
	  count[r][c] = 0;
	}
      }
      for (vector<Instance>::iterator instIt=trainData.begin(); instIt!=trainData.end(); instIt++){
	int row = stringToBool(instIt->m_attribValues[i])?0:1;
	int col=0;
	if (_root.getType()==BOOLEAN){
	  col = stringToBool(instIt->m_rootValue)?0:1;
	} else if (_root.getType()==DISCRETE){
	  col = _root.translate(instIt->m_rootValue);
	}
	count[row][col]++;
      }
      for (int r=0; r<rows; r++){
	for (int c=0; c<cols; c++){
	  depProbs[r][c] = static_cast<double>(count[r][c]) / static_cast<double>(trainData.size());
	}
      }
      _endnodeList[nameOrder[i]]->setCondProbabilities(reinterpret_cast<float*>(depProbs), rows, cols);
    } else if (_endnodeList[nameOrder[i]]->getType()==DISCRETE){
      int rows=_endnodeList[nameOrder[i]]->getNumberOfDiscreteValues();
      int cols=_root.getNumberOfDiscreteValues();
      float depProbs[rows][cols];
      int count[rows][cols];
      for (int r=0; r<rows; r++){
	for (int c=0; c<cols; c++){
	  count[r][c] = 0;
	}
      }
      for (vector<Instance>::iterator instIt=trainData.begin(); instIt!=trainData.end(); instIt++){
	int row = _endnodeList[nameOrder[i]]->translate(instIt->m_attribValues[i]);
	int col=0;
	if (_root.getType()==BOOLEAN){
	  col = stringToBool(instIt->m_rootValue)?0:1;
	} else if (_root.getType()==DISCRETE){
	  col = _root.translate(instIt->m_rootValue);
	}
	count[row][col]++;
      }
      for (int r=0; r<rows; r++){
	for (int c=0; c<cols; c++){
	  depProbs[r][c] = static_cast<double>(count[r][c]) / static_cast<double>(trainData.size());
	}
      }
      _endnodeList[nameOrder[i]]->setCondProbabilities(reinterpret_cast<float*>(depProbs), rows, cols);
    } else {
      continue;
    }
  }
}

bool NBL::NaiveBayesNet::constructNetFromReader(InstanceReader& reader)
{
  NodeDescription rootDescr = reader.getRootNodeDescription();
  switch (rootDescr.type){
  case BOOLEAN: initRootBool(rootDescr.name); break;
  case DISCRETE: initRootDiscrete(rootDescr.name, rootDescr.numberOfValues); break;
  default: return false;
  }

  vector<NodeDescription> nodesDescr = reader.getEndNodeDescriptions();
  for (vector<NodeDescription>::iterator it=nodesDescr.begin(); it!=nodesDescr.end(); it++){
    switch (it->type){
    case BOOLEAN: addBoolNode(it->name); break;
    case DISCRETE: addDiscreteNode(it->name, it->numberOfValues);
                   EndNode* node=getNodeByName(it->name);
		   for (set<string>::iterator valIt=it->discreteValues.begin(); valIt!=it->discreteValues.end(); valIt++){
		     node->addDiscreteValue(*valIt);
		   }
                   break;
    default: return false;
    }
  }

  return true;
}

void NBL::NaiveBayesNet::initRootBool(string rootName)
{
  _root.setName(rootName);
  _root.initBoolNode();
  _isValid = true;
}

void NBL::NaiveBayesNet::initRootDiscrete(string rootName, unsigned int numberOfValues)
{
  _root.setName(rootName);
  _root.initDiscreteNode(numberOfValues);
  _isValid = true;
}

bool NBL::NaiveBayesNet::addBoolNode(string name)
{
  if (!_isValid){ return false; }
  
  return addEndNode(name, BOOLEAN);
}

bool NBL::NaiveBayesNet::addDiscreteNode(string name, unsigned int numberOfValues)
{
  if (!_isValid){ return false; }

  return addEndNode(name, DISCRETE, numberOfValues);
}

NBL::RootNode* NBL::NaiveBayesNet::getRootNode()
{
  return &_root;
}

NBL::EndNode* NBL::NaiveBayesNet::getNodeByName(string name)
{
  EndNode* res=NULL;
  
  if (_endnodeList.find(name)!=_endnodeList.end()){
    res = _endnodeList[name];
  } else {
    res = NULL;
  }

  return res;
}

void NBL::NaiveBayesNet::setNodeValue(string name, bool value)
{
  if (_endnodeList.find(name)==_endnodeList.end()){ return; }

  _endnodeList[name]->setValue(value);
}

void NBL::NaiveBayesNet::setNodeValue(string name, int value)
{
  if (_endnodeList.find(name)==_endnodeList.end()){ return; }

  _endnodeList[name]->setValue(value);
}

void NBL::NaiveBayesNet::setNodeValue(string name, string value)
{
  if (_endnodeList.find(name)==_endnodeList.end()){ return; }

  _endnodeList[name]->setValue(value);
}

void NBL::NaiveBayesNet::calcClassificationResult(bool& res)
{
  if (_root.getType()!=BOOLEAN){ return; }

  float* distrib = new float[2];
  calcResultDistribution(distrib, 2);

  if (distrib[0] > distrib[1]){
    res = true;
  } else {
    res = false;
  }
}

void NBL::NaiveBayesNet::calcClassificationResult(int& res)
{
  if (_root.getType()!=DISCRETE){ return; }

  int length = _root.getNumberOfDiscreteValues();
  float* distrib = new float[length];
  calcResultDistribution(distrib, length);

  int maxIndex=0;
  for (int i=1; i<length; i++){
    if (distrib[i] > distrib[maxIndex]){
      maxIndex = i;
    }
  }

  res = maxIndex;
}

void NBL::NaiveBayesNet::calcResultDistribution(float* res, unsigned int length)
{
  for (unsigned int v=0; v<_root.getNumberOfDiscreteValues(); v++){
    float prob=1.0;
    for (NodeList::iterator it=_endnodeList.begin(); it!=_endnodeList.end(); it++){
//       cout << "Working on node >" << it->first << "< (with type" << it->second->getType() << ") (#Values=" << it->second->getNumberOfDiscreteValues() << ")" << endl;
      prob *= it->second->getCondProbForRootValue(v);
//       cout << "next cond prob: " << it->second->getCondProbForRootValue(v) << endl;
    }
    res[v] = prob * _root.getProbability(v);
//     cout << "next root prob: " << _root.getProbability(v) << endl;
//     cout << "Result of these factors: " << res[v] << endl;
  }
}

void NBL::NaiveBayesNet::learnParameters(vector<Instance>& trainData, vector<string> nodeOrder)
{
  learnRootProbabilities(trainData);

  learnEndnodeProbabilities(trainData, nodeOrder);
}

bool NBL::NaiveBayesNet::learnFromFile(string filename)
{
  InstanceReader reader(filename);
  if (!reader.isValid()){ return false; }
  
  if (!constructNetFromReader(reader)){ return false; }
  
  vector<NodeDescription> nodesDescr = reader.getEndNodeDescriptions();
  vector<string> nameOrder;
  for (vector<NodeDescription>::iterator it=nodesDescr.begin(); it!=nodesDescr.end(); it++){
    nameOrder.push_back(it->name);
  }

  vector<Instance>* instances = reader.getInstances();

  learnParameters(*instances, nameOrder);
  
  return true;
}


#ifdef NaiveBayesNet_test
#include <stdio.h>
int main(int argc, char **argv)
{
  // This is a module-test block. You can put code here that tests
  // just the contents of this C file, and build it by saying
  //             make NaiveBayesNet_test
  // Then, run the resulting executable (NaiveBayesNet_test).
  // If it works as expected, the module is probably correct. ;-)

  fprintf(stderr, "Testing NaiveBayesNet\n");

  return 0;
}
#endif /* NaiveBayesNet_test */

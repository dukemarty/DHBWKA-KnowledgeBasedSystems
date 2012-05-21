/*! \if exist This is -*- C++ -*- from nbg \endif

    \file  Main.cc

    \par Last Author: Martin Loesch (<martin.loesch@@kit.edu>)
    \par Date of last change: 13.04.11

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
#include "Main.h"
#include "Nodes.h"
#include "Instance.h"
#include "InstanceReader.h"

using namespace std;
using namespace NBL;



void normalizeDistribution(float* distribution, unsigned int length)
{
  float sum=0.0;

  for (unsigned int i=0; i<length; i++){
    sum += distribution[i];
  }

  for (unsigned int i=0; i<length; i++){
    distribution[i] /= sum;
  }
}

void setupHandCraftedNetwork(NaiveBayesNet& network)
{
  network.initRootBool("Stolen?");
  network.addBoolNode("Color=Red?");
  network.addBoolNode("Type=SUV?");
  network.addBoolNode("Origin=Domestic?");

  float relProbs[] = {0.5, 0.5};
  network.getRootNode()->setProbabilities(relProbs, 2);

  float condProbs[2][2];
  
  condProbs[0][0] = 0.6;  condProbs[0][1] = 0.4;
  condProbs[1][0] = 0.4;  condProbs[1][1] = 0.6;
  network.getNodeByName("Color=Red?")->setCondProbabilities(reinterpret_cast<float*>(condProbs), 2, 2);

  condProbs[0][0] = 0.25;  condProbs[0][1] = 0.75;
  condProbs[1][0] = 0.68;  condProbs[1][1] = 0.32;
  network.getNodeByName("Type=SUV?")->setCondProbabilities(reinterpret_cast<float*>(condProbs), 2, 2);

  condProbs[0][0] = 0.4;  condProbs[0][1] = 0.6;
  condProbs[1][0] = 0.6;  condProbs[1][1] = 0.4;
  network.getNodeByName("Origin=Domestic?")->setCondProbabilities(reinterpret_cast<float*>(condProbs), 2, 2);
}

void runAutoTheftNetwork(NaiveBayesNet& network)
{
  cout << "Results for  <Color=Yellow> <Type=Sports> <Origin=Imported>" << endl;
  network.setNodeValue("Color=Red?", false);
  network.setNodeValue("Type=SUV?", false);
  network.setNodeValue("Origin=Domestic?", false);

  float resultDistribution[2];
  network.calcResultDistribution(resultDistribution, 2);
  bool result;
  network.calcClassificationResult(result);
  
  cout << "Probability distribution:" << endl;
  cout << " TRUE->" << resultDistribution[0] << "      FALSE->" << resultDistribution[1] << endl;
  cout << "Normalized:" << endl;
  normalizeDistribution(resultDistribution, 2);
  cout << " TRUE->" << resultDistribution[0] << "      FALSE->" << resultDistribution[1] << endl;
  
  cout << "Classification result: " << (result?"true":"false") << endl << endl;


  cout << "Results for  <Color=Red> <Type=SUV> <Origin=Domestic>" << endl;
  network.setNodeValue("Color=Red?", true);
  network.setNodeValue("Type=SUV?", true);
  network.setNodeValue("Origin=Domestic?", true);

  network.calcResultDistribution(resultDistribution, 2);
  network.calcClassificationResult(result);
  
  cout << "Probability distribution:" << endl;
  cout << " TRUE->" << resultDistribution[0] << "      FALSE->" << resultDistribution[1] << endl;
  cout << "Normalized:" << endl;
  normalizeDistribution(resultDistribution, 2);
  cout << " TRUE->" << resultDistribution[0] << "      FALSE->" << resultDistribution[1] << endl;
  
  cout << "Classification result: " << (result?"true":"false") << endl << endl;
}

void runSlideExampleNetwork(NBL::NaiveBayesNet& network)
{
  cout << "Results for <Vorhersage=sonnig> <Temperatur=heiss> <Luftfeuchtigkeit=hoch> <Wind=schwach>" << endl;
  network.setNodeValue("Vorhersage", string("sonnig"));
  network.setNodeValue("Temperatur", string("heiss"));
  network.setNodeValue("Luftfeuchtigkeit", string("hoch"));
  network.setNodeValue("Wind", string("schwach"));

  float resultDistribution[2];
  network.calcResultDistribution(resultDistribution, 2);
  bool result;
  network.calcClassificationResult(result);
  
  cout << "Probability distribution:" << endl;
  cout << " TRUE->" << resultDistribution[0] << "      FALSE->" << resultDistribution[1] << endl;
  cout << "Normalized:" << endl;
  normalizeDistribution(resultDistribution, 2);
  cout << " TRUE->" << resultDistribution[0] << "      FALSE->" << resultDistribution[1] << endl;
  
  cout << "Classification result: " << (result?"true":"false") << endl << endl;


  cout << "Results for <Vorhersage=sonnig> <Temperatur=kalt> <Luftfeuchtigkeit=hoch> <Wind=stark>" << endl;
  network.setNodeValue("Vorhersage", string("sonnig"));
  network.setNodeValue("Temperatur", string("kalt"));
  network.setNodeValue("Luftfeuchtigkeit", string("hoch"));
  network.setNodeValue("Wind", string("stark"));

  network.calcResultDistribution(resultDistribution, 2);
  network.calcClassificationResult(result);
  
  cout << "Probability distribution:" << endl;
  cout << " TRUE->" << resultDistribution[0] << "      FALSE->" << resultDistribution[1] << endl;
  cout << "Normalized:" << endl;
  normalizeDistribution(resultDistribution, 2);
  cout << " TRUE->" << resultDistribution[0] << "      FALSE->" << resultDistribution[1] << endl;
  
  cout << "Classification result: " << (result?"true":"false") << endl << endl;
}

void setupNetworkForLearning(NBL::NaiveBayesNet& network)
{
  network.initRootBool("Stolen?");
  network.addBoolNode("Color=Red?");
  network.addBoolNode("Type=SUV?");
  network.addBoolNode("Origin=Domestic?");
}

void learnNetworkManually(NaiveBayesNet& network)
{
  vector<Instance> trainData;

  Instance temp;
  temp.m_attribValues.resize(3);

  temp.m_rootValue = "ja";
  temp.m_attribValues[0] = "ja";  temp.m_attribValues[1] = "nein";  temp.m_attribValues[2] = "ja";
  trainData.push_back(temp);

  temp.m_rootValue = "nein";
  temp.m_attribValues[0] = "ja";  temp.m_attribValues[1] = "nein";  temp.m_attribValues[2] = "ja";
  trainData.push_back(temp);

  temp.m_rootValue = "ja";
  temp.m_attribValues[0] = "ja";  temp.m_attribValues[1] = "nein";  temp.m_attribValues[2] = "ja";
  trainData.push_back(temp);

  temp.m_rootValue = "nein";
  temp.m_attribValues[0] = "nein";  temp.m_attribValues[1] = "nein";  temp.m_attribValues[2] = "ja";
  trainData.push_back(temp);

  temp.m_rootValue = "ja";
  temp.m_attribValues[0] = "nein";  temp.m_attribValues[1] = "nein";  temp.m_attribValues[2] = "nein";
  trainData.push_back(temp);

  temp.m_rootValue = "nein";
  temp.m_attribValues[0] = "nein";  temp.m_attribValues[1] = "ja";  temp.m_attribValues[2] = "nein";
  trainData.push_back(temp);

  temp.m_rootValue = "ja";
  temp.m_attribValues[0] = "nein";  temp.m_attribValues[1] = "ja";  temp.m_attribValues[2] = "nein";
  trainData.push_back(temp);

  temp.m_rootValue = "nein";
  temp.m_attribValues[0] = "nein";  temp.m_attribValues[1] = "ja";  temp.m_attribValues[2] = "ja";
  trainData.push_back(temp);

  temp.m_rootValue = "nein";
  temp.m_attribValues[0] = "ja";  temp.m_attribValues[1] = "ja";  temp.m_attribValues[2] = "nein";
  trainData.push_back(temp);

  temp.m_rootValue = "ja";
  temp.m_attribValues[0] = "ja";  temp.m_attribValues[1] = "nein";  temp.m_attribValues[2] = "nein";
  trainData.push_back(temp);

  for (unsigned int i=0; i<trainData.size(); i++){
    cout << " Instance " << i << " :  " << trainData[i].toString() << endl;
  }

  vector<string> nameOrder;
  nameOrder.push_back("Color=Red?");
  nameOrder.push_back("Type=SUV?");
  nameOrder.push_back("Origin=Domestic?");
  
  network.learnParameters(trainData, nameOrder);
}

void testHandCraftedNetwork()
{
  cout << endl << endl << " --- Test manually built network ---" << endl;

  NaiveBayesNet testNet;
  setupHandCraftedNetwork(testNet);

  runAutoTheftNetwork(testNet);
}

void testManualInstances()
{
  cout << endl << endl << " --- Test learning from manually inserted data ---" << endl;

  NaiveBayesNet testNet;
  setupNetworkForLearning(testNet);
  learnNetworkManually(testNet);

  runAutoTheftNetwork(testNet);
}

void testLoadingOfData()
{
  cout << endl << endl << " --- Test loading of data files ---" << endl;

  cout << endl << " - 1st test case:  >data/cartheft.txt< --" << endl;
  testSingleFileLoading("data/cartheft.txt");

  cout << endl << " - 2nd test case:  >data/folienbeispiel.txt< --";
  testSingleFileLoading("data/folienbeispiel.txt");
}

void testSingleFileLoading(string filename)
{
  InstanceReader reader1(filename);

  vector<Instance>* instances = reader1.getInstances();

  for (unsigned int i=0; i<instances->size(); i++){
    cout << " Instance " << i << " :  " << (*instances)[i].toString() << endl;
  }
}

void testLearningFromFile()
{
  cout << endl << endl << " --- Test learning from file ---" << endl;

  NaiveBayesNet testNet;
  testNet.learnFromFile("data/cartheft.txt");

  runAutoTheftNetwork(testNet);
}

void testComplexLearningFromFile()
{
  cout << endl << endl << " --- Test complex learning from file ---" << endl;

  NaiveBayesNet testNet;
  testNet.learnFromFile("data/folienbeispiel.txt");

  runSlideExampleNetwork(testNet);
}

int main(int argc, char **argv)
{
  cout << endl << "       ==================================" << endl;
  cout  << " ===== Naive Bayesian Classifiers Library =====" << endl;
  cout  << "       ==================================" << endl;


  testHandCraftedNetwork();

  testManualInstances();

  testLoadingOfData();

  testLearningFromFile();

  testComplexLearningFromFile();
  
  return 0;
}

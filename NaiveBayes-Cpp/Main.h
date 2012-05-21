/*! \if exist This is -*- C++ -*- from nbg \endif

    \file  Main.h
    \brief 

    Please put your documentation for this file here.

    \par Last Author: Martin Loesch (<loesch@@ira.uka.de>)
    Date of last change: 11.04.10

    \author   Martin Loesch (<loesch@@ira.uka.de>)
    \date     2010-04-08
    \par Copyright:
              Martin Loesch, Chair Prof. Dillmann (IAIM)\n
              Institute for Computer Science and Engineering (CSE)\n
	      University of Karlsruhe. All rights reserved\n
	      http://wwwiaim.ira.uka.de
*/

#ifndef MAIN_H
#define MAIN_H

/* system includes */
#include <string>

/* my includes */
#include "NaiveBayesNet.h"


void normalizeDistribution(float* distribution, unsigned int length);

/*!
  \brief Build a complete NBN by hand and use it to classify some examples.
*/
void testHandCraftedNetwork();
void setupHandCraftedNetwork(NBL::NaiveBayesNet& network);

void testManualInstances();
void setupNetworkForLearning(NBL::NaiveBayesNet& network);
void learnNetworkManually(NBL::NaiveBayesNet& network);

void testSingleFileLoading(string filename);
void testLoadingOfData();

void testLearningFromFile();
void testComplexLearningFromFile();

void runAutoTheftNetwork(NBL::NaiveBayesNet& network);
void runSlideExampleNetwork(NBL::NaiveBayesNet& network);

#endif /* MAIN_H */

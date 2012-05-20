/*! \if exist This is -*- C++ -*- from nbg \endif

    \file  PerceptronLearningFramework.h
    \brief 

    Please put your documentation for this file here.

    \par Last Author: Martin Loesch (<martin.loesch@@kit.edu>)
    Date of last change: 28.11.11

    \author   Martin Loesch (<loesch@@ira.uka.de>)
    \date     2011-03-23
    \par Copyright:
              Martin Loesch, Chair Prof. Dillmann (HIS)
              Institute for Anthropomatics (IFA)
	      Karlsruhe Institute of Technology (KIT). All rights reserved
	      http://his.anthropomatik.kit.edu
*/

#ifndef PERCEPTRONLEARNINGFRAMEWORK_H
#define PERCEPTRONLEARNINGFRAMEWORK_H

/* system includes */
#include <string>
#include <vector>

/* my includes */
#include <Perceptron/Perceptron.h>

using namespace std;
namespace FL = Flood;


typedef enum {
  POS,
  NEG
} ClassLabel;


/*!
  \brief Labeled data instance for a two-class problem.
*/
struct LabeledData {
  ClassLabel label;
  FL::Vector<double> data;

  LabeledData(int length);
};

/*!
  \brief configuration parameters for a perceptron.
*/
struct PerceptronConfig {
  double threshold;
  FL::Vector<double> weights;

  PerceptronConfig(int length);
};

typedef vector<LabeledData> TrainData;

/*!
  \brief Apply configuration parameter object to a perceptron.

  @param neuron perceptron which is configured
  @param config configuration parameters
*/
void applyConfiguration(FL::Perceptron* neuron, PerceptronConfig& config);

/*!
  \brief Load training data from a text file.

  Format of the file:
  Number of attributes
  val_1, val_2, ..., val_numberOfAttributes, CLASS

  where CLASS can be either '+' or '-'.

  \attention The returned data object must be freed by the caller!
  
  @param filename name of the text file with the data
  @return TrainData object containing the data from the file
*/
TrainData* loadDataFromFile(string filename);

/*!
  \brief Train new perceptron fo a given set of training data.

  \attention The returned perceptron must be freed by the caller!

  @param data set of training data
  @return new perceptron trained to recognize the classes described by the training data
*/
FL::Perceptron* trainNewPerceptron(TrainData* data);

/*!
  \brief Show definint parameters of perceptron on standard out.

  @param neuron perceptron which is to be printed
*/
void printPerceptron(FL::Perceptron* neuron);

/*!
  \brief Run loop for using perceptron.

  @param neuron perceptron which is to be tested.
*/
void runPerceptron(FL::Perceptron* neuron);

/*!
  \brief Check whether perceptron correctly classifies all training data in a data set.

  @param neuron perceptron which is tested
  @param data data set on which the perceptron is tested
  @return true if all instances are classified correctly, false else
*/
bool evaluatePerceptronCorrectness(FL::Perceptron* neuron, TrainData& data);


#endif /* PERCEPTRONLEARNINGFRAMEWORK_H */

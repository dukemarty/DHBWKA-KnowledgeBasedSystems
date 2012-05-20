/*! \if exist This is -*- C++ -*- from nbg \endif

    \file  PerceptronLearning.h
    \brief 

    Please put your documentation for this file here.

    \par Last Author: Martin Loesch (<martin.loesch@@kit.edu>)
    Date of last change: 20.05.12

    \author   Martin Loesch (<loesch@@ira.uka.de>)
    \date     2011-03-23
    \par Copyright:
              Martin Loesch, Chair Prof. Dillmann (HIS)
              Institute for Anthropomatics (IFA)
			  Karlsruhe Institute of Technology (KIT). All rights reserved
			  http://his.anthropomatik.kit.edu
*/

#ifndef PERCEPTRONLEARNING_H
#define PERCEPTRONLEARNING_H

/* system includes */
#include <string>
#include <vector>

/* my includes */
#include <Perceptron/Perceptron.h>

using namespace std;
namespace FL = Flood;

//! \brief Enum for two-class class labels.
typedef enum {
  POS,
  NEG
} ClassLabel;

/*!
	\brief Struct for labeled data.
	
	An object of this struct contains a lable of the data and a double vector (in Flood format) of the given length as data.
*/
struct LabeledData {
  ClassLabel label;
  FL::Vector<double> data;

  /*!
	\brief Constructor for labeled data.
	
	@param length size of data vector
  */
  LabeledData(int length);
};

/*!
	\brief Struct for a perceptron configuration.
	
	Objects of this struct can be used for learning of perceptron configurations for given problems.
*/
struct PerceptronConfig {
  double threshold;
  FL::Vector<double> weights;

  /*!
	\brief Constructor for perceptron configuration.
	
	@param length number of input slots of the perceptron (= number of weights that define the perceptron)
  */
  PerceptronConfig(int length);
};

//! define a vector of LabeledData as TrainData
typedef vector<LabeledData> TrainData;

/*!
	\brief Initialize perceptron configuration randomly.
	
	Thresholds and weight are given a random value between 1/1000 and 1.
	
	@param config configuration object that is to be initialized
*/
void initRandomly(PerceptronConfig& config);

/*!
	\brief Set a perceptron to a given configuration. 
	
	@param neuron perceptron that is to be configured
	@param config configuration that is applied to the perceptron
*/
void applyConfig(FL::Perceptron* neuron, PerceptronConfig& config);

/*!
	\brief Check whether a perceptron performs correctly on a given data set.
	
	The classification uses a threshold of 0.5 on the output value of the perceptron.
	
	@param neuron perceptron that is tested
	@param data set of labeled data
	@return true if full data set is classified correctly, false else.
*/
bool checkoutClassification(FL::Perceptron* neuron, TrainData& data);

/*!
	\brief Train a perceptron to recognize a function defined in a training data set.
	
	Internally, the perceptron learning algorithm is applied, but without using a delta parameter to control the speed of the convergence/to ensure a convergence at all.
	
	@param neuron perceptron that is trained
	@param data data set of the function which shall be learned
*/
void trainPerceptron(FL::Perceptron* neuron, TrainData& data);

/*!
	\brief Allow interactive testing of a perceptron.
	
	The user repeatedly allowed to enter input signals which are fed into the perceptron, and the result of the perceptron is printed to standard out.
	
	@param neuron perceptron that shall be tested
*/
void testPerceptron(FL::Perceptron* neuron);


#endif /* PERCEPTRONLEARNING_H */

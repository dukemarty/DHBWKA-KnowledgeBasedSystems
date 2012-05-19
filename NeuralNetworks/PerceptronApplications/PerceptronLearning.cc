/*! \if exist This is -*- C++ -*- from nbg \endif

    \file  PerceptronLearning.cc

    \par Last Author: Martin Loesch (<martin.loesch@@kit.edu>)
    \par Date of last change: 23.03.11

    \author   Martin Loesch (<loesch@@ira.uka.de>)
    \date     2011-03-23
    \par Copyright:
              Martin Loesch, Chair Prof. Dillmann (HIS)\n
              Institute for Anthropomatics (IFA)\n
			  Karlsruhe Institute of Technology (KIT). All rights reserved\n
			  http://his.anthropomatik.kit.edu
*/

/* system includes */
#include <assert.h>
#include <cstdlib>
#include <iostream>

/* my includes */
#include "PerceptronLearning.h"


LabeledData::LabeledData(int length) : data(length)
{
}


PerceptronConfig::PerceptronConfig(int length) : weights(length)
{
}


void initRandomly(PerceptronConfig& config)
{
  srand(time(NULL));

  config.threshold = 1.0 / static_cast<double>(rand() % 1000 + 1);
  
  for (unsigned int i=0; i<config.weights.get_size(); i++){
    config.weights[i] = 1.0 / static_cast<double>(rand() % 1000 + 1);
  }
}

void applyConfig(FL::Perceptron* neuron, PerceptronConfig& config)
{
  cout << "New configuration:" << endl;
  cout << "config.threshold = " << config.threshold << endl;
  for (unsigned int i=0; i<config.weights.get_size(); i++){
    cout << "config.weights[" << i << "] = " << config.weights[i] << endl;
  }
  
  neuron->set_bias(config.threshold);
  neuron->set_synaptic_weights(config.weights);
}

bool checkoutClassification(FL::Perceptron* neuron, TrainData& data)
{
  bool res=true;
  
  for (TrainData::iterator it=data.begin(); it!=data.end(); ++it){
    double output = neuron->calculate_output(it->data);
    if (it->label==POS){
      output>0.5?res&=true:res&=false;
    } else {
      output<0.5?res&=true:res&=false;
    }
  }

  return res;
}

void trainPerceptron(FL::Perceptron* neuron, TrainData& data)
{
  PerceptronConfig adaptConfig(neuron->get_inputs_number());
  initRandomly(adaptConfig);

  neuron->set_activation_function(FL::Perceptron::Threshold);
  applyConfig(neuron, adaptConfig);
  
  while (!checkoutClassification(neuron, data)){
    LabeledData* testInst = &(data[rand() % data.size()]);

    if (testInst->label == POS){
      if (neuron->calculate_output(testInst->data)<0.5){
		adaptConfig.threshold += 1.0;
		for (unsigned int i=0; i<adaptConfig.weights.get_size(); ++i){
			adaptConfig.weights[i] += testInst->data[i];
		}
	  }
    } else {  // testInst->label == NEG
      if (neuron->calculate_output(testInst->data)>0.5){
		adaptConfig.threshold -= 1.0;
		for (unsigned int i=0; i<adaptConfig.weights.get_size(); ++i){
			adaptConfig.weights[i] -= testInst->data[i];
		}
      }
    }
    applyConfig(neuron, adaptConfig);
  }
}

void testPerceptron(FL::Perceptron* neuron)
{
  char repeat='y';
  FL::Vector<double> inputData(neuron->get_inputs_number());
  
  while (repeat !='n' && repeat != 'N'){
    for (int i=0; i<neuron->get_inputs_number(); i++){
      cout << "Value " << i << ":  ";
      double val;
      cin >> val;
      inputData[i] = val;
    }

    cout << "Result of perceptron:  " << neuron->calculate_output(inputData) << endl;

    cout << endl << "Repeat? (y/n)  ";
    cin >> repeat;
    cout << endl;
  }
}

int main(int argc, char **argv)
{
  cout << " ** Learning of Perceptron Parameters **" << endl;

  FL::Perceptron* learnedAnd = new FL::Perceptron(2);
  TrainData andData;
  LabeledData p00(2); p00.label = NEG; p00.data[0] = 0; p00.data[1] = 0;
  LabeledData p01(2); p01.label = NEG; p01.data[0] = 0; p01.data[1] = 1;
  LabeledData p10(2); p10.label = NEG; p10.data[0] = 1; p10.data[1] = 0;
  LabeledData p11(2); p11.label = POS; p11.data[0] = 1; p11.data[1] = 1;
  andData.push_back(p00);
  andData.push_back(p01);
  andData.push_back(p10);
  andData.push_back(p11);

  trainPerceptron(learnedAnd, andData);

  testPerceptron(learnedAnd);
  
  return 0;
}

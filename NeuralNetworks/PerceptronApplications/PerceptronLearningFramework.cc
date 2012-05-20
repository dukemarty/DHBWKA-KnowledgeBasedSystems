/*! \if exist This is -*- C++ -*- from nbg \endif

    \file  PerceptronLearningFramework.cc

    \par Last Author: Martin Loesch (<martin.loesch@@kit.edu>)
    \par Date of last change: 20.05.12

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
#include "PerceptronLearningFramework.h"


LabeledData::LabeledData(int length) : data(length)
{
}


PerceptronConfig::PerceptronConfig(int length) : weights(length)
{
}


void initializePerceptronConfiguration(PerceptronConfig& config)
{
  srand(time(NULL));

  config.threshold = 1.0 / static_cast<double>(rand() % 100 + 1);
  
  for (unsigned int i=0; i<config.weights.get_size(); ++i) {
      config.weights[i] = 1.0 / static_cast<double>(rand() % 100 + 1);
  }
}

void applyConfiguration(FL::Perceptron* neuron, PerceptronConfig& config)
{
  cout << "New configuration:" << endl;
  cout << "config.threshold = " << config.threshold << endl;
  for (unsigned int i=0; i<config.weights.get_size(); i++){
    cout << "config.weights[" << i << "] = " << config.weights[i] << endl;
  }
  
  neuron->set_bias(config.threshold);
  neuron->set_synaptic_weights(config.weights);
}

TrainData* loadDataFromFile(string filename)
{
  TrainData* res = new TrainData;
  int attribNumber=0;

  FILE* datafile;
  datafile = fopen(filename.c_str(), "r");

  if (!datafile){
    cerr << "ERROR while opening file \"" << filename << "\":  Could not be opened!" << endl;
    return NULL;
  }
  
  fscanf(datafile, "%i\n", &attribNumber);

  float nextVal=0.0;
  char nextClass=' ';
  while (!feof(datafile)){
    LabeledData instance(attribNumber);
    fscanf(datafile, "%f", &nextVal);
    instance.data[0] = nextVal;
    for (unsigned int i=1; i<attribNumber; i++){
      fscanf(datafile, ",%f", &nextVal);
      instance.data[i] = nextVal;
    }
    fscanf(datafile, ",%c\n", &nextClass);

    if (nextClass=='+'){
      instance.label = POS;
    } else if (nextClass='-'){
      instance.label = NEG;
    } else {
      cerr << "ERROR: While loading file \"" << filename << "\"" << endl;
    }
    
    res->push_back(instance);
  }

  fclose(datafile);
  
  return res;
}

bool evaluatePerceptronCorrectness(FL::Perceptron* neuron, TrainData& data)
{
  bool res=true;
  
  for (TrainData::iterator it=data.begin(); it!=data.end(); it++){
    double output = neuron->calculate_output(it->data);
    if (it->label==POS){
      output>0.5?res&=true:res&=false;
    } else {
      output<0.5?res&=true:res&=false;
    }
  }

  return res;
}

void printPerceptron(FL::Perceptron* neuron)
{
  cout << "Inner structure of perceptron:" << endl;
  for (int i=0; i<neuron->get_inputs_number(); i++){
    cout << "  w_" << i << " = " << neuron->get_synaptic_weight(i);
  }
  cout << endl;
  cout << " bias=" << neuron->get_bias() << endl;
}

void runPerceptron(FL::Perceptron* neuron)
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

FL::Perceptron* trainNewPerceptron(TrainData* data)
{
  cout << "Training new perceptron";
  
  FL::Perceptron* resultPerceptron = new FL::Perceptron(data->operator[](0).data.get_size());
  resultPerceptron->set_activation_function(FL::Perceptron::Threshold);
  PerceptronConfig config(data->operator[](0).data.get_size());

  initializePerceptronConfiguration(config);
  applyConfiguration(resultPerceptron, config);
  
  while (!evaluatePerceptronCorrectness(resultPerceptron, *data)){
    int nextEx = rand() % data->size();
    double output = resultPerceptron->calculate_output((*data)[nextEx].data);
//     cout << "Result for example " << nextEx << " = " << output << endl;
    
    if ((*data)[nextEx].label == POS){
      if (output < 0.5){
	for (unsigned int i=0; i<config.weights.get_size(); ++i){
	  config.weights[i] += (*data)[nextEx].data[i];
	}
	config.threshold += 1.0;
	applyConfiguration(resultPerceptron, config);
      }
    } else { // label == NEG
      if (output > 0.5){
	for (unsigned int i=0; i<config.weights.get_size(); ++i){
// 	  cout << "adapt weights; -= " << (*data)[nextEx].data[i] << endl;
	  config.weights[i] -= (*data)[nextEx].data[i];
	}
	config.threshold -= 1.0;
	applyConfiguration(resultPerceptron, config);
      }
    }
  }
  
  return resultPerceptron;
}

int main(int argc, char **argv)
{
  cout << " ** Learning of Perceptron Parameters **" << endl;

  string filename = "andData.txt";
  if (argc>1){
    filename = argv[1];
  } else {
    cout <<   "As no filename was given as parameter, the data set for the logical AND operator is used!" << endl;
  }

  TrainData* trainData = loadDataFromFile(filename);
  FL::Perceptron* learnedOperator = trainNewPerceptron(trainData);
  delete trainData;
  
  runPerceptron(learnedOperator);

  delete learnedOperator;
  
  return 0;
}

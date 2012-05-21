
#include <iostream>

#include <MultilayerPerceptron/MultilayerPerceptron.h>
#include <ObjectiveFunctional/SumSquaredError.h>
#include <TrainingAlgorithm/GradientDescent.h>

#include "MlnnFromFileApplication.h"


namespace FL = Flood;
using namespace std;


LabeledData::LabeledData(int length) : data(length)
{
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


FL::InputTargetDataSet* generateTrainDataSet(TrainData* data)
{
  int instCount = data->size();
  int featCount = (*data)[0].data.get_size();
  
  FL::InputTargetDataSet* res = new FL::InputTargetDataSet(instCount, featCount, 1);
  
  FL::Matrix<double> trainMatrix(instCount, featCount+1);
  FL::Vector<double> trainSample(featCount+1);

  for (unsigned int i=0; i<data->size(); ++i){
    for (unsigned int f=0; f<featCount; ++f){
      trainSample[f] = (*data)[i].data[f];
    }
    if ((*data)[i].label==POS){
      trainSample[featCount] = 1.0;
    } else { // ((*data)[i].label==NEG)
      trainSample[featCount] = -1.0;
    }
    trainMatrix.set_row(i, trainSample);
  }

  res->set_data(trainMatrix);
  
  return res;
}

bool thresholdWithNull(double value)
{
  return (value > 0.0);
}

int main(int argc, char* argv[])
{
  cout << "Test Multilayer Neural Network class from Flood3 library!" << endl;
  
  // initialize neural network
  FL::Vector<int> hiddenL(1);
  hiddenL[0] = 8;
//   hiddenL[1] = 7;
  FL::MultilayerPerceptron net(2, hiddenL, 1);

  // initialize training data
  FL::Matrix<double> dataMatrix(4, 3);
  FL::Vector<double> row(3);
  row[0] = 0.0;   row[1] = 0.0;    row[2] = -1.0;
  dataMatrix.set_row(0, row);
  row[0] = 1.0;   row[1] = 1.0;    row[2] = -1.0;
  dataMatrix.set_row(1, row);
  row[0] = 1.0;   row[1] = 0.0;    row[2] = +1.0;
  dataMatrix.set_row(2, row);
  row[0] = 0.0;   row[1] = 1.0;    row[2] = +1.0;
  dataMatrix.set_row(3, row);

//   row[0] = 0.9;   row[1] = 1.1;    row[2] = -1.0;
//   dataMatrix.set_row(4, row);
//   row[0] = 1.1;   row[1] = 0.1;    row[2] = +1.0;
//   dataMatrix.set_row(5, row);
  
  FL::InputTargetDataSet* trainData = new FL::InputTargetDataSet(4, 2, 1);
  trainData->set_data(dataMatrix);


  // initialize learning components
  FL::SumSquaredError errorMeasure(&net, trainData);
  FL::GradientDescent learner(&errorMeasure);
  learner.set_maximum_epochs_number(500);
  learner.set_minimum_evaluation_improvement(1.0e-9);
  learner.set_gradient_norm_goal(0.0);

  learner.train();
  
  // evaluate trained network, e.g. via
  
  char repeat='y';
  double inputData[2];
  
  while (repeat != 'n' && repeat != 'N'){
    for (int i=0; i<2; i++){
      cout << "Value " << i << ":  ";
      cin >> inputData[i];
    }

    FL::Vector<double> input(2);
    input[0] = inputData[0];
    input[1] = inputData[1];
    double result = net.calculate_output(input)[0];
    // APPLY network to (inputData[0], inputData[1])
    cout << "Result of network: " << (thresholdWithNull(result)?"true":"false") << "  (" << result << ")" << endl; 

    cout << endl << "Repeat? (y/n)  ";
    cin >> repeat;
    cout << endl;
  }	
  
  
  delete trainData;
  
  return 0;
}


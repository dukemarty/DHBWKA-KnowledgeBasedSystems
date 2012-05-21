
#include <iostream>

#include <MultilayerPerceptron/MultilayerPerceptron.h>
#include <ObjectiveFunctional/SumSquaredError.h>
#include <TrainingAlgorithm/GradientDescent.h>

#include "MlnnFromFileApplication.h"


namespace FL = Flood;
using namespace std;


GeneralData::GeneralData(int inputLength, int outputLength) : inputData(inputLength), outputData(outputLength)
{
}
						      
GeneralTrainData* loadGeneralDataFromFile(string filename)
{
  GeneralTrainData* res = new GeneralTrainData;
  int attribNumber=0, resNumber=0;

  FILE* datafile;
  datafile = fopen(filename.c_str(), "r");

  if (!datafile){
    cerr << "ERROR while opening file \"" << filename << "\":  Could not be opened!" << endl;
    return NULL;
  }
  
  fscanf(datafile, "%i\n%i\n", &attribNumber, &resNumber);

  float nextVal=0.0;
  char nextClass=' ';
  while (!feof(datafile)){
    GeneralData instance(attribNumber, resNumber);
    fscanf(datafile, "%f", &nextVal);
    instance.inputData[0] = nextVal;
    for (unsigned int i=1; i<attribNumber; i++){
      fscanf(datafile, ",%f", &nextVal);
      instance.inputData[i] = nextVal;
    }
    fscanf(datafile, ",%f", &nextVal);
    instance.outputData[0] = nextVal;
    for (unsigned int i=1; i<resNumber; i++){
      fscanf(datafile, ",%f", &nextVal);
      instance.outputData[i] = nextVal;
    }

    res->push_back(instance);
  }

  fclose(datafile);
  
  return res;
}

FL::InputTargetDataSet* generateGeneralTrainDataSet(GeneralTrainData* data)
{
  int instCount = data->size();
  int featCount = (*data)[0].inputData.get_size();
  int resCount = (*data)[0].outputData.get_size();
  
  FL::InputTargetDataSet* res = new FL::InputTargetDataSet(instCount, featCount, resCount);
  
  FL::Matrix<double> trainMatrix(instCount, featCount+resCount);
  FL::Vector<double> trainSample(featCount+resCount);

  for (unsigned int i=0; i<data->size(); ++i){
    unsigned int f;
    for (f=0; f<featCount; ++f){
      trainSample[f] = (*data)[i].inputData[f];
    }
    for (unsigned int r=0; r<resCount; ++r){
      trainSample[f+r] = (*data)[i].outputData[r];
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
  
  string filename = "ex3Data1.txt";
  if (argc>1){
    filename = argv[1];
  } else {
    cout <<   "As no filename was given as parameter, the data set for the logical AND operator is used!" << endl;
  }

  GeneralTrainData* rawTrainData = loadGeneralDataFromFile(filename);
  FL::InputTargetDataSet* trainData = generateGeneralTrainDataSet(rawTrainData);

  // initialize neural network
  FL::Vector<int> hiddenL(2);
  hiddenL[0] = 6;
  hiddenL[1] = 6;
  FL::MultilayerPerceptron net(trainData->get_input_variables_number(), hiddenL, trainData->get_target_variables_number());

  // initialize learning components
  FL::SumSquaredError errorMeasure(&net, trainData);
  FL::GradientDescent learner(&errorMeasure);
  learner.set_maximum_epochs_number(1000);
  learner.set_minimum_evaluation_improvement(1.0e-9);
  learner.set_gradient_norm_goal(0.0);

  // train networks using trainData 
  learner.train();

  
  char repeat='y';
  double* inputData = new double[trainData->get_input_variables_number()];
  
  while (repeat != 'n' && repeat != 'N'){
    for (int i=0; i<trainData->get_input_variables_number(); i++){
      cout << "Value " << i << ":  ";
      cin >> inputData[i];
    }

    FL::Vector<double> data(trainData->get_input_variables_number());
    for (int i=0; i<trainData->get_input_variables_number(); ++i){
      data[i] = inputData[i];
    }
    
    FL::Vector<double> result = net.calculate_output(data); // 0.0; // APPLY network to (inputData[0], inputData[1])
    cout << "Result of network: ";
    for (unsigned int i=0; i<trainData->get_target_variables_number(); ++i){
//       cout << "   |   Var " << i << " :    " << result[i];
      cout << "   |   Var " << i << " :    " << (thresholdWithNull(result[i])?"true":"false") << " (" << result[i] << ")";
    }
    cout << endl;

    
//     << (thresholdWithNull(result)?"true":"false") << "  (" << result << ")" << endl; 
    cout << endl << "Repeat? (y/n)  ";
    cin >> repeat;
    cout << endl;
  }	
  
  
  delete trainData;
  
  return 0;
}


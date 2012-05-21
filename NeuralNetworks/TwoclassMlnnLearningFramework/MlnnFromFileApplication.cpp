
#include <iostream>

#include <MultilayerPerceptron/MultilayerPerceptron.h>

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
  
  string filename = "andData.txt";
  if (argc>1){
    filename = argv[1];
  } else {
    cout <<   "As no filename was given as parameter, the data set for the logical AND operator is used!" << endl;
  }

  TrainData* rawTrainData = loadDataFromFile(filename);
  FL::InputTargetDataSet* trainData = generateTrainDataSet(rawTrainData);

  // initialize neural networks

  // train networks using trainData 

  // evaluate trained network, e.g. via
  
  char repeat='y';
  double inputData[2];
  
  while (repeat != 'n' && repeat != 'N'){
    for (int i=0; i<2; i++){
      cout << "Value " << i << ":  ";
      cin >> inputData[i];
    }

    double result = // APPLY network to (inputData[0], inputData[1])
    cout << "Result of network: " << (thresholdWithNull(result)?"true":"false") << "  (" << result << ")" << endl; 

    cout << endl << "Repeat? (y/n)  ";
    cin >> repeat;
    cout << endl;
  }	
  
  
  delete trainData;
  
  return 0;
}


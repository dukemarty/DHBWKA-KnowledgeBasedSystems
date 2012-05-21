
#include <iostream>

#include <MultilayerPerceptron/MultilayerPerceptron.h>

#include "MlnnEx3Application.h"
#include "Ex3Mlnn.h"


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

  FL::Vector<int> hiddenLayers2(2, 2); // short for setting each element to "2"
  FL::Vector<int> hiddenLayers1(1, 4);
  
  Ex3Mlnn myOperator1, myOperator2(hiddenLayers2), myOperator3(hiddenLayers1);

  myOperator1.train(trainData);
  myOperator2.train(trainData);
  myOperator3.train(trainData);
  cout << "Result of training the first network (1HL, 2Ns):" << endl << endl;
  myOperator1.print();
  cout << "Result of training the second network (2HL, je 2Ns):" << endl << endl;
  myOperator2.print();
  cout << "Result of training the third network (1HL, 4Ns):" << endl << endl;
  myOperator3.print();

  char repeat='y';
  double inputData[2];
  
  while (repeat != 'n' && repeat != 'N'){
    for (int i=0; i<2; i++){
      cout << "Value " << i << ":  ";
      cin >> inputData[i];
    }

    double op1Result = myOperator1.doEx3(inputData[0], inputData[1]);
    cout << "Result of trained operator 1 (1HL, 2Ns): " << (thresholdWithNull(op1Result)?"true":"false") << "  (" << op1Result << ")" << endl; 
    double op2Result = myOperator2.doEx3(inputData[0], inputData[1]);
    cout << "Result of trained operator 2 (2HL, je 2Ns): " << (thresholdWithNull(op2Result)?"true":"false") << "  (" << op2Result << ")" << endl; 
    double op3Result = myOperator3.doEx3(inputData[0], inputData[1]);
    cout << "Result of trained operator 3 (1HL, 4Ns): " << (thresholdWithNull(op3Result)?"true":"false") << "  (" << op3Result << ")" << endl; 

    cout << endl << "Repeat? (y/n)  ";
    cin >> repeat;
    cout << endl;
  }	
  
  
  delete trainData;
  
  return 0;
}


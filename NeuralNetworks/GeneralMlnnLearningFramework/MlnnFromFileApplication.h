#pragma once

/* system includes */
#include <string>
#include <vector>
#include <Utilities/InputTargetDataSet.h>

using namespace std;
namespace FL = Flood;

struct GeneralData{
  FL::Vector<double> inputData;
  FL::Vector<double> outputData;

  GeneralData(int inputLength, int outputLength);
};

typedef vector<GeneralData> GeneralTrainData;

/*!
  \brief Load training data from a text file.

  Format of the file:
  Number of input signals
  Number of output signals
  inval_1, inval_2, ..., inval_numberOfInputSignals, outval_1, outval_2, ..., outval_numberOfOutputSignals

  where CLASS can be either '+' or '-'.

  \attention The returned data object must be freed by the caller!
  
  @param filename name of the text file with the data
  @return TrainData object containing the data from the file
*/
GeneralTrainData* loadGeneralDataFromFile(string filename);


/*
  \brief Transform data loaded from file to format used by MLNN learning algorithms.

  @param data training data in format as read from file (by loadGeneralDataFromFile)
  @return same training data in format for MLNN learning
*/
FL::InputTargetDataSet* generateTrainDataSet(GeneralTrainData* data);

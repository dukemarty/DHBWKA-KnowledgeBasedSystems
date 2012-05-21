#pragma once

/* system includes */
#include <string>
#include <vector>
#include <Utilities/InputTargetDataSet.h>

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

typedef vector<LabeledData> TrainData;


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


FL::InputTargetDataSet* generateTrainDataSet(TrainData* data);

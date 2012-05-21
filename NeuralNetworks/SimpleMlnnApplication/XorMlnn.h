#pragma once

#include <string>
#include <MultilayerPerceptron/MultilayerPerceptron.h>
#include <Utilities/InputTargetDataSet.h>

namespace FL = Flood;
using namespace std;

/*!
  \class XorMlnn
  \brief This class represents a multi-layer neural network for an XOR operator.
*/
class XorMlnn
{
private:
	FL::MultilayerPerceptron* _network;
	FL::Vector<int> _hiddenLayers;

	double _minImprovement;
	int _displayPeriod;
	int _epochs;

	void initTrainParameters();

public:
	//@{
	//! \name Constructors & Destructors
	XorMlnn(void);
	XorMlnn(FL::Vector<int> hiddenLayerConfig);
	XorMlnn(string filename);
	virtual ~XorMlnn(void);
	//@}

	void train(FL::InputTargetDataSet* trainData);
	void print();
	void save(string filename);
	double doXor(double input1, double input2);
};

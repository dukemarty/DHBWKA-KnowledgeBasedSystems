#include <ObjectiveFunctional/SumSquaredError.h>
#include <TrainingAlgorithm/GradientDescent.h>

#include "XorMlnn.h"

XorMlnn::XorMlnn(void)
{
	_hiddenLayers.resize(1);
	_hiddenLayers[0] = 2;

	_network = new FL::MultilayerPerceptron(2,_hiddenLayers,1);

	initTrainParameters();
}

XorMlnn::XorMlnn(FL::Vector<int> hiddenLayerConfig)
{
	_hiddenLayers = hiddenLayerConfig;

	_network = new FL::MultilayerPerceptron(2,_hiddenLayers,1);

	initTrainParameters();
}

XorMlnn::XorMlnn(string filename)
{
	_network = new FL::MultilayerPerceptron;

	_network->load((char*)filename.c_str());

	initTrainParameters();
}

void XorMlnn::initTrainParameters()
{
	_minImprovement = 1.0e-9;
	_displayPeriod = 10;
	_epochs = 500;
}

XorMlnn::~XorMlnn(void)
{
}

void XorMlnn::train(FL::InputTargetDataSet* trainData)
{
	// set-up training objects
	FL::SumSquaredError trainingErrorMeasure(_network, trainData);
	FL::GradientDescent gradientDescentLearner(&trainingErrorMeasure);

	// set gradient descent parameters
	gradientDescentLearner.set_maximum_epochs_number(_epochs);
	gradientDescentLearner.set_display_period(_displayPeriod);
	gradientDescentLearner.set_minimum_evaluation_improvement(_minImprovement);
	gradientDescentLearner.set_gradient_norm_goal(0.0);
	gradientDescentLearner.set_warning_gradient_norm(1e10);
	gradientDescentLearner.train();
}

void XorMlnn::print()
{
	_network->print();
}
void XorMlnn::save(string filename)
{
	_network->save((char*)filename.c_str());
}

double XorMlnn::doXor(double input1, double input2)
{
	FL::Vector<double> input(2);
	input[0] = input1;
	input[1] = input2;

	FL::Vector<double> results;
	if (_network){
		results = _network->calculate_output(input);
	} else {
		return 0.0;
	}

	return results[0];
}

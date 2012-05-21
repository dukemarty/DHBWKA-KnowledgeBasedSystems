// SimpleMlnnApplication.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include <iostream>

#include <MultilayerPerceptron/MultilayerPerceptron.h>
#include <Utilities/InputTargetDataSet.h>

#include "XorMlnn.h"

namespace FL = Flood;
using namespace std;

FL::InputTargetDataSet* generateTrainDataSet1()
{
	FL::InputTargetDataSet* res = new FL::InputTargetDataSet(4, 2, 1);

	FL::Matrix<double> trainMatrix(4, 3);
	FL::Vector<double> trainSample(3);
	
	trainSample[0] = 0.0;	trainSample[1] = 0.0;	trainSample[2] = -1.0;
	trainMatrix.set_row(0, trainSample);
	
	trainSample[0] = 1.0;	trainSample[1] = 0.0;	trainSample[2] = 1.0;
	trainMatrix.set_row(1, trainSample);

	trainSample[0] = 0.0;	trainSample[1] = 1.0;	trainSample[2] = 1.0;
	trainMatrix.set_row(2, trainSample);

	trainSample[0] = 1.0;	trainSample[1] = 1.0;	trainSample[2] = -1.0;
	trainMatrix.set_row(3, trainSample);

	res->set_data(trainMatrix);

	return res;
}

FL::InputTargetDataSet* generateTrainDataSet2()
{
	FL::InputTargetDataSet* res = new FL::InputTargetDataSet(8, 2, 1);

	FL::Matrix<double> trainMatrix(8, 3);
	FL::Vector<double> trainSample(3);
	
	trainSample[0] = 0.0;	trainSample[1] = 0.0;	trainSample[2] = -1.0;
	trainMatrix.set_row(0, trainSample);

	trainSample[0] = 1.0;	trainSample[1] = 0.0;	trainSample[2] = 1.0;
	trainMatrix.set_row(1, trainSample);

	trainSample[0] = 0.0;	trainSample[1] = 1.0;	trainSample[2] = 1.0;
	trainMatrix.set_row(2, trainSample);

	trainSample[0] = 0.9;	trainSample[1] = 0.8;	trainSample[2] = -1.0;
	trainMatrix.set_row(3, trainSample);

	trainSample[0] = 0.1;	trainSample[1] = 0.2;	trainSample[2] = -1.0;
	trainMatrix.set_row(4, trainSample);

	trainSample[0] = 0.9;	trainSample[1] = 0.2;	trainSample[2] = 1.0;
	trainMatrix.set_row(5, trainSample);

	trainSample[0] = 0.1;	trainSample[1] = 0.8;	trainSample[2] = 1.0;
	trainMatrix.set_row(6, trainSample);

	trainSample[0] = 1.0;	trainSample[1] = 1.0;	trainSample[2] = -1.0;
	trainMatrix.set_row(7, trainSample);
	res->set_data(trainMatrix);

	return res;
}

bool thresholdWithNull(double value)
{
	return (value > 0.0);
}

int main(int argc, char* argv[])
{
	cout << "Test Multilayer Neural Network class from Flood2 library!" << endl;

	FL::InputTargetDataSet* trainData1 = generateTrainDataSet1();
	FL::InputTargetDataSet* trainData2 = generateTrainDataSet2();

	FL::Vector<int> hiddenLayers2(2, 2); // short for setting each element to "2"
	FL::Vector<int> hiddenLayers1(1, 4);

	XorMlnn myOperator11, myOperator12(hiddenLayers2), myOperator13(hiddenLayers1);
	XorMlnn myOperator21, myOperator22(hiddenLayers2), myOperator23(hiddenLayers1);

	myOperator11.train(trainData1);
	myOperator12.train(trainData1);
	myOperator13.train(trainData1);
	cout << "Result of training the first network (1HL, 2Ns) with data set 1:" << endl << endl;
	myOperator11.print();
	cout << "Result of training the second network (2HL, je 2Ns) with data set 1:" << endl << endl;
	myOperator12.print();
	cout << "Result of training the third network (1HL, 4Ns) with data set 1:" << endl << endl;
	myOperator13.print();

	myOperator21.train(trainData2);
	myOperator22.train(trainData2);
	myOperator23.train(trainData2);
	cout << "Result of training the first network (1HL, 2Ns) with data set 2:" << endl << endl;
	myOperator21.print();
	cout << "Result of training the second network (2HL, je 2Ns) with data set 2:" << endl << endl;
	myOperator22.print();
	cout << "Result of training the third network (1HL, 4Ns) with data set 2:" << endl << endl;
	myOperator23.print();


	char repeat='y';
	double inputData[2];

	while (repeat != 'n' && repeat != 'N'){
		for (int i=0; i<2; i++){
			cout << "Value " << i << ":  ";
			cin >> inputData[i];
		}

		double op1Result = myOperator11.doXor(inputData[0], inputData[1]);
		cout << "Result of trained operator 1 (1HL, 2Ns): " << (thresholdWithNull(op1Result)?"true":"false") << "  (" << op1Result << ")" << endl; 
		double op2Result = myOperator12.doXor(inputData[0], inputData[1]);
		cout << "Result of trained operator 2 (2HL, je 2Ns): " << (thresholdWithNull(op2Result)?"true":"false") << "  (" << op2Result << ")" << endl; 
		double op3Result = myOperator13.doXor(inputData[0], inputData[1]);
		cout << "Result of trained operator 3 (1HL, 4Ns): " << (thresholdWithNull(op3Result)?"true":"false") << "  (" << op3Result << ")" << endl; 

		double op4Result = myOperator21.doXor(inputData[0], inputData[1]);
		cout << "Result of trained operator 4 (1HL, 2Ns): " << (thresholdWithNull(op4Result)?"true":"false") << "  (" << op4Result << ")" << endl; 
		double op5Result = myOperator22.doXor(inputData[0], inputData[1]);
		cout << "Result of trained operator 5 (2HL, je 2Ns): " << (thresholdWithNull(op5Result)?"true":"false") << "  (" << op5Result << ")" << endl; 
		double op6Result = myOperator23.doXor(inputData[0], inputData[1]);
		cout << "Result of trained operator 6 (1HL, 4Ns): " << (thresholdWithNull(op6Result)?"true":"false") << "  (" << op6Result << ")" << endl; 

		cout << endl << "Repeat? (y/n)  ";
		cin >> repeat;
		cout << endl;
	}	


	delete trainData1;
	delete trainData2;

	return 0;
}


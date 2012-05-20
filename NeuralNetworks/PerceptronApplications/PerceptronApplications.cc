// PerceptronApplications.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include <iostream>

#include <Perceptron/Perceptron.h>

namespace FL = Flood;
using namespace std;

FL::Perceptron *AndOp=NULL, *OrOp=NULL, *PrimArgOp=NULL;
FL::Perceptron xorUpperLeft(2), xorLowerRight(2), xorOut(2);
FL::Perceptron ex3Horiz(2), ex3Diag(2), ex3BlackCross(2), ex3GreenCircle(2), ex3BlueSquare(2);

/*!
	\brief Transform double value to boolean by thresholding with 0.
	
	@param value
	@return
*/
bool thresholdWithNull(double value)
{
  return (value > 0.0);
}

void initExercise3Perceptrons()
{
  ex3Horiz.set_activation_function(FL::Perceptron::Threshold);
  ex3Diag.set_activation_function(FL::Perceptron::Threshold);
  ex3BlackCross.set_activation_function(FL::Perceptron::Threshold);
  ex3GreenCircle.set_activation_function(FL::Perceptron::Threshold);
  ex3BlueSquare.set_activation_function(FL::Perceptron::Threshold);

  ex3Horiz.set_bias(1.5);
  FL::Vector<double> ex3hWeights(2);
  ex3hWeights[0] = 0.0;
  ex3hWeights[1] = -1.0;
  ex3Horiz.set_synaptic_weights(ex3hWeights);

  ex3Diag.set_bias(-2.25);
  FL::Vector<double> ex3dWeights(2);
  ex3dWeights[0] = 0.5;
  ex3dWeights[1] = 0.5;
  ex3Diag.set_synaptic_weights(ex3dWeights);

  ex3BlackCross.set_bias(-0.5);
  FL::Vector<double> ex3bcWeights(2);
  ex3bcWeights[0] = 1.0;
  ex3bcWeights[1] = 0.0;
  ex3BlackCross.set_synaptic_weights(ex3bcWeights);

  ex3GreenCircle.set_bias(0.25);
  FL::Vector<double> ex3gcWeights(2);
  ex3gcWeights[0] = -0.5;
  ex3gcWeights[1] = -0.5;
  ex3GreenCircle.set_synaptic_weights(ex3gcWeights);

  ex3BlueSquare.set_bias(-0.5);
  FL::Vector<double> ex3bsWeights(2);
  ex3bsWeights[0] = 0.0;
  ex3bsWeights[1] = 1.0;
  ex3BlueSquare.set_synaptic_weights(ex3bsWeights);
}

void initXorPerceptrons()
{
  xorUpperLeft.set_activation_function(FL::Perceptron::Threshold);
  xorLowerRight.set_activation_function(FL::Perceptron::Threshold);
  xorOut.set_activation_function(FL::Perceptron::Threshold);


  // init upper left perceptron
  xorUpperLeft.set_bias(-0.25);
  FL::Vector<double> xulWeights(2);
  xulWeights[0] = 0.5;
  xulWeights[1] = -0.5;
  xorUpperLeft.set_synaptic_weights(xulWeights);

  // init lower right perceptron
  xorLowerRight.set_bias(-0.25);
  FL::Vector<double> xlrWeights(2);
  xlrWeights[0] = -0.5;
  xlrWeights[1] = 0.5;
  xorLowerRight.set_synaptic_weights(xlrWeights);

  // init out perceptron
  xorOut.set_bias(-0.25);
  FL::Vector<double> xoWeights(2);
  xoWeights[0] = 0.5;
  xoWeights[1] = 0.5;
  xorOut.set_synaptic_weights(xoWeights);
}

/*!
	\brief Initialize perceptrons for standard operations 'AND', 'OR' and selection of first argument.
	
	@param numberOfInputSignals number of input signals
*/
void initOperatorPerceptrons(int numberOfInputSignals)
{
  AndOp = new FL::Perceptron(numberOfInputSignals);
  OrOp = new FL::Perceptron(numberOfInputSignals);
  PrimArgOp = new FL::Perceptron(numberOfInputSignals);

  // set activation functions
  AndOp->set_activation_function(FL::Perceptron::Linear);
  AndOp->set_activation_function(FL::Perceptron::Threshold);
  OrOp->set_activation_function(FL::Perceptron::Linear);
  PrimArgOp->set_activation_function(FL::Perceptron::Linear);

  // init AND for n arguments
  double andInputWeight = 1.0/numberOfInputSignals;
  cout << "And weight " << andInputWeight << endl;
  double andThreshold = 1.0 - (andInputWeight / 2.0);
  cout << "And threshold " << andThreshold << endl;
  AndOp->set_bias(-andThreshold);
  FL::Vector<double> weights(numberOfInputSignals);
  for (int i=0; i<numberOfInputSignals; i++){
    weights[i] = andInputWeight;
  }
  AndOp->set_synaptic_weights(weights);

  // init OR for n arguments
  double orInputWeight = 1.0/numberOfInputSignals;
  double orThreshold = orInputWeight / 2;
  OrOp->set_bias(-orThreshold);
  for (int i=0; i<numberOfInputSignals; i++){
    weights[i] = orInputWeight;
  }
  OrOp->set_synaptic_weights(weights);

  // init PROJ of 1st Argument for n arguments
  PrimArgOp->set_bias(0.0);
  weights[0] = 1.0;
  for (int i=1; i<numberOfInputSignals; i++){
    weights[i] = 0.0;
  }
  PrimArgOp->set_synaptic_weights(weights);
}

void closeOperatorPerceptrons()
{
  delete AndOp;
  AndOp = NULL;

  delete OrOp;
  OrOp = NULL;

  delete PrimArgOp;
  PrimArgOp = NULL;
}

/*!
	\brief Calculate 'and' operation with pre-configured perceptron.
	
	@param numberOfSignals number of input signals the perceptron accepts (and are given as second parameter)
	@param signals array of input values for the perceptron
	@return output value of the global <tt>AndOp</tt> perceptron
*/
double calculateAnd(int numberOfSignals, double* signals)
{
  FL::Vector<double> inputSignal(numberOfSignals);

  for (int i=0; i<numberOfSignals; i++){
    inputSignal[i] = signals[i];
  }

  double outputSignal = AndOp->calculate_output(inputSignal);

  return outputSignal;
}

double calculateOr(int numberOfSignals, double* signals)
{
  FL::Vector<double> inputSignal(numberOfSignals);

  for (int i=0; i<numberOfSignals; i++){
    inputSignal[i] = signals[i];
  }

  double outputSignal = OrOp->calculate_output(inputSignal);

  return outputSignal;
}

double calculatePrimArg(int numberOfSignals, double* signals)
{
  FL::Vector<double> inputSignal(numberOfSignals);

  for (int i=0; i<numberOfSignals; i++){
    inputSignal[i] = signals[i];
  }

  double outputSignal = PrimArgOp->calculate_output(inputSignal);

  return outputSignal;
}

double calculateXor(double* signals)
{
  FL::Vector<double> inputSignal(2);

  inputSignal[0] = signals[0];
  inputSignal[1] = signals[1];
  double upperLeftSignal = xorUpperLeft.calculate_output(inputSignal);

  inputSignal[0] = signals[0];
  inputSignal[1] = signals[1];
  double lowerRightSignal = xorLowerRight.calculate_output(inputSignal);

  std::cout << "XOR-Intermediate:  ul=" << upperLeftSignal << "    lr=" << lowerRightSignal << std::endl;

  // inputSignal[0] = (upperLeftSignal>0.0?1.0:0.0);
  // inputSignal[1] = (lowerRightSignal>0.0?1.0:0.0);
  	inputSignal[0] = upperLeftSignal;
  	inputSignal[1] = lowerRightSignal;
	
  double outputSignal = xorOut.calculate_output(inputSignal);

  return outputSignal;
}

void calculateExercise3(double* signals)
{
  FL::Vector<double> inputSignals(2);
	
  inputSignals[0] = signals[0];	inputSignals[1] = signals[1];
  double horizRes = ex3Horiz.calculate_output(inputSignals);
  
  inputSignals[0] = signals[0];	inputSignals[1] = signals[1];
  double diagRes = ex3Diag.calculate_output(inputSignals);

  cout << "Horiz-Res=" << horizRes << "      Diag-Res=" << diagRes << endl;

  inputSignals[0] = horizRes;		inputSignals[1] = diagRes;
  double bcRes = ex3BlackCross.calculate_output(inputSignals);

  inputSignals[0] = horizRes;		inputSignals[1] = diagRes;
  double gcRes = ex3GreenCircle.calculate_output(inputSignals);

  inputSignals[0] = horizRes;		inputSignals[1] = diagRes;
  double bsRes = ex3BlueSquare.calculate_output(inputSignals);

  cout << "Exercise 3:  BlackCross=" << (thresholdWithNull(bcRes)?"true":"false") << "   GreenCircle=" << (thresholdWithNull(gcRes)?"true":"false") << "   BlueSquare=" << (thresholdWithNull(bsRes)?"true":"false") << endl;
}

int main(int argc, char* argv[])
{
  cout << "Test Perceptron class from Flood3 library!" << endl;

  int numberOfInputs=0;
  cout << "How much input signals do you want?  ";
  cin >> numberOfInputs;

  initOperatorPerceptrons(numberOfInputs);
  initXorPerceptrons();
  initExercise3Perceptrons();

  char repeat='y';
  double* inputData = new double[numberOfInputs];

  while (repeat != 'n' && repeat != 'N'){
    for (int i=0; i<numberOfInputs; i++){
      cout << "Value " << i << ":  ";
      cin >> inputData[i];
    }

    double andResult = calculateAnd(numberOfInputs, inputData);
    cout << "Result of perceptron-AND: " << (thresholdWithNull(andResult)?"true":"false") << "  (" << andResult << ")" << endl; 
    double orResult = calculateOr(numberOfInputs, inputData);
    cout << "Result of perceptron-OR: " << (thresholdWithNull(orResult)?"true":"false") << "  (" << orResult << ")" << endl; 
    // double primargResult = calculatePrimArg(numberOfInputs, inputData);
    // cout << "Result of perceptron-Prim-Arg: " << (thresholdWithNull(primargResult)?"true":"false") << "  (" << primargResult << ")" << endl;

    if (numberOfInputs==2){
      double xorResult = calculateXor(inputData);
      cout << "Result of perceptron-XOR: " << (thresholdWithNull(xorResult)?"true":"false") << "  (" << xorResult << ")" << endl; 

      calculateExercise3(inputData);
    }	

    cout << endl << "Repeat? (y/n)  ";
    cin >> repeat;
    cout << endl;
  }	

  delete[] inputData;
  closeOperatorPerceptrons();

  return 0;
}


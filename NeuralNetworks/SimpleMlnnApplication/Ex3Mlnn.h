#pragma one

#include <string>
#include <MultilayerPerceptron/MultilayerPerceptron.h>
#include <Utilities/InputTargetDataSet.h>

namespace FL = Flood;
using namespace std;

class Ex3Mlnn
{
private:
	FL::MultilayerPerceptron* _network;
	FL::Vector<int> _hiddenLayers;

	double _minImprovement;
	int _displayPeriod;
	int _epochs;

	void initTrainParameters();

public:
	Ex3Mlnn(void);
	Ex3Mlnn(FL::Vector<int> hiddenLayerConfig);
	Ex3Mlnn(string filename);
	virtual ~Ex3Mlnn(void);

	void train(FL::InputTargetDataSet* trainData);
	void print();
	void save(string filename);
	double doEx3(double input1, double input2);
};

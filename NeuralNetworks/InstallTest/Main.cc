#include <iostream>

#include <Perceptron/Perceptron.h>


using namespace std;

int main(int argc, char* argv[])
{
//   cout << "Test Perceptron classes from Flood3 library!" << endl;
  
//   Flood::Perceptron p;
//   Flood::Vector<double> v(2);

//   v[0] = 1.0;
//   v[1] = 13.2;
  
//   cout << " v[0] = " << v[0] << "      v[1] = " << v[1] << endl;

  // initialize perceptron
  Flood::Perceptron p(2);
//   p.set_activation_function(Flood::Perceptron::Threshold);

  // set weights
  Flood::Vector<double> weights(2);
  weights[0] = 1.0;
  weights[1] = 1.0;
  p.set_synaptic_weights(weights);
  
  // set threshold
  p.set_bias(-1.5);

  // testen
  Flood::Vector<double> query(2);
  double res=-1.0;
  
  query[0] = 0.0;   query[1] = 0.0;
  res = p.calculate_output(query);
  cout << query[0] << " & " << query[1] << "  ->  " << res << endl;
  
  query[0] = 1.0;   query[1] = 0.0;
  res = p.calculate_output(query);
  cout << query[0] << " & " << query[1] << "  ->  " << res << endl;

  query[0] = 0.0;   query[1] = 1.0;
  res = p.calculate_output(query);
  cout << query[0] << " & " << query[1] << "  ->  " << res << endl;

  query[0] = 0.8;   query[1] = 0.84;
  res = p.calculate_output(query);
  cout << query[0] << " & " << query[1] << "  ->  " << res << endl;

  return 0;
}


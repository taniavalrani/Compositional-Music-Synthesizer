#include "rhythm.cpp"
#include "type1.hpp"
#include "type2.hpp"
double fuzzy_var_engine(std::string INPUT);

int main(int argc, char *argv[])
{
  int type = 1;
  
  Type1 *pattern1 = new Type1;
  Type2 *pattern2 = new Type2;

  std::vector<std::string> args(argv + 1, argv + argc);
  for (std::vector<std::string>::iterator i = args.begin(); i != args.end(); ++i) {
    if (*i == "-var" || *i == "--variance" ) {
      std::string input = *++i;
      std::transform(input.begin(), input.end(), input.begin(), ::toupper);
      double var = fuzzy_var_engine(input);
      pattern1->set_variance(var);
      pattern2->set_variance(var);
    }
    if (*i == "-t" || *i == "--type" )
      type = stoi(*++i);
  }

  if (type == 1 || type == 3) {
    pattern1->build();
    pattern1->print();
    delete pattern1;
  }

  if (type == 2 || type == 3) {
    pattern2->build();
    pattern2->print();
    delete pattern2;
  }
}

double fuzzy_var_engine(std::string INPUT) {
  using namespace fl;
  Engine* engine = FllImporter().fromFile("varengine.fll");

  std::string status;
  if (not engine->isReady(&status))
    throw Exception("[engine error] engine is not ready:n" + status, FL_AT);

  InputVariable* form = engine->getInputVariable("form");
  OutputVariable* variance = engine->getOutputVariable("variance");

  std::default_random_engine rndgen;
  scalar location;

  if (INPUT == "EXPOSITION") {
    std::uniform_real_distribution<double> distribution(0.001,0.400);
    location = distribution(rndgen);
  } else if (INPUT == "DEVELOPMENT") {
    std::uniform_real_distribution<double> distribution(0.300,0.999);
    location = distribution(rndgen);
  } else if (INPUT == "RECAPITULATION") {
    std::uniform_real_distribution<double> distribution(0.150,0.700);
    location = distribution(rndgen);
  } else {
    std::cerr << "Unknown value: " << INPUT << std::endl;
    exit(EXIT_FAILURE);
  }

  
  form->setValue(location);
  engine->process();
  double VARIANCE = stod(Op::str(variance->getValue()));
  // std::cout << "location: " << location << "variance: "
  // 	    << VARIANCE << std::endl;

  return VARIANCE;

}

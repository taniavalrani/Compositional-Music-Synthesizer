#include <iostream>
#include <random>
#include <iomanip>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <string>
#include <set>
#include <numeric>
#include <map>
#include <locale>
#include "fl/Headers.h"

typedef std::vector<int>::iterator vecit;
typedef std::map<std::string, std::vector<int>>::iterator gmapit;
typedef std::map<std::string, std::vector<int>> genmap;
typedef std::set<int, std::less<int>> bset;

namespace Rhythm {
  std::string input_ratio(char& iDelim) {
    std::string iFull;
    while (1) {
      std::cout << "Enter a ratio in the format 'x:y[:z:...]': ";
      std::cin >> iFull;
      int loc = iFull.find(iDelim);
      if (loc == -1 ) {
	std::cerr << "Must be a valid ratio of the form 'x:y[:z:...]'."
		  << std::endl;
	continue;
      }
      return iFull;
    }    
  }

  std::vector<int> populate_locations(std::string& iFull, char& iDelim) {
    std::vector<int> iLocations;
    for (int i = 0; i < iFull.size(); ++i) {
      if (iFull[i] == iDelim)
	iLocations.push_back(i);
    }
    return iLocations;
  }

  template <class T>
  void format_print(T inp_vec, int cp) {
    for (int i = 0; i < cp; ++i) {
      if (std::binary_search(inp_vec.begin(), inp_vec.end(), i))
	std::cout << std::setw(5) << "X";
      else
	std::cout << std::setw(5) << "";
    }
  }

  std::vector<int> split(std::string phrase, std::string delimiter) {
    std::vector<int> num_list;
    size_t pos = 0;
    std::string token;
    while ((pos = phrase.find(delimiter)) != std::string::npos) {
      token = phrase.substr(0,pos);
      num_list.push_back(stoi(token));
      phrase.erase(0, pos + delimiter.length());
    }
    num_list.push_back(stoi(phrase.substr(0,phrase.back())));

    return num_list;
  }

  void print_generator_map(genmap& generator_map) {
    for (gmapit i = generator_map.begin(); i != generator_map.end(); ++i) {
      std::cout << i->first << ": ";
      for (vecit j = i->second.begin(); j != i->second.end(); ++j)
	std::cout << *j << " ";
      std::cout << std::endl;    
    }
  }

  bset build_set(genmap& generator_map) {
    bset beat_set;
    for (auto i: generator_map)
      for (vecit j = i.second.begin(); j != i.second.end(); ++j)
	beat_set.insert(*j);
       
    return beat_set;
  }

  std::vector<int> build_resultants(bset& beat_set) {
    std::vector<int> resultants;

    std::adjacent_difference(beat_set.begin(), beat_set.end(), std::back_inserter(resultants));
    resultants.erase(resultants.begin());

    return resultants;
  }

  void print_graph(genmap generator_map, bset beat_set, int cp) {
    std::cout << std::setw(3) << "CD:";
    for (int i = 0; i < cp; ++i)
      std::cout << std::setw(5) << "X";

    for (auto i: generator_map) {
      std::cout << std::endl << std::setw(2) << i.first << ":";
      Rhythm::format_print(i.second, cp);
    }

    std::cout << std::endl << std::setw(3) << "r:";
    Rhythm::format_print(beat_set, cp);

    std::cout << std::endl;
  }

  namespace Rules {
    int randomize_variance(std::default_random_engine rndgen, int i, int j,
			   double variance, int cp) {
      std::uniform_int_distribution<int> distribution(-i,cp);
      int val = distribution(rndgen);
      if (j == 0 && val < 0) val = 0;
      double ratio_cast = (double) val * variance;
      if (ratio_cast > 0) ratio_cast += .5;
      else ratio_cast -= .5;
      int rnd = (int)ratio_cast;      
      // std::cout << "i: " << i << " val: " << val
      // 		<< " ratio_cast: " << ratio_cast << " rnd: " << rnd
      // 		<< " cp: " << cp << std::endl;
      return j + rnd;
    }
  }
}

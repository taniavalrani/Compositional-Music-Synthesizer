class Type1
{
public:
  Type1() {
    set_cp(0);
  }

  Type1(int num) {
    set_cp(num);
  }
     
  ~Type1() { }

  // Virtual functions
  virtual void build();
  virtual void print();

  // Functions   
  std::string get_ratio() const {
    return full;
  }
  void set_cp (int num) {
    cp = num;
  }
  int get_cp () const {
    return cp;
  }
  void set_variance(double var) {
    variance = var;
  }
  double get_variance() const {
    return variance;
  }
  
private:
  // Functions
  void calculate_ratio();
  void build_map();
  void add_to_set(std::vector<int> inp_vec);

  void print_resultants() {
    for (vecit i = resultants.begin(); i != resultants.end(); ++i)
      std::cout << *i;
  
    std::cout << std::endl;
  }
  genmap populate_generator_map(std::vector<int>& nums, int cp);

  // Data
  std::vector<int> cd, locations, ratios, resultants;
  int cp;
  double variance;
  std::string full;
  bset beat_set;
  genmap generator_map;

};

void Type1::build() {
  calculate_ratio();
  build_map();
  beat_set = Rhythm::build_set(generator_map);
  resultants = Rhythm::build_resultants(beat_set);
}

void Type1::print() {
  std::cout << "Ratio: " << get_ratio() << std::endl;
  std::cout << std::endl;
  Rhythm::print_generator_map(generator_map);
  std::cout << std::endl;
  Rhythm::print_graph(generator_map, beat_set, cp);
  std::cout << std::endl;
  print_resultants();
  std::cout << std::endl;
}
  
void Type1::build_map() {

  std::vector<int> nums;

  nums = Rhythm::split(full, ":");

  if (cp == 0) {
    cp = 1;
    for (vecit i = nums.begin(); i != nums.end(); ++i)
      cp *= *i;
  }

  generator_map = populate_generator_map(nums, cp);
}

void Type1::calculate_ratio() {
  char delim = ':';

  full = Rhythm::input_ratio(delim);
  locations = Rhythm::populate_locations(full, delim);
}
      
genmap Type1::populate_generator_map(std::vector<int>& nums, int cp) {
  std::string l = "a";
  genmap generator_map;
  std::default_random_engine rndgen;

  for (vecit i = nums.begin(); i != nums.end(); ++i) {
    std::vector<int> generator;
    for (int j = 0; j <= cp; j += *i) {
      if (variance != 0) {
	int var = Rhythm::Rules::randomize_variance(rndgen, *i, j, variance, cp);
	generator.push_back(var);
      }
      else
	generator.push_back(j);      
    }
    std::sort (generator.begin(), generator.end());
    
    generator_map[l] = generator;
    std::transform(l.begin(), l.end(), l.begin(),
		   [](char i) -> char { return ++i; });
  }
  return generator_map;
}

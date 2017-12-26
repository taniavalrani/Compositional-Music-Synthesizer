class Type2
{
public:

  Type2() { }

  Type2(int num) {
    set_cp(num);
  }

  ~Type2() { }

  // Virtual Functions
  virtual void build();
  virtual void print();

  // Functions
  void set_cp(int num) {
    cp = num;
  }
  int get_cp() const {
    return cp;
  }
  std::string get_ratio() const {
    return full;
  }
  void set_variance(double var) {
    variance = var;
  }
  double get_variance() const {
    return variance;
  }
  
  void build_map();

private:
  // Functions
  void calculate_ratio();
  genmap populate_generator_map(std::vector<int>& nums, int cp);
  void print_resultants() {
    for (vecit i = resultants.begin(); i != resultants.end(); ++i)
      std::cout << *i;
  
    std::cout << std::endl;
  }

  // Data
  int cp, max;
  std::string full;
  std::vector<int> locations, resultants;
  double variance;
  bset beat_set;
  genmap generator_map;
};

void Type2::build() {
  calculate_ratio();
  build_map();
  beat_set = Rhythm::build_set(generator_map);
  resultants = Rhythm::build_resultants(beat_set);
}

void Type2::print() {
  std::cout << "Ratio: " << get_ratio() << std::endl;
  std::cout << std::endl;
  Rhythm::print_generator_map(generator_map);
  std::cout << std::endl;
  Rhythm::print_graph(generator_map, beat_set, cp);
    print_resultants();
  std::cout << std::endl;

}

void Type2::calculate_ratio() {
  char delim = ':';

  full = Rhythm::input_ratio(delim);
  locations = Rhythm::populate_locations(full, delim);
}

void Type2::build_map() {

  std::vector<int> nums;

  nums = Rhythm::split(full, ":");

  if (cp == 0) {
    max = *std::max_element(nums.begin(), nums.end());
    cp = max * max;
  }

  generator_map = populate_generator_map(nums, cp);
}

genmap Type2::populate_generator_map(std::vector<int>& nums, int cp) {
  std::string l = "a";
  genmap generator_map;
  std::default_random_engine rndgen;
  int iter = 1, itercmp = 0;
  
  for (auto m: nums) iter *= m;

  for (vecit i = nums.begin(); i != nums.end(); ++i) {
    std::vector<int> generator;

    if (*i != max && max % *i != 0) {
      std::vector<std::vector<int>> gens;
      int niter = max - *i + 1;
      for (int j = 0; j < niter; ++j) {
	std::vector<int> x;
	for (int k = 0; k < max; ++k) {
	  int n = (k * *i) + (j * max);
	  if (variance != 0) {
	    int var = Rhythm::Rules::randomize_variance(rndgen, *i, n, variance, cp);
	    x.push_back(var);
	  } else
	    x.push_back(n);
	  gens.push_back(x);
	}

	int gen_num = 1;
	for (auto v: gens) {
	  std::string lx = l + std::to_string(gen_num);
	  generator_map[lx] = v;
	  ++gen_num;
	}
      }
    } else {
	for (int j = 0; j <= cp; j += *i) {
	  if (variance != 0) {
	    int var = Rhythm::Rules::randomize_variance(rndgen, *i, j, variance, cp);
	    generator.push_back(var);
	  } else
	    generator.push_back(j);
      generator_map[l] = generator;
	}
    }
    std::transform(l.begin(), l.end(), l.begin(),
		   [](char i) -> char { return ++i; });
  }
  return generator_map;
}

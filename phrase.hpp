class Phrase : public Score
{
 public:
  Phrase() {
    set_gap_weight(500);
    set_length_weight(600);
    set_optimal_length(10.0);
  }

  ~Phrase() { }

  // Virtual Functions
  virtual void build();
  virtual void print();

  // Getters
  void print_gaps() const;
  int get_gap_weight() const {
    return gap_weight;
  }
  int get_length_weight() const {
    return length_weight;
  }
  double get_optimal_length() const {
    return optimal_length;
  }

  // Setters
  void set_gap_weight(int iGap_weight) {
    gap_weight = iGap_weight;
  }
  void set_length_weight(int iLength_weight) {
    length_weight = iLength_weight;
  }
  void set_optimal_length(double iOptimal_length) {
    optimal_length = iOptimal_length;
  }
  
 private:
  // Functions
  void calculate_gap_scores();
  void analyze();
  void build_note_list();
  void print_gaps();
  void print_final();

  // Data
  Util::Quantization event_lengths;
  std::vector<int> gaps;
  int gap_weight, length_weight;
  double optimal_length;
  /* std::deque<int> final (count); */
  std::deque<int> final;
};

// Virtual functions
void Phrase::build() {
  build_note_list();
  calculate_gap_scores();
  analyze();
}

void Phrase::print() {
  print_gaps();
  print_final();
}

// Getters
void Phrase::print_gaps() {
  Util::Quantization::iterator i;
  std::vector<int>::iterator j;

  for (i = event_lengths.begin(), j = gaps.begin(); i != event_lengths.end() ||
	 j != gaps.end(); ++i, ++j) {
    std::cout << i->first << "\t" << i->second << "\t" << "Gap Score: "
	      << *j << std::endl;
  }
}

void Phrase::print_final() {
  std::cout << "Final array:\n";
  for (int z = 0; z <= count-1; z++) {
    std::cout << final[z] << "\n";
    if (final[z] == 1) {
      std::cout << "Phrase at " << z << "\n";
    }
  }
}

// Private Functions
void Phrase::build_note_list() {

  std::vector<int> phrase_onsets;
  std::vector<int> phrase_offsets;  
  
  for (const auto &arr: note_list) {
      phrase_onsets.push_back(arr.front());
      phrase_offsets.push_back(arr.at(1));
  }

  int start = Util::normalize_vector(phrase_onsets);
  if (start != 0)
    for (std::vector<int>::iterator i = phrase_offsets.begin();
	 i != phrase_offsets.end(); ++i)
      *i -= start;

  std::vector<int>::iterator i,j;

  for (i = phrase_onsets.begin(), j = phrase_offsets.begin();
       i != phrase_onsets.end() || j != phrase_offsets.end(); ++i, ++j)
    event_lengths.insert(Util::Quantization::value_type(*i, *j));

  phrase_onsets = phrase_offsets = std::vector<int>();

}

void Phrase::calculate_gap_scores() {
  for (Util::Quantization::iterator i = event_lengths.begin(); i != event_lengths.end();
       ++i) {
    int index = std::distance(event_lengths.begin(), i);
    if (index == 0) 
      gaps.push_back(0);
    else {
      int average = i->first / index+1;
      
      int gap = gap_weight * ((i->first - std::prev(i)->first) +
			      (i->first - std::prev(i)->second)) / average;

      gaps.push_back(gap);
    }
  }
}

void Phrase::analyze() {
  double best_score;
  int length_penalty, phase_penalty = 0;
  int best[10000];
  double analysis[10000];   
  std::vector<std::vector<double>> score = Util::Matrix(10000, 20);
  best[0]=0;
  analysis[0]=0.0;
  for (int i=1; i < count; i++) {
    best_score = -1000000.0;
    for (int j = 1; i - j >= 0 && j < 20; j++) {
      length_penalty = fabs(length_weight * (LOG2(j/optimal_length)));

      score[i][j]=analysis[i-j] + ((( (gaps[i-j] / 2) + (gaps[i] / 2) -
    				      length_penalty) - phase_penalty) * sqrt(j));
      if (score[i][j]>best_score) {
    	best_score=score[i][j];
    	analysis[i]=score[i][j];
    	best[i]=j;
      }
    }
    std::cout << "The best j for note " << i << " is " << best[i] << " with score "
	      << analysis[i] << ".\n";
  }

  final.resize(count);
  
  final.pop_back(); final.push_back(1);
  int z=count-1;
  while(z>0) {
    final[z-best[z]]=1;
    z=z-best[z];
  }
  final.assign(0,1);
}

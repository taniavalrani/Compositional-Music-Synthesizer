class Tactus : public Score
{
 public:
  Tactus() {
    set_meter("4/4");
    set_tempo(120);
    set_tactus_range(400, 1600);
    set_tactus_above(0);
    set_tactus_below(0);
    set_pip_time(35);
  }
  Tactus(std::string meter, int inTempo) {
    set_meter(meter);
    set_tempo(inTempo);
  }  
  
  ~Tactus() {};

  // Virtual Functions
  virtual void build() {
    build_note_list();
    calculate();
  }
  virtual void print ();

  // Getters
  void write_meter() const;
  void write_tempo() const { std::cout << tempo << std::endl; }
  void write_tactus_range() const {
    std::cout << "(" <<tactus_range.first << ", "
	      << tactus_range.second << ")" << std::endl;
  }
  void write_pip_deque() const;
  void write_pip_scores() const;

  // Setters
  void set_meter(const std::string& meter);
  void set_tempo(int inTempo) { tempo = inTempo; }
  void set_tactus_range(int low, int high) {
    tactus_range = std::make_pair (low,high);
  }      
  void set_tactus_above(int above) { tactus_above = above; }
  void set_tactus_below(int below) { tactus_below = below; }
  void set_pip_time(int time) { pip_time = time; }
  

 protected:
  // Functions
  void fill_tactus(int start, int tactus_interval);
  void calculate();
  int starting_point(int qtempo);
  int calculate_tactus_interval();
  void score_pips();
  void quantize_list(int last);
  void build_pip_deque(int last);
  void build_note_list();  

  // Data
  int bpm;
  int beat_value;
  int tempo;
  std::pair<int,int> tactus_range;
  int beat_interval;
  Util::Quantization_map tactus;
  int tactus_above;
  int tactus_below;
  std::vector<int> meter_vector;
  std::deque<int> pip_deque;
  Util::Quantization quantized_meter, event_lengths;  
  std::map<int,int> pip_scores;
  int pip_time;

};

void Tactus::print() {
  std::cout << "\nTactus:" << std::endl;
  std::cout << std::left;
  for (const auto pos: tactus) {
    std::cout << std::setw(15) << "Time: " << std::setw(15) << pos.first
	      << std::setw(15) << "Level: " << std::setw(15) <<pos.second
	      << std::endl;
  }
}

void Tactus::set_meter(const std::string& meter) {
  std::string::size_type pos = meter.find('/');
  if (meter.npos != pos) {
    bpm = stoi(meter.substr(0, pos));
    beat_value = stoi(meter.substr(pos + 1));    
  }  
}

void Tactus::write_meter() const {
  std::cout << bpm << "/" << beat_value << std::endl;
}

void Tactus::calculate() {
  int quantized_tempo = Util::Quantize_Elm(tempo, pip_deque);
  
  // Attempt to determine if the tactus falls somewhere else (pickup measures)
  int start = starting_point(quantized_tempo);

  // Determine tactus interval
  int tactus_interval = calculate_tactus_interval();
  
  // Fill tactus map
  fill_tactus(start, tactus_interval);   
}

void Tactus::fill_tactus(int start, int tactus_interval) {
    while (tactus_below > 0) {
    for (int i = start; i <= pip_deque.back(); i += (tactus_interval /
						     (2 * tactus_below))) {
      int quantized_interval = Util::Quantize_Elm(i, pip_deque);
      tactus[quantized_interval]++;
    }

    tactus_below--;
  }	 
      
  for (int i = start; i <= pip_deque.back(); i += tactus_interval) {
    int quantized_interval = Util::Quantize_Elm(i, pip_deque);
    //tactus.insert(Quantization_map::value_type(quantized_interval, 2));
    tactus[quantized_interval]++;
  }

  while (tactus_above > 0) {
    for (int i = start; i <= pip_deque.back(); i += (tactus_interval *
						     (2 * tactus_above))) {
      int quantized_interval = Util::Quantize_Elm(i, pip_deque);
      tactus[quantized_interval]++;
    }

    tactus_above--;
  }
}

int Tactus::calculate_tactus_interval() {
  double interval = (60. / tempo) * 1000.;
  // int quantized_interval = Util::Quantize_Elm(interval, pip_deque);
  //  std::cout << "interval: " << quantized_interval << std::endl;

  if (interval >= tactus_range.first &&
      interval <= tactus_range.second)
    return interval;
  else
    return interval * 2;

}
  
  

int Tactus::starting_point(int qtempo) {
  std::vector<int> possible_starting_points;

  for (std::map<int,int>::iterator i = pip_scores.begin();
       i->first <= (qtempo * (bpm * (bpm / beat_value)));
       ++i) {
    if (i->second > pip_scores.begin()->second) 
      possible_starting_points.push_back(i->first);
  }
  
  if (possible_starting_points.empty())
    return pip_deque.front();
  else
    return possible_starting_points.front();
}

void Tactus::write_pip_deque() const {
  for (const auto &pip: pip_deque)
    std::cout << pip << std::endl;
}

void Tactus::write_pip_scores() const {
  for (const auto pos: pip_scores) {
    std::cout << "Pip: " << std::setw(10) << pos.first << std::setw(10)
	      << "Score: " << std::setw(5) << pos.second
	      << std::endl;
  }
}

void Tactus::quantize_list(int last) {
  int last_pip = last;
  build_pip_deque(last_pip);

  int i = 0;
  int h;  

  for (const auto &time: meter_vector) {
    int val = Util::Quantize_Elm(time, pip_deque);
    quantized_meter.insert(Util::Quantization::value_type(val, time));
  }            

}
	
void Tactus::build_pip_deque(int last) {
  int last_pip = last;

    if ((last_pip % pip_time) != 0) {
      build_pip_deque(++last_pip);
    }
    else {
      for (int i = last_pip; i >= 0; i -= pip_time)
	pip_deque.push_front(i);
    }
}    


void Tactus::score_pips() {
  for (const auto &pip: pip_deque) {
    int n = quantized_meter.count(pip);
    pip_scores[pip] = n;
  }
}

void Tactus::build_note_list() {
  
  for (const auto &arr: note_list)
    meter_vector.push_back(arr.front());


  if (meter_vector.front() != 0)
    int start = Util::normalize_vector(meter_vector);

  quantize_list(meter_vector.back());

  score_pips();

}

  
    

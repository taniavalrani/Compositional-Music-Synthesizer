class Meter : public Score
{
public:
  Meter() { pip_time = 35; }
  Meter(int time) : pip_time(time) { pip_time = time; };
  ~Meter() {};
  virtual void build_note_list();
  void write_meter_vector() const;
  void write_pip_deque() const;
  void write_quantized_meter() const;
  void write_pip_scores() const;
  void set_pip_time(int time) { pip_time = time; }

protected:
  std::vector<int> meter_vector;
  std::deque<int> pip_deque;
  Quantization quantized_meter, event_lengths;  
  std::map<int,int> pip_scores;
  int pip_time;
  void score_pips();
  int normalize_vector(std::vector<int>& shift_vec);
  void quantize_list(int last);
  void build_pip_deque(int last);

};  

void Meter::build_note_list() {
  
  for (const auto &arr: note_list)
    meter_vector.push_back(arr.front());


  if (meter_vector.front() != 0)
    int start = normalize_vector(meter_vector);

  quantize_list(meter_vector.back());

  score_pips();

}

int Meter::normalize_vector(std::vector<int>& shift_vec) {
  // Shift the first value to 0,
  // bring along every other value
  
  int diff = shift_vec.front();
  
  std::transform (shift_vec.begin(), shift_vec.end(),
		  shift_vec.begin(),
		  [&](int el) {		    
		    return el - diff;
		  });
  return diff;
}

void Meter::quantize_list(int last) {
  int last_pip = last;
  build_pip_deque(last_pip);

  int i = 0;
  int h;  

  for (const auto &time: meter_vector) {
    int val = Util::Quantize_Elm(time, pip_deque);
    quantized_meter.insert(Quantization::value_type(val, time));
  }            

}
	
void Meter::build_pip_deque(int last) {
  int last_pip = last;

    if ((last_pip % pip_time) != 0) {
      build_pip_deque(++last_pip);
    }
    else {
      for (int i = last_pip; i >= 0; i -= pip_time)
	pip_deque.push_front(i);
    }
}    


void Meter::score_pips() {
  for (const auto &pip: pip_deque) {
    int n = quantized_meter.count(pip);
    pip_scores[pip] = n;
  }
}

void Meter::write_pip_scores() const {
  for (const auto pos: pip_scores) {
    std::cout << "Pip: " << std::setw(10) << pos.first << std::setw(10)
	      << "Score: " << std::setw(5) << pos.second
	      << std::endl;
  }
}

void Meter::write_meter_vector() const {
  for (const auto &el: meter_vector)
    std::cout << el << std::endl;
}

void Meter::write_pip_deque() const {
  for (const auto &pip: pip_deque)
    std::cout << pip << std::endl;
}

void Meter::write_quantized_meter() const {
  std::cout << std::left;
  for (const auto pos: quantized_meter) {
    std::cout << std::setw(15) << "Quantized: " << std::setw(15) << pos.first
	      << std::setw(15) << "Unquantized: " << std::setw(15) <<pos.second
	      << std::endl;
  }
}

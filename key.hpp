// Based on the K-S algorithm (Though not entirely)

class Key : public Score
{
public:
  Key() {
    for (int i = 0; i < 12; i++)
      pitches[i] = 0;
    build_input_map();
    major = { 5., 2., 3.5, 2., 4.5, 4., 2., 4.5, 2., 3.5, 1.5, 4. };
    minor = { 5., 2., 3.5, 4.5, 2., 4., 2., 4.5, 3.5, 2., 1.5, 4. };
  }  
  ~Key() { }

  // Virtual functions:
  // virtual void build();
  // virtual void print();
  
  // Functions:
  void compare_profile();
  
  // Getters
  void print_pitches() const {
    for (const auto &pitch: pitches)
      std::cout << pitch.first << ": " << pitch.second << std::endl;
  }
  void print_key() const { std::cout << key << std::endl; }
  
protected:
  // Functions
  void build_input_map();
  

  // Data
  std::map<int,double> pitches;
  std::deque<double> pitch_scores;
  std::vector<double> major, minor, maj_score_vector, min_score_vector;
  std::string key;

};

void Key::build_input_map() {
  // for (const auto &arr: note_list)
  //   pitches.push_back(arr.at(2) % 12);
  for (const auto &arr: note_list) {
    int duration = arr.at(1) - arr.front();
    pitches[arr.at(2) % 12] += duration;
  }

  auto largest = std::max_element(pitches.begin(), pitches.end(),
				  [](const std::pair<int,double>& p1,
				     const std::pair<int,double>& p2) {
				    return p1.second < p2.second; })->second;
  
  for (auto &pitch: pitches)
    pitch_scores.push_back((pitch.second / largest) * 100.);
}

void Key::compare_profile() {
  double maj_score, min_score;

  for (int j = 0; j < 12; ++j) {
    maj_score =  min_score = 0;

    for (std::deque<double>::iterator i = pitch_scores.begin(); i != pitch_scores.end();
	 ++i) {
      auto index = std::distance(pitch_scores.begin(), i);
      maj_score += *i * major[index];
      min_score += *i * minor[index];
      // std::cout << "Index: " << index << " Pitch score: " << *i
      // 		<< " Major score: " << maj_score << " Minor score: "
      // 		<< min_score << std::endl;
    }
    maj_score_vector.push_back(maj_score);
    min_score_vector.push_back(min_score);
    auto first_val = pitch_scores.front();
    pitch_scores.pop_front();
    pitch_scores.push_back(first_val);
  }

  std::vector<double>::iterator max_maj =
    std::max_element(maj_score_vector.begin(), maj_score_vector.end());
  std::vector<double>::iterator max_min =
    std::max_element(min_score_vector.begin(), min_score_vector.end());
  // std::cout << "Max_maj: " << *max_maj << " Max_min: " << *max_min << std::endl;
  
  *max_maj > *max_min ? key =
    std::to_string(std::distance(maj_score_vector.begin(), max_maj)) + "M"
    : key = std::to_string(std::distance(min_score_vector.begin(),
					 max_min)) + "m";

  // std::cout << "Key: " << key << std::endl;

  // for (std::vector<double>::iterator i = maj_score_vector.begin();
  //      i != maj_score_vector.end(); ++i)
  //   std::cout << *i << std::endl;
  // for (std::vector<double>::iterator i = min_score_vector.begin();
  //      i != min_score_vector.end(); ++i)
  //   std::cout << *i << std::endl;

}

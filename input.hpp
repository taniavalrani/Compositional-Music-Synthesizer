// Inputs a notes file as a doubly linked list of arrays
class Input
{
public:
  Input(char *argv) {
    std::string input;
    input = argv;
    set_input_source(input);
    read_file();
  }

  Input(std::string source) {
    set_input_source(source);
    get_file();
  }

  ~Input() { }

  // Functions
  void read_file();

  // Getters
  void get_file() const;
  void print_list() const;
  Util::Note_list get_note_list() const {
    return note_list;
  }
  int get_count() const {
    return count;
  }

  // Setters
  void set_input_source(std::string notes) {
    input_source = notes;
  }

 protected:
  std::string input_type, input_source;
  int start_time, stop_time, note;
  Util::Note_list note_list;
  int count;

};

// Functions
void Input::read_file() {
  std::ifstream input(input_source);
  std::string iInput_type;
  int istart_time, istop_time, inote, event = 0;

  if (! input) {
    std::cerr << "can't open input file \'" << input_source << "\'\n";
				
    exit(EXIT_FAILURE);
  }
    
  while ( input >> iInput_type >> istart_time >> istop_time >> inote ) {
    if (iInput_type == "Note") {      
      ++event;
      std::array<int, 3> event_data = { istart_time, istop_time, inote };
      note_list.push_back(event_data);
    }
  }
  count = event;
}

// Getters
void Input::get_file() const {
  std::ifstream input(input_source);

  if (! input) {
    std::cerr << "can't open input file \'" << input_source << "\'\n";
				
    exit(EXIT_FAILURE);
  }

  if (input.is_open())
    std::cout << input.rdbuf();
}

void Input::print_list() const {
  for (const auto &arr: note_list) {
    for (const auto &v: arr)
      std::cout << v << ' ';
    std::cout << "\n";
  }
  std::cout << "\n";
}

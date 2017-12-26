// Base class
class Score
{
public:
  /* Score() { } */

  /* Score(Input input) { */
  /*   /\* note_list = input.get_list(); *\/ */
  /*   /\* count = input.get_count(); *\/ */
  /*   set_note_list(input.get_note_list()); */
  /*   set_count(input.get_count()); */
  /* } */

  /* ~Score() { } */

  // Virtual functions
  virtual void build() { }
  virtual void print() { }

  // Getters
  int get_count() const {
    return count;
  }
  void write_list() const;

  // Setters
  void set_note_list(Util::Note_list iNote_list) {
    note_list = iNote_list;
  }
  void set_count(int iCount) {
    count = iCount;
  }
  void set_both(Util::Note_list iNote_list, int iCount) {
    set_note_list(iNote_list);
    set_count(iCount);
  }

 protected:
  // Data
  static Util::Note_list note_list;
  static int count;
};

void Score::write_list() const {
  for (const auto &arr: note_list) {
    for (const auto &v: arr)
      std::cout << v << ' ';
    std::cout << "\n";
  }
  std::cout << "\n";
}

Util::Note_list Score::note_list;
int Score::count;

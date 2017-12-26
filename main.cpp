#include "imports.hpp"
#include "util.hpp"
#include "input.hpp"
#include "score.hpp"
#include "tactus.hpp"
#include "phrase.hpp"

void Usage(char* input) {
  std::cout << "Usage: " << input << " <filename> " << "[-abhmpt]\n";
}

void setOptions( int argc, char *argv[], Tactus &tactus ) {
  std::vector<std::string> args(argv + 1, argv + argc);
    
  for (std::vector<std::string>::iterator i = args.begin(); i != args.end();
       ++i) {
    if (*i == "-a" || *i == "--above")
      tactus.set_tactus_above(stoi(*++i));
    if (*i == "-b" || *i == "--below") 
      tactus.set_tactus_below(stoi(*++i));
    if (*i == "-h" || *i == "--help") {
      Usage(argv[0]);
      exit(0);
    }
    if (*i == "-p" || *i == "--pip")
      tactus.set_pip_time(stoi(*++i));
    if (*i == "-m" || *i == "--meter")
      tactus.set_meter(*++i);
    if (*i == "-t" || *i == "--tempo")
      tactus.set_tempo(stoi(*++i));
  }
}

int main( int argc, char *argv[] )
{  
  Phrase phrase;
  Tactus tactus;
  Input input(argv[1]);
  
  Score *score;
  score->set_both(input.get_note_list(), input.get_count());
  
  if ( argc < 2 ) {
    Usage(argv[0]);
  } else {
    setOptions(argc, argv, tactus);
  }
  
  score = &phrase;
  score->build();
  score->print();
  
  score = &tactus;
  score->build();
  score->print();
}


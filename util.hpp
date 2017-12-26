#define LOG2(x) (log(x) / log(2))

namespace Util {
  typedef unsigned long int ulong;
  typedef std::multimap<ulong,ulong> Quantization;
  typedef std::map<ulong,ulong> Quantization_map;

  typedef std::list<std::array<int,3>> Note_list;
  
 
  template <typename T, typename container>
    T Quantize_Elm (T elm, container cont) {

    if (elm == cont.front()) {
      return cont.front();
    }
    if (elm >= cont.back()) {
      return cont.back();
    }

    auto closest = adjacent_find(cont.begin(), cont.end(),
			       [&elm] (T i, T j) { return i <= elm && elm <= j;});

    return (elm-(*closest) < *(closest+1)-elm) ? *closest : *(closest+1);
  }

  std::vector<std::vector<double>> Matrix (int m, int n) {
    std::vector<std::vector<double>> a;
    a.resize(m);
    for (int i = 0; i < m; ++i)
      a[i].resize(n);
    return a;
  }

   int normalize_vector(std::vector<int>& shift_vec) {
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
  
}

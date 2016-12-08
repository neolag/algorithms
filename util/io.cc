#include <iostream>
#include <iomanip>

#include <functional>
#include <map>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using istream_op = std::function<void(std::istream&)>;
using ostream_op = std::function<void(std::ostream&)>;

std::istream& operator>>(std::istream& s, const istream_op& op) {
  op(s);
  return s;
}
std::ostream& operator<<(std::ostream& s, const ostream_op& op) {
  op(s);
  return s;
}

// JSON

ostream_op as_json(const std::string& v) {
  return [&v](std::ostream& s) {
    s << '"' << v << '"';
  };
}

ostream_op as_json(double v) {
  return [&v](std::ostream& s) {
    s << v;
  };
}

template <typename Type>
ostream_op as_json(const std::vector<Type>& vec) {
  return [&vec](std::ostream& s) {
      s << '[';
      for (int i = 0; i < vec.size(); i++) {
        s << (i > 0 ? ", " : "") << as_json(vec[i]);
      }
      s << ']';
  };
}

template<typename map_t>
ostream_op map_as_json(const map_t& map, const bool compact = true) {
  return [&map, compact](std::ostream& s) {
      s << (compact ? "{" : "{ ");
      bool is_first = true;
      for (auto it = map.begin(); it != map.end(); ++it) {
        if (is_first) is_first = false;
        else s << (compact ? "," : ", ");

        s << as_json(it->first) << (compact ? ":" : ": ") << as_json(it->second);
      }
      s << (compact ? "}" : " }");
  };
}

template<typename key_t, typename value_t>
ostream_op as_json(const std::map<key_t, value_t>& map, const bool compact = true) {
  return map_as_json(map, compact);
};

template<typename key_t, typename value_t>
ostream_op as_json(const std::unordered_map<key_t, value_t>& map, const bool compact = true) {
  return map_as_json(map, compact);
};

// - Idea: Use custom template definition for std::to_string to define as template
//   definition when making the call to to_string.
//template<typename value_type, template to_string_fn>
//auto as_json(const value_type& v) -> std::enable_if<std::is_same<std::string, std::result_of<to_string_fn(value_type)>::type>> {
//  return [&v](std::ostream& s) {
//      s << std::to_string(v);
//  };
//}

template<typename Type>
using read_constructor = std::function<Type(std::istream&)>;

template<typename Type>
Type operator<<(std::istream& s, read_constructor<Type> r) {
  return r(s);
}

void read_all(std::istream& s) {}

template<typename Arg1, typename... Args>
void read_all(std::istream& s, Arg1& arg1, Args&... args) {
  s >> arg1;
  read_all(s, args...);
}


//template<typename Type, typename... Args>
//read_constructor<Type> read_construct(Args&... args) {
//  return [&, args...](std::istream& s) -> Type {
//      read_all(s, args...);
//      return Type(args...);
//  };
//}
//template<typename Type, typename... Args>
//read_constructor<Type*> read_new(Args&... args) {
//  return [&](std::istream& s) -> Type* {
//      read_all(s, args...);
//      return new Type(args...);
//  };
//}

// Vectors

template <typename Type>
istream_op content(std::vector<Type>& vec) {
  return [&vec](std::istream& s) {
      for (Type& e : vec) s >> std::ws >> e;
  };
}

template<typename Type>
istream_op size(std::vector<Type>& vec) {
  return [&vec](std::istream& s) {
    int size;
    s >> size;
    vec.resize(size);
  };
}

template<typename Type>
ostream_op content(const std::vector<Type>& vec,
                   const std::string& separator = " ") {
  return [&vec, separator](std::ostream& s) {
    bool first = true;
    for (auto& e : vec) {
      if (first) first = false;
      else s << separator;
      s << e;
    }
  };
}

template<typename Type>
ostream_op derefed(const std::vector<Type>& vec,
                   const std::string& separator = " ") {
  return [&vec, separator](std::ostream& s) {
    bool first = true;
    for (auto& e : vec) {
      if (first) first = false;
      else s << separator;
      s << *e;
    }
  };
}

//int main() {
//  using namespace std;
//  map<string, vector<string>> json = {{"key1", {"v1", "v2", "v3"}}};
//  cout << as_json(json, false) << endl;
//  cin >> read_new<string>();
//}
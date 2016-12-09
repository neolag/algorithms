#include <iostream>

#include <memory>
#include <string>

using namespace std;

struct Node {
  unique_ptr<Node> zero, one;

  unique_ptr<Node>& child(char c) {
    switch (c) {
      case '0': return zero;
      case '1': return one;
      default: throw exception();
    }
  }

  bool operator!=(const Node& o) {return !(*this == o);}
  bool operator==(const Node& o) {
    return zero == o.zero && one == o.one;
  }
};

static const Node kEmptyNode = Node();

int main() {
  Node ptree;
  string code;
  for (int iter = 1; !cin.eof(); iter++) {
    ptree = Node();
    bool decodable = true;
    while (true) {
      getline(cin, code);
      if (cin.eof() || code == "9") break;

      Node* curr = &ptree;
      bool is_new = false;
      for (auto c : code) {
        if (curr != &ptree && !is_new && *curr == kEmptyNode) {
          // prefix of current code already in tree
          decodable = false;
        }
        auto& child = curr->child(c);
        if (!child) {
          child.reset(new Node);
          is_new = true;
        }
        curr = child.get();
      }
      
      if (!is_new) {
        // current code is a prefix of something in the tree
        decodable = false;
      }
    }
    if (ptree != kEmptyNode) {
      cout << "Set " << iter << (decodable ? " is" : " is not")
           << " immediately decodable" << endl; 
    }
  }
  return 0;
}

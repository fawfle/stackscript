#include <../data-structures/stack.hpp>
#include <vector>
using namespace std;

class Scanner {
    private:
        vector<string> tokens;
    public:
        void tokenify(string str);
        
};
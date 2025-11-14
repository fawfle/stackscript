#include "scanner.hpp"
#include <sstream>

using namespace std;

// Need to handle edge cases
void Scanner::tokenify(string str) {
    tokens.clear();
    stringstream ss(str);
    string character;
    while (getline(ss,character,' ')) {
        tokens.push_back(character);
    }
}
#include <iostream>
#include <vector>
using namespace std;

vector<string> split(string str, char delimiter) {
	vector<string> strs;
	string *temp = new string("");
    for (unsigned i = 0; i < str.size(); i++) {
		if(str[i] == delimiter) {
			strs.push_back(*temp);			
			temp = new string("");
		} else {
			*temp += str[i];
		}
	}
    strs.push_back(*temp);
	return strs;
}

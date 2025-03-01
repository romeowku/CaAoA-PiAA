#include <iostream>
#include <vector>

using namespace std;

void print(const vector<int> &indexes) 
{
    int count = indexes.size();
    for (int i = 0; i < count; i++) 
    { 
        cout << indexes[i];
        if (i < count - 1) { cout << ","; }
    }
}

vector<int> prefixFunction(const string& pattern) 
{
    int patternLen = pattern.length();
    vector<int> prefix(patternLen, 0);
    int index = 0;

    for (int i = 1; i < patternLen; i++) 
    {
        while (index > 0 && pattern[i] != pattern[index])
            index = prefix[index - 1];

        if (pattern[i] == pattern[index])
            index++;

        prefix[i] = index;
    }   
    return prefix;
}

vector<int> KMP(const string& text, const string& pattern) 
{
    int textLen = text.length(), patternLen = pattern.length();
    vector<int> prefix = prefixFunction(pattern);
    vector<int> result;

    int coincidences = 0;
    for (int i = 0; i < textLen; i++) 
    {
        while (coincidences > 0 && text[i] != pattern[coincidences])
            coincidences = prefix[coincidences - 1];

        if (text[i] == pattern[coincidences]) { coincidences++; }

        if (coincidences == patternLen) 
        {
            result.push_back(i - patternLen + 1);
            coincidences = prefix[coincidences - 1];
        }
    }
    return result;
}

int main() 
{
    string pattern, text;
    cin >> pattern >> text;

    vector<int> entrances = KMP(text, pattern);

    if (!entrances.empty()) { print(entrances); } 
    else { cout << -1; }
    cout << '\n'; 
    return 0; 
}

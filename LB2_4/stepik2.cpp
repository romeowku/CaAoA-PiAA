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

int KMP(const string& text, const string& pattern) 
{
    string doubleText = text + text;
    int textLen = text.length(), patternLen = pattern.length();
    vector<int> prefix = prefixFunction(pattern);
    int coincidences = 0;

    for (int i = 0; i < textLen * 2; i++) 
    {
        while (coincidences > 0 && doubleText[i] != pattern[coincidences]) 
        {
            coincidences = prefix[coincidences - 1];
        }
        if (doubleText[i] == pattern[coincidences]) { coincidences++; }
        
        if (coincidences == patternLen) { return (i - textLen + 1); }
    }
    return -1;
}

int main() 
{
    string A, B;
    cin >> A >> B;

    if (A.length() == B.length()) 
    {
        int result = KMP(A, B);
        cout << result;
    } else { cout << "-1"; }
    cout << endl;

    return 0;
}

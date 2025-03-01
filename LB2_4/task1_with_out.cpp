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

void printMatch(const string& text, const string& pattern, int textPos, int patternPos, bool match) 
{
    cout << "ТЕКСТ:    " << text << endl;
    cout << "ШАБЛОН:   ";
    for (int i = 0; i < textPos - patternPos; i++) { cout << " "; }

    cout << pattern << endl;
    cout << "          ";

    for (int i = 0; i < textPos; i++) { cout << " "; }
    cout << "^" << (match ? " (✅ Совпадение)" : " (❌ Несовпадение)\n ----------Переход дальше по тексту----------") << "\n\n";
}


vector<int> prefixFunction(const string& pattern) 
{
    int patternLen = pattern.length();
    vector<int> prefix(patternLen, 0);
    int index = 0;

    cout << "\nПостроение префикс-функции для шаблона: \"" << pattern << "\"\n";
    cout << "----------------------------------------------------\n";

    cout << "Рассматриваем символ pattern[" << 0 << "] = '" << pattern[0] << "'\n";
    cout << "   Префикс-функция: prefix[" << 0 << "] = " << index << "\n\n";
    for (int i = 1; i < patternLen; i++) 
    {
        cout << "Рассматриваем символ pattern[" << i << "] = '" << pattern[i] << "'\n";

        while (index > 0 && pattern[i] != pattern[index]) 
        {
            cout << "   Несовпадение: pattern[" << i << "] = '" << pattern[i] 
                 << "' ≠ pattern[" << index << "] = '" << pattern[index] << "'\n";
            cout << "   Возвращаемся к prefix[" << index - 1 << "] = " << prefix[index - 1] << "\n";
            index = prefix[index - 1];  
        }

        if (pattern[i] == pattern[index]) 
        {
            index++;
            cout << "   Совпадение: pattern[" << i << "] = pattern[" << index - 1 << "] ('" << pattern[i] << "')\n";
        }

        prefix[i] = index;

        cout << "   Префикс-функция: prefix[" << i << "] = " << index << "\n\n";
    }

    cout << "Итоговая префикс-функция: ";
    print(prefix);
    cout << "\n----------------------------------------------------\n";
    
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
        {
            printMatch(text, pattern, i, coincidences, false);
            cout << "========== ВОЗВРАТ НА ИНДЕКС " << prefix[coincidences - 1] << " ==========\n\n";
            coincidences = prefix[coincidences - 1]; 
        }

        printMatch(text, pattern, i, coincidences, text[i] == pattern[coincidences]);

        if (text[i] == pattern[coincidences]) 
        { 
            coincidences++; 
        }

        if (coincidences == patternLen) 
        {
            cout << "__________✅НАЙДЕНО ВХОЖДЕНИЕ ШАБЛОНА В ПОЗИЦИИ " << i - patternLen + 1 << "__________✅" << endl << endl;
            result.push_back(i - patternLen + 1);
            cout << "========== Префикс сдвигает на " << prefix[coincidences - 1] << " ==========\n\n";
            coincidences = prefix[coincidences - 1]; 
        }
    }

    return result;
}

int main() 
{
    string pattern, text;
    cin >> pattern;
    cin >> text;

    vector<int> entrances = KMP(text, pattern);

    cout << "\nРезультат поиска: ";
    if (!entrances.empty()) { print(entrances); } 
    else { cout << -1; }
    cout << "\n"; 

    return 0; 
}

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
            printMatch(doubleText, pattern, i, coincidences, false);
            cout << "========== ВОЗВРАТ НА ИНДЕКС " << prefix[coincidences - 1] << " ==========\n\n";
            coincidences = prefix[coincidences - 1];
        }
        
        printMatch(doubleText, pattern, i, coincidences, doubleText[i] == pattern[coincidences]);

        if (doubleText[i] == pattern[coincidences]) { coincidences++; }
        
        if (coincidences == patternLen) 
        { 
            cout << "__________✅НАЙДЕНО ВХОЖДЕНИЕ ШАБЛОНА В ПОЗИЦИИ " << i - textLen + 1 << "__________✅" << endl << endl;
            return (i - textLen + 1); 
        }
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
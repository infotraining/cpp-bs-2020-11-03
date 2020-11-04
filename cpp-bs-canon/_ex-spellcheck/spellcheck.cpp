#include <chrono>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <regex>
#include <unordered_set>

using namespace std;

vector<std::string> regex_split(const std::string& s, std::string regex_str = R"(\s+)")
{
    std::vector<std::string> words;

    std::regex rgx(regex_str);

    std::sregex_token_iterator iter(s.begin(), s.end(), rgx, -1);
    std::sregex_token_iterator end;

    while (iter != end)
    {
        words.push_back(*iter);
        ++iter;
    }

    return words;
}

int main()
{
    // wczytaj zawartość pliku en.dict ("słownik języka angielskiego")    
    // sprawdź poprawość pisowni następującego zdania:    
    string input_text = "this is an exmple of very badd snetence";

    vector<string> words = regex_split(input_text);

    ifstream in("en.dict");

    if (!in)
    {
        std::exit(1);
    }


    std::unordered_set<std::string> dict;
    std::string entry;

    while (in >> entry)
    {
        dict.insert(entry);
    }

    std::cout << "Dictionary size: " << dict.size() << "\n";
    
    std::vector<std::string> misspelled;

    auto start = std::chrono::high_resolution_clock::now();

    for (const auto& tocheck : words)
        if (dict.find(tocheck) == dict.end())
            misspelled.push_back(tocheck);

    auto end = std::chrono::high_resolution_clock::now();

    for(const auto& item : misspelled)
        cout << item << " - " << "not exist" << endl;

    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "us\n\n";

    /////////////////////////////////////////////////////////////////

    std::vector<std::string> vec_dict(dict.begin(), dict.end());
    std::sort(vec_dict.begin(), vec_dict.end());
    misspelled.clear();

    start = std::chrono::high_resolution_clock::now();

    for (const auto& tocheck : words)
        if (!std::binary_search(vec_dict.begin(), vec_dict.end(), tocheck))
            misspelled.push_back(tocheck);

    end = std::chrono::high_resolution_clock::now();

    for (const auto& item : misspelled)
        cout << item << " - " << "not exist" << endl;

    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "us\n";
}
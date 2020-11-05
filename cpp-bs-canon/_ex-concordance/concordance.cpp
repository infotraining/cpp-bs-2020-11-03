#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <unordered_map>
#include <set>
#include <string>
#include <algorithm>
#include <unordered_map>

using namespace std;

/*
    Napisz program zliczający ilosc wystapien danego slowa w pliku tekstowym. Wyswietl 20 najczęściej występujących slow (w kolejności malejącej).
*/

std::vector<std::string> load_words(const std::string& file_name)
{
    ifstream fin(file_name);

    if (!fin)
        throw runtime_error("File "s + file_name + " can't be opened");

    std::vector<std::string> words;
    words.reserve(250'000);

    std::string token;
    while (fin >> token)
    {
        words.push_back(token);
    }

    return words;
}

std::unordered_map<std::string, size_t> count_words(const std::vector<std::string>& words)
{
    std::unordered_map<std::string, size_t> concordance;

    for (const auto& item : words)
        ++(concordance[item]);

    return concordance;
}

std::multimap<size_t, std::string, std::greater<>> make_rating(const std::unordered_map<std::string, size_t>& concordance)
{
    std::multimap<size_t, std::string, std::greater<>> rating;

    for (const auto& item : concordance)
        rating.emplace(item.second, item.first);

    return rating;
}

int main()
{
    const string file_name = "tokens.txt";

    std::vector<std::string> words = load_words(file_name);

    std::cout << "Loading file... " << words.size() << " words has been loaded...\n";

    std::cout << "The most common words:\n";

    auto rating = make_rating(count_words(words));
    
    std::for_each_n(rating.begin(), 20, [](const auto& kv) { std::cout << kv.second << " - " << kv.first << "\n"; });
}

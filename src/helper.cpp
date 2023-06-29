#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>

using namespace std;

string trim(const string &str)
{
    string result(str);
    result.erase(0, result.find_first_not_of(" \t\r\n"));
    result.erase(result.find_last_not_of(" \t\r\n") + 1);
    return result;
}

string join(vector<string> items, string delimiter)
{
    string result;
    for (string item : items)
        result += result.empty() ? item : delimiter + item;
    return result;
}

vector<string> split(string str, char separator)
{
    vector<string> list;
 
    stringstream stream(str);
    string word;
    while (!stream.eof()) {
        getline(stream, word, separator);
        list.push_back(word);
    }

    return list;
}

int strpos(const string &haystack, const string &needle)
{
    int sleng = haystack.length();
    int nleng = needle.length();

    if (sleng == 0 || nleng == 0)
        return -1;
 
    for (int i = 0, j = 0; i < sleng; j = 0, i++)
    {
        while (i + j < sleng && j < nleng && haystack[i+j] == needle[j]) j++;
        if (j == nleng) return i;
    }

    return -1;
}

string array_ends_with(vector<string> haystack, string needle)
{
    string lastrow;
    reverse(haystack.begin(), haystack.end());
    for (string row : haystack)
    {
        if (strpos(row, needle) != -1)
        {
            lastrow = row;
            break;
        }
    }
    return lastrow;
}

string env(vector<string> envlist, string s_key)
{
    string found;
    for (string item : envlist) 
    {
        item = trim(item);

        int possep = strpos(item, "=");
        string key = item.substr(0, possep);
        int n = item.length() - key.length() - 1;
        string value = item.substr(possep + 1, n);

        if (key == s_key)
        {
            found = value;
            break;
        }
    }
    return found;
}
#include <iostream>
#include <string>
#include <vector>
#include <cstring>

using namespace std;

struct TrieNode {
    char data;
    bool IsTheEnd;
    vector<TrieNode*> children;

    TrieNode(char ch) : data(ch), IsTheEnd(false), children(26, nullptr) {}
};

class Tree {
private:
    TrieNode* root;

    void InsertChars(TrieNode* node, const string& word, int index)
    {
        if (index == word.size())
        {
            node->IsTheEnd = true;
            return;
        }
        int charIndex = word[index] - 'a';
        if (node->children[charIndex] == nullptr)
        {
            node->children[charIndex] = new TrieNode(word[index]);
        }
        InsertChars(node->children[charIndex], word, index + 1);
    }

    void exploreBranches(TrieNode* node, string currentWord, vector<string>& words)
    {
        if (node->IsTheEnd)
        {
            words.push_back(currentWord);
        }
        for (int i = 0; i < 26; ++i)
        
        {
            if (node->children[i])
            {
                exploreBranches(node->children[i], currentWord + char('a' + i), words);
            }
        }
    }

    vector<string> wildcardSearchHelper(TrieNode* node, const string& pattern, int index, string currentWord) {
        vector<string> results;
        if (index == pattern.size())
        {
            if (node->IsTheEnd)
            {
                results.push_back(currentWord);
            }
            return results;
        }

        char ch = pattern[index];
        if (ch == '*')
        {
            for (int i = 0; i < 26; ++i)
            {
                if (node->children[i])
                {
                    vector<string> temp = wildcardSearchHelper(node->children[i], pattern, index, currentWord + char('a' + i));
                    results.insert(results.end(), temp.begin(), temp.end());
                }
            }
            vector<string> temp = wildcardSearchHelper(node, pattern, index + 1, currentWord);
            results.insert(results.end(), temp.begin(), temp.end());
        }
        else
        {
            int charIndex = ch - 'a';
            if (node->children[charIndex])
            {
                results = wildcardSearchHelper(node->children[charIndex], pattern, index + 1, currentWord + ch);
            }
        }

        return results;
    }

public:
    Tree() : root(new TrieNode('\0')) {}

    void Insert(const string& word)
    {
        InsertChars(root, word, 0);
    }

    bool search(const string& word)
    {
        TrieNode* temp = root;
        for (char ch : word)
        {
            int charIndex = ch - 'a';
            if (temp->children[charIndex] == nullptr)
            {
                return false;
            }
            temp = temp->children[charIndex];
        }
        return temp->IsTheEnd;
    }

    void printAll(TrieNode* node, string currentWord = "")
    {
        if (node == nullptr)
        {
            return;
        }

        if (node->IsTheEnd)
        {
            cout << currentWord << endl;
        }

        for (int i = 0; i < 26; ++i)
        {
            if (node->children[i])
            {
                printAll(node->children[i], currentWord + char('a' + i));
            }
        }
    }

    vector<bool> multipleSearch(const vector<string>& words)
    {
        vector<bool> results;
        for (const string& word : words)
        {
            results.push_back(search(word));
        }
        return results;
    }

    vector<string> prefixSearch(const string& prefix)
    {
        TrieNode* temp = root;
        vector<string> results;
        for (char ch : prefix)
        {
            int charIndex = ch - 'a';
            if (temp->children[charIndex] == nullptr)
            {
                return results;
            }
            temp = temp->children[charIndex];
        }
        exploreBranches(temp, prefix, results);
        return results;
    }

    vector<string> searchFuzzy(const string& pattern)
    {
        vector<string> results;
        TrieNode* curr = root;
        fuzzySearchHelper(curr, pattern, 0, "", results);
        return results;
    }

    void fuzzySearchHelper(TrieNode* node, const string& pattern, int index, string currentWord, vector<string>& results)
    {
        if (index == pattern.size())
        {
            if (node->IsTheEnd)
            {
                results.push_back(currentWord);
            }
            return;
        }

        char ch = pattern[index];
        if (ch == '?')
        {
            for (int (i) = 0; (i) < 26; ++i)
            {
                if (node->children[i])
                {
                    fuzzySearchHelper(node->children[i], pattern, index + 1, currentWord + char('a' + i), results);
                }
            }
        }
        else
        {
            int charIndex = ch - 'a';
            if (node->children[charIndex])
            {
                fuzzySearchHelper(node->children[charIndex], pattern, index + 1, currentWord + ch, results);
            }
        }
    }

    void misSpelled(TrieNode* node, const char* word, char* prefix, int length)
    {
        if (word[0] == '\0' && node->IsTheEnd)
        {
            prefix[length] = '\0';
            cout << prefix << endl;
            return;
        }
        for (int i = 0; i < 26; i++)
        {
            if (node->children[i])
            {
                prefix[length] = 'a' + i;
                misSpelled(node->children[i], word + (word[0] == 'a' + i), prefix, length + 1);
            }
        }
    }

    void misSpelled(const char* word)
    {
        char miss_arr[100];
        misSpelled(root, word, miss_arr, 0);
    }

    vector<string> wildcardSearch(const string& pattern)
    {
        return wildcardSearchHelper(root, pattern, 0, "");
    }
};

int main() {
    Tree trie;
    int choice;
    string word, pattern, prefix;
    vector<string> words;

    while (true)
    {
        cout << "1. Insert a word "<<endl;
        cout << "2. Search for a single word"<<endl;
        cout << "3. Search for multiple words"<<endl;
        cout << "4. Prefix search"<<endl;
        cout << "5. Fuzzy search"<<endl;
        cout << "6. Wildcard search"<<endl;
        cout << "7. Misspelled word search"<<endl;
        cout << "8. Exit"<<endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter word to insert: ";
                cin >> word;
                trie.Insert(word);
                cout << "Word inserted."<<endl;;
                break;

            case 2:
                cout << "Enter word to search: ";
                cin >> word;
                cout << "Word " << (trie.search(word) ? "exists.\n" : "does not exist.\n");
                break;

            case 3:
                int n;
                cout << "Enter number of words: ";
                cin >> n;
                words.clear();
                cout << "Enter words:\n";
                for (int i = 0; i < n; ++i) {
                    cin >> word;
                    words.push_back(word);
                }
                {
                    vector<bool> results = trie.multipleSearch(words);
                    cout << "Multiple Words Search: ";
                    for (bool result : results) {
                        cout << (result ? "True" : "False") << " ";
                    }
                    cout << endl;
                }
                break;

            case 4:
                cout << "Enter prefix to search: ";
                cin >> prefix;
                {
                    vector<string> prefixResults = trie.prefixSearch(prefix);
                    cout << "Prefix Search for '" << prefix << "': ";
                    for (const string& word : prefixResults) {
                        cout << word << " ";
                    }
                    cout << endl;
                }
                break;

            case 5:
                cout << "Enter pattern for fuzzy search including a question mark : ";
                cin >> pattern;
                {
                    vector<string> fuzzyResults = trie.searchFuzzy(pattern);
                    cout << "Fuzzy Search for '" << pattern << "': ";
                    for (const string& word : fuzzyResults) {
                        cout << word << " ";
                    }
                    cout << endl;
                }
                break;

            case 6:
                cout << "Enter pattern for wildcard search including a * : ";
                cin >> pattern;
                {
                    vector<string> wildcardResults = trie.wildcardSearch(pattern);
                    cout << "Wildcard Search for '" << pattern << "': ";
                    for (const string& word : wildcardResults) {
                        cout << word << " ";
                    }
                    cout << endl;
                }
                break;

            case 7:
                cout << "Enter word for misspelled search: ";
                cin >> word;
                cout << "Misspelled Search for '" << word << "': ";
                trie.misSpelled(word.c_str());
                break;

            case 8:
                cout << "thankss"<<endl;
                return 0;

            default:
                cout << "Invalid choice, please try again."<<endl;
                break;
        }
    }

    return 0;
}

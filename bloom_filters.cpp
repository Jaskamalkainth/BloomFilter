#include <iostream>
#include <functional>
#include <vector>
#include <string>

using namespace std;

const int MAX_BUFF_SIZE = 1e6; // 4MB

using HashFunction = function<int(const string&)>;

class BloomFilters
{
    private:
        vector<char> buffer;
        vector<HashFunction> hashFunctions; // K hash functions

    public:
        BloomFilters(int bufferSize, vector<HashFunction>& functions)
        {
            buffer.resize(bufferSize);
            hashFunctions = functions;
        }
        /*
            Inserts the input string
            TC: O(K* HFC) where HFC is hash function complexity
            HFC for below hash functions is O(|S|) where |S| is string length
        */
        void insert(const string& str) 
        {
            for (auto& func: hashFunctions) 
            {
                buffer[func(str) % buffer.size()] = 1;
            }
        }
        const bool isPresent(const string& str) const
        {
            for (auto& func: hashFunctions)
            {
                if (buffer[func(str) % buffer.size()] != 1)
                    return false;
            }
            return true;
        }

};

unsigned int hashFunction1(const string& str)
{
    const int b = 31;
    unsigned int res = 0;
    for (const char& ch: str)
    {
        res = res*b + ch;
    }
    return res;
}
unsigned int hashFunction2(const string& str)
{
    const int b = 3727;
    unsigned int res = 0;
    for (const char& ch: str)
    {
        res = res*b + ch - '0';
    }
    return res;
}

int main()
{
    vector<HashFunction> functions;
    functions.push_back(hashFunction1);
    functions.push_back(hashFunction2);

    BloomFilters *bf1 = new BloomFilters(MAX_BUFF_SIZE, functions);

    // cout << bf1->isPresent("foo") << endl;
    // bf1->insert("foo");
    // cout << bf1->isPresent("foo") << endl;

    // cout << bf1->isPresent("bar") << endl;
    // bf1->insert("bar");
    // cout << bf1->isPresent("bar") << endl;

    // cout << bf1->isPresent("foobar") << endl;
    // bf1->insert("foobar");
    // cout << bf1->isPresent("foobar") << endl;

    int q; cin >> q;
    while (q--)
    {
        int option; cin >> option;
        string str; cin >> str;
        if (option == 1)
        {
           bf1->insert(str);
        }
        else 
        {
            if (bf1->isPresent(str) == 1)
                cout << "present\n";
            else
                cout << "not present\n";
        }
    }

    delete bf1;
    return 0;
}
/*
    what is the probability that given string S, isPresent(S) returns True when it was not
    present at the first place.

    falsePositive when for String S: all k bits are set after N insertions of strings

    Insert a string S: 

    P(bit is 1) = 1/m
    P(bit is 0) = 1-1/m
    P(the bit is 0 after k Hash functions) = (1-1/m)^k
    P(the bit is 0 after N insertions) = (1-1/m)^{kn}
    P(the bit is 1 after N insertions) = 1 - (1-1/m)^{kn}

    P(all k bits is 1 after N insertion) = (1-(1-1/m)^{kn})^k
                                         = (1-e^{kn/m})^k

    Prob is minimum when k = m/n * log2 n: number of insertions m: buffer size
    https://pages.cs.wisc.edu/~cao/papers/summary-cache/node8.html
*/
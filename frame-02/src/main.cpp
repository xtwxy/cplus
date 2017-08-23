#include <iostream>
#include <iomanip>

#include <string>
#include <cstdlib>

#include <algorithm>
#include <vector>
#include <set>
#include <memory>

using namespace std;

namespace token {
    struct Delimiter {
        Delimiter() { }
        Delimiter(const Delimiter& rhs) : delims(rhs.delims) { }
        virtual ~Delimiter() { }

        bool operator == (const Delimiter& rhs) const {
            return this->delims == rhs.delims;
        }

        Delimiter& operator = (const Delimiter& rhs) {
            this->delims = rhs.delims;
            return *this;
        }

        Delimiter& operator = (const string& rhs) {
            this->delims.insert(rhs);
            return *this;
        }

        Delimiter& operator | (const string& d) {
            delims.insert(d);
            return *this;
        }

        Delimiter& operator |= (const string& d) {
            delims.insert(d);
            return *this;
        }

        set<string> delims; 
    };

    struct Tokenizer {
        Tokenizer() { }
        Tokenizer(const Delimiter& rhs) : delims(rhs) { }
        Tokenizer(const Tokenizer& rhs) : delims(rhs.delims) { }
        virtual ~Tokenizer() { }

        bool operator == (const Tokenizer& rhs) const {
            return this->delims == rhs.delims;
        }

        Tokenizer& operator = (const Tokenizer& rhs) {
            this->delims = rhs.delims;
            return *this;
        }

        vector<string>& operator()(const string& str, vector<string>& tokens) {
            size_t pos = 0;
            string emit;
            while(pos < str.length()) {
                bool matched = false;
                for(auto i = delims.delims.begin(); i != delims.delims.end(); ++i) {
                    if(str.compare(pos, i->length(), *i) == 0) {
                        matched = true;
                        pos += i->length();
                        break;
                    }
                }
                if(matched) {
                    if(emit.length() > 0) {
                        tokens.push_back(emit);
                        emit.clear();
                    }
                } else {
                    char c = str.at(pos++);
                    emit.push_back(c);
                }
            }
            tokens.push_back(emit);
            emit.clear();

            return tokens;
        }

        Delimiter delims; 
    };

}

int main(int argc, char* argv[]) {
    if(argc != 2) {
        cout << "Usage:"
            << "\t" << argv[0]
            << " <string>" << endl;
        return EXIT_FAILURE;
    }

    token::Delimiter d;

    d = ",";
    d |= " ";
    d | "cafe" | "babe" | "\t" | "\n" | "\r";
    d | std::string("gun") | std::string("rose");

    token::Tokenizer tokenize(d);
    vector<string> tokens;
    string str = argv[1];
    tokenize(str, tokens);

    cout << "string:" << endl;
    cout << "\t" << str << endl;
    cout << "delimiters:" << endl;
    for_each(d.delims.begin(), d.delims.end(), [] (const string& s) {
            cout << "\t[" << s << "]" << endl;
            });

    cout << "tokens:" << endl;
    for_each(tokens.begin(), tokens.end(), [] (const string& s) {
            cout << "\t[" << s << "]" << endl;
            });

    return EXIT_SUCCESS;
}


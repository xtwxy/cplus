#include <iostream>
#include <iomanip>

#include <string>
#include <cstdlib>

#include <algorithm>
#include <vector>
#include <map>
#include <memory>

using namespace std;

struct JsValue {
    typedef shared_ptr<JsValue> Ptr;
    virtual const string json() const = 0;
};

struct JsString : public JsValue {
    JsString() { }
    JsString(const string& v) : str(v) { }

    bool operator < (const JsString& rhs) const {
        return this->str < rhs.str;
    }

    virtual const string json() const {
        string s;
        s.append("\"")
            .append(str)
            .append("\"");
        return s;
    }

    string str;
};

struct JsObject :public JsValue {
    virtual const string json() const {
        string s;
        s.append("{");
        bool first = true;
        for_each(fields.begin(), fields.end(), [&first, &s](const pair<string, JsValue::Ptr>& f) {
                if(first) {
                    first = false;
                    append(s, f);
                } else {
                    s.append(",");
                    append(s, f);
                }
            });
        s.append("}");
        return s;
    }
   
    static void append(string& s, const pair<string, JsValue::Ptr>& f) {
        s.append("\"")
            .append(f.first)
            .append("\"")
            .append(":")
            .append(f.second->json());
    }

    static JsObject fromJson(const string& json) {
        JsObject o;
        return o;
    }

    map<string, JsValue::Ptr> fields;
};

int main(int argc, char* argv[]) {
    JsObject o;
    o.fields.insert(make_pair("hello", make_shared<JsString>("world")));
    o.fields.insert(make_pair("cafe", make_shared<JsString>("babe")));
    cout << o.json() << endl;

    string s = "{\"name\":\"babe\",\"name\":\"world\"}";
    
    JsObject j = JsObject::fromJson(s);
    cout << j.json() << endl;

    return EXIT_SUCCESS;
}


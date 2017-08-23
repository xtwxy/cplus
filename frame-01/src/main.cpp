#include <iostream>
#include <iomanip>

#include <string>
#include <cstdlib>

#include <algorithm>
#include <vector>
#include <memory>

using namespace std;

template<typename T> struct Field;

struct AbstractField {
    typedef shared_ptr<AbstractField> Ptr;
    virtual ~AbstractField() {}
    virtual const unsigned char* bytes() const = 0;
    virtual size_t size() const = 0;
    virtual const type_info& type() const = 0;

    template<typename T>
    static const shared_ptr<Field<T> > cast(Ptr p) {
        if(typeid(Field<T>) == p->type()) {
            return static_pointer_cast<Field<T> >(p);
        } else {
            return shared_ptr<Field<T> >();
        }
    }
};

template<typename T>
struct Field : public AbstractField {
    typedef shared_ptr<Field> Ptr;
    Field(T v) : repr(v) {}
    virtual ~Field() {}
    const unsigned char* bytes() const {
        return repr.bytes;
    }

    void value(const T& v) {
        repr.value = v;
    }

    template<typename V> const V value() const {
        return *reinterpret_cast<const V*>(&repr.value);
    }

    size_t size() const {
        return sizeof(T);
    }

    const type_info& type() const {
        return typeid(*this);
    }

    union Repr {
        Repr(T v) : value(v) {}
        unsigned char bytes[sizeof(T)];
        T value;
    } repr;
};

template<typename Ptr>
void printField(Ptr f) {
    cout << "f->size() = " << f->size() 
        << ", f->value<int>() = " << f->template value<int>()
        << ", f->value<long>() = " << f->template value<long>()
        << ", f->value<float>() = " << f->template value<float>()
        << ", f->value<double>() = " << f->template value<double>()
        << endl;

    cout << "f->bytes() = [ ";
    cout << hex << setw(2) << setfill('0');
    for_each(f->bytes(), (f->bytes() + f->size()), [] (const unsigned char x) {
            cout 
            << (0xff & x)
            << " ";
            }); 
    cout << "]" << endl;
}

void print(AbstractField::Ptr p) {
    if(AbstractField::cast<int>(p)) {
        cout << "bingo! type: " << p->type().name() << endl;
    }

    if(typeid(Field<int>) == p->type()) {
        printField(static_pointer_cast<Field<int> >(p));
    } else if(typeid(Field<long>) == p->type()) {
        printField(static_pointer_cast<Field<long> >(p));
    } else if(typeid(Field<float>) == p->type()) {
        printField(static_pointer_cast<Field<float> >(p));
    } else if(typeid(Field<double>) == p->type()) {
        printField(static_pointer_cast<Field<double> >(p));
    } else {
        cout << "unknown type: " << p->type().name() << endl;
    }
}

int main(int argc, char* argv[]) {

    vector<AbstractField::Ptr> fields;
    fields.push_back(make_shared<Field<int> >(64));
    fields.push_back(make_shared<Field<long> >(128));
    fields.push_back(make_shared<Field<float> >(1.414f));
    fields.push_back(make_shared<Field<double> >(2.71828));

    for_each(fields.begin(), fields.end(), [] (AbstractField::Ptr p) {
            print(p);
            });
    return EXIT_SUCCESS;
}


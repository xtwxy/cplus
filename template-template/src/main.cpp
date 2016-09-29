#include <iostream>
#include <cstdlib>

using namespace std;

template <typename T>
class Vector {
 public:
  Vector operator*(const Vector& rhs) const {
    return *this;
  }

  string info() {
    return __PRETTY_FUNCTION__;
  }
};

template <typename P, template<typename T> class V>
class HighOrderAlgorithm {
  public:
   typedef V<P> X;
   HighOrderAlgorithm(const X k) : k_(k) { }

   X operator() (const X& x) const {
    return k_ * x; 
   }

  string info() {
    return __PRETTY_FUNCTION__;
  }
 
  private:
   X k_;
};

template<typename T>
class LowOrderAlgorithm {
 public:
  LowOrderAlgorithm(const T& k) :k_(k) { }

  T operator() (const T& x) const {
    return k_ * x;
  }
  
  string info() {
    return __PRETTY_FUNCTION__;
  }
 
 private:
   T k_;
};

int main(int argc, char* argv[]) {
  typedef Vector<double> DoubleVector;
  typedef LowOrderAlgorithm<DoubleVector> DoubleVectorAction;
  typedef HighOrderAlgorithm<double, Vector> Action;

  DoubleVector dvk;
  DoubleVector dv;
  DoubleVectorAction dva(dvk);
  dva(dv);

  Action::X k;
  Action::X x;
  Action action(k);
  action(x);

  cout << dvk.info() << endl;
  cout << dv.info() << endl;
  cout << dva.info() << endl;
  
  cout << k.info() << endl;
  cout << x.info() << endl;
  cout << action.info() << endl;

	return EXIT_SUCCESS;
}


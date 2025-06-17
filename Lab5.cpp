#include <bits/stdc++.h>
using namespace std;

class Complex{
    private:
        double a, b;

    public:
        Complex(){}
        Complex(double a, double b = 0) : a(a), b(b) {}

        Complex operator+(Complex &other){
            Complex result;
            result = *this;

            result.a += other.a;
            result.b += other.b;
            return result;
        }

        Complex operator-(Complex &other){
            Complex result;
            result = *this;

            result.a -= other.a;
            result.b -= other.b;
            return result;
        }

        Complex operator*(Complex &other){
            Complex result;

            result.a = (other.a)*(a) - (b)*(other.b);
            result.b *= other.b;
            return result;
        }

        void printComplex(){
            cout << a << " " << b << endl;
        }

        void printRational(){
            cout << a << endl;
        }
};

class Rational : public Complex{
    private:
        int numr, denr;

    public:

    //create a constructor with complex as parameter
        Rational(){}
        Rational(int numr, int denr) : numr(numr), denr(denr), Complex((double(numr)/denr)) {}  

        void print(){
            cout << double(numr) << endl;
        }

        int getNumr(){ return numr;}
        void setNumr(int n){numr = n; }
        void setDenr(int d){ denr = d;}
};


class Natural : public Complex{
    private:
        int n;

    public:
        Natural(){}
        Natural(int n) : n(n), Complex(double(n)) {}

        void print(){
            cout << n << endl;
        }
};


int main(){
    int op;
    cin >> op;
    string type;
    Complex* result; 

    while(op--){
        cin >> type;
        string command;
        cin >> command;

        if(type == "complex"){
            if(command == "add"){}
            else if(command == "sub"){}
            else if(command == "mult"){}
        }

        else if(type == "rational"){
            if(command == "add"){
                int n1, d1;
                int n2, d2;
                cin >> n1 >> d1;
                cin >> n2 >> d2;
                Complex *c1 = new Rational(n1, d1);
                Complex *c2 = new Rational(n2, d2);
                Complex result = (*c1 + *c2);
                result.printRational();
            }
            else if(command == "sub"){
                int n1, d1;
                int n2, d2;
                cin >> n1 >> d1;
                cin >> n2 >> d2;
                Complex *c1 = new Rational(n1, d1);
                Complex *c2 = new Rational(n2, d2);
                Complex result = (*c1 + *c2);
                result.printRational();
            }
            else if(command == "mult"){}
            else if(command == "reduce"){}
        }

        else if(type == "natural"){
            if(command == "isprime"){}
            else if(command == "inverse"){}
        }
    }

    return 0;
}

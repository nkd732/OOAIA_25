#include <bits/stdc++.h> 
using namespace std;

class Complex{  //Complex class
    private:
        long long int r, i;  
    public: 
        Complex(long long int a = 0, long long int b = 0) : r(a), i(b) {}

        Complex operator+(Complex &other){   
            return Complex(r+other.r, i + other.i);
        }
        Complex operator-(Complex &other){   
            return Complex(r-other.r, i - other.i);
        }
        Complex operator*(Complex &other){   
            return Complex((r * other.r - i * other.i), (r * other.i + i * other.r)); 
        }
        Complex& operator+=(Complex &other){ 
            r += other.r;
            i += other.i;
            return *this;
        }
        Complex& operator-=(Complex &other){ 
            r -= other.r;
            i -= other.i;
            return *this;
        }

        long long int getReal() { return r; } 
        long long int getComplex() { return i; }
        bool check(){
            return (r == 0 && i == 0); 
        }

        friend ostream& operator<<(ostream &out, const Complex &c){
            out << c.r << " " << c.i;
            //if (c.i != 0) out << " " << c.i; 
            return out;
        }
};

template <class T>
class Multiplication{  //using karstuba 
private:
    vector<T> p1;

public:
    Multiplication(vector<T> coeff) : p1(coeff) {}

    vector<T> operator*(vector<T> &p2){
        return multiply(p1, p2);
    }

    vector<T> multiply(vector<T> &p1, vector<T> &p2){
        long long int n  = max(p1.size(), p2.size());
        long long int newSz = 1;
        while(newSz < n) newSz *= 2;
        newSz *= 2;

        vector<T> A = p1;
        vector<T> B = p2;
        A.resize(newSz, 0);  //to make both polynomials of same size
        B.resize(newSz, 0);

        return multiplyPoly(A, B);
    }

private:
    vector<T> multiplyPoly(vector<T> &A, vector<T> &B){
        long long int n = A.size();
        if (n == 1) return {A[0] * B[0]};

        long long int mid = n/2;
        vector<T> Alow(A.begin(), A.begin() + mid);  //take of n/2 size
        vector<T> Ahigh(A.begin() + mid, A.end());
        vector<T> Blow(B.begin(), B.begin() + mid);
        vector<T> Bhigh(B.begin() + mid, B.end());

        vector<T> P1 = multiplyPoly(Alow, Blow);
        vector<T> P2 = multiplyPoly(Ahigh, Bhigh);

        vector<T> Asum(mid);
        vector<T> Bsum(mid);
        for(size_t i = 0; i < mid; i++){
            Asum[i] = Alow[i] + Ahigh[i];
            Bsum[i] = Blow[i] + Bhigh[i];
        }

        vector<T> P3 = multiplyPoly(Asum, Bsum);

        vector<T> result(2 * n - 1, 0);
        for(size_t i = 0; i < P1.size(); i++) result[i] += P1[i];
        for(size_t i = 0; i < P2.size(); i++) result[i + n] += P2[i];
        for(size_t i = 0; i < P3.size(); i++){
            result[i + mid] += (P3[i] - P1[i] - P2[i]);
        }
        return result;
    }
};

template <>
vector<Complex> Multiplication<Complex>::multiplyPoly(vector<Complex> &A, vector<Complex> &B){
    long long int n = A.size();
    if (n == 1) return {A[0] * B[0]};

    long long int mid = n / 2;
    vector<Complex> Alow(A.begin(), A.begin() + mid);
    vector<Complex> Ahigh(A.begin() + mid, A.end());
    vector<Complex> Blow(B.begin(), B.begin() + mid);
    vector<Complex> Bhigh(B.begin() + mid, B.end());

    vector<Complex> P1 = multiplyPoly(Alow, Blow);
    vector<Complex> P2 = multiplyPoly(Ahigh, Bhigh);

    vector<Complex> Asum(mid);
    vector<Complex> Bsum(mid);
    for(size_t i = 0; i < mid; i++){
        Asum[i] = Alow[i] + Ahigh[i];
        Bsum[i] = Blow[i] + Bhigh[i];
    }

    vector<Complex> P3 = multiplyPoly(Asum, Bsum);

    vector<Complex> result(2 * n - 1, Complex(0, 0));
    for(size_t i = 0; i < P1.size(); i++) result[i] = P1[i];
    for(size_t i = 0; i < P2.size(); i++) result[i + n] = P2[i];
    for(size_t i = 0; i < P3.size(); i++){
        Complex temp = P3[i] - P1[i] - P2[i]; // Store the temporary result
        result[i + mid] += temp;
    }
    return result;
}

template <class T>
class Evaluation{
public:
    T evaluate(vector<T> &coeffs, long long int x){
        T result = coeffs[0];
        long long int power = x;  // Start with x^1

        for(size_t i = 1; i < coeffs.size(); i++){
            result = result + (coeffs[i] * power);
            power = power * x;  // Multiply by x in each step
        }
        return result;
    }
};

template<>
string Evaluation<string>::evaluate(vector<string> &coeffs, long long int x){  //evalution for string type
    string result("");
    //reverse(coeffs.begin(), coeffs.end());
    long long int n = coeffs.size();

    for(long long int i = n-1; i >= 0; i--){   //start from back
        for(long long int j = 0; j < pow(x, i); j++){  //till pow(x, i) times, append coeff[i] to result
            result += coeffs[i];      
        }
    }
    return result;
}

template <class T>
class Differentiation{
public:
    vector<T> differentiate(const vector<T>& coeffs){
        vector<T> result;
        for(size_t i = 1; i < coeffs.size(); i++){
            result.push_back(coeffs[i] * i);  //result[i] = coeff[i+1] * (i+1) 
        }
        return result;
    }
};


int main(){
    long long int q;
    cin >> q;

    while(q--){
        long long int op;  //operation
        cin >> op;

        if(op == 1){  //Multiplication
            string type;  //input type
            cin >> type;

            long long int deg1;  //degree of polynomial 1
            cin >> deg1;

            if(type == "integer"){
                vector<long long int> poly1(deg1);
                for(long long int i = 0; i < deg1; i++){
                    cin >> poly1[i];
                }

                long long int deg2;   //degree of poly 2
                cin >> deg2;
                vector<long long int> poly2(deg2); 
                for(long long int i = 0; i < deg2; i++){
                    cin >> poly2[i];
                }
                
                Multiplication<long long int> multiplier(poly1); 
                vector<long long int> result = multiplier*poly2;
                while((result.size() > deg1 + deg2 - 1) && !result.empty() && result.back() == 0){
                    result.pop_back();  //to remove trailing zeros
                }                
                for(long long int x : result) cout << x << " ";
                cout << endl;
            }   

            else if (type == "float"){
                vector<long double> poly1(deg1);
                for(long long int i = 0; i < deg1; i++){
                    cin >> poly1[i];
                }

                long long int deg2;
                cin >> deg2;
                vector<long double> poly2(deg2);
                for(long long int i = 0; i < deg2; i++){
                    cin >> poly2[i];
                }

                Multiplication<long double> multiplier(poly1);
                vector<long double> result = multiplier*(poly2);
                while((result.size() > deg1 + deg2 - 1)&& !result.empty() && result.back() < 1e-9){
                    result.pop_back();
                }                
                for(long double x : result){
                    cout << fixed << setprecision(6) << x << " ";
                }
                cout << endl;
            }
            else if(type == "complex"){
                vector<Complex> poly1(deg1);
                for(long long int i = 0; i < deg1; i++){
                    long long int real, imag;
                    cin >> real >> imag;
                    poly1[i] = Complex(real, imag);
                }

                long long int deg2;
                cin >> deg2;
                vector<Complex> poly2(deg2);
                for(long long int i = 0; i < deg2; i++){
                    long long int real, imag;
                    cin >> real >> imag;
                    poly2[i] = Complex(real, imag);
                }

                Multiplication<Complex> multiplier(poly1);
                vector<Complex> result = multiplier*(poly2);

                while((result.size() > deg1 + deg2 - 1) && !result.empty() && result.back().check()){
                    result.pop_back();
                }

                for(Complex &x : result){
                    cout << x << " ";
                }
                cout << endl;
            }
        }
        else if (op == 2){  //Evaluation
            string type;
            cin >> type;
            long long int deg;
            cin >> deg;

            if(type == "integer"){
                vector<long long int> poly(deg);
                for(long long int i = 0; i < deg; i++){
                    cin >> poly[i];
                }
                long long int x;
                cin >> x;
                Evaluation<long long int> evaluator;
                cout << evaluator.evaluate(poly, x) << endl;
            } 
            else if (type == "float"){
                vector<long double> poly(deg);
                for (long long int i = 0; i < deg; i++){
                    cin >> poly[i];
                }
                long long int x;
                cin >> x;
                Evaluation<long double> evaluator;
                cout << fixed << setprecision(6) << evaluator.evaluate(poly, x) << endl;
            } 
            else if(type == "string"){
                vector<string> poly(deg);
                for(long long int i = 0; i < deg; i++){
                    string temp;
                    cin >> temp;
                    poly[i] = string(temp);
                }
                long long int x;
                cin >> x;
                Evaluation<string> evaluator;
                //sString res = evaluator.evaluate(poly, x);
                //res.reverseStr();
                cout << evaluator.evaluate(poly, x) << endl;
            }
        }
        else if (op == 3){  // Differentiation
            string type;
            cin >> type;
            long long int deg;
            cin >> deg;
            
            if(type == "integer"){
                vector<long long int> poly(deg);
                for(long long int i = 0; i < deg; i++){
                    cin >> poly[i];
                }
                Differentiation<long long int> diff;
                vector<long long int> result = diff.differentiate(poly);
                for(long long int x : result) cout << x << " ";
                cout << endl;
            } 
            else if(type == "float"){
                vector<long double> poly(deg);
                for(long long int i = 0; i < deg; i++){
                    cin >> poly[i];
                }
                Differentiation<long double> diff;
                vector<long double> result = diff.differentiate(poly);
                for(long double x : result) cout << fixed << setprecision(6) << x << " ";
                cout << endl;
            }
        }

    }
    return 0;
}



        

#include <bits/stdc++.h>
using namespace std;

class CurrencySorter{
    public:
        CurrencySorter(){};

        void operator()(vector<int> &vec){
            vec = quickSort(vec);
        }
    private:
        vector<int> quickSort(vector<int> &vec){
            sort(vec.begin(), vec.end());
            return vec;
        }
};

class FibonacciGenerator{
    unordered_map<int, int> ump;
    public:
        FibonacciGenerator(){
            ump[0] = 0;
            ump[1] = 1;
        }

        int operator()(int i){
            if(i < 0) return -1;

            if(ump.find(i) != ump.end()){
                return ump[i];
            }

            ump[i] = (*this)(i-1) + (*this)(i-2);
            return ump[i];
        }
};

class PrimeCalculator{
    private:
        int l, r;
        vector<int> primes;

        bool isPrime(int n){
            if(n == 0 || n == 1) return false;

            for(int i = 2; (i*i) <= n; i++){
                if(n % i == 0) return false;
            }
            return true;
        }

        void findPrimes(){
            for(int i = l; i<r+1; i++){
                if(isPrime(i)) primes.push_back(i);
            }
        }

    public:
        PrimeCalculator(int i, int j) : l(i), r(j) {
            findPrimes();
        }

        void printPrimes(int l, int r){
            for(auto p : primes){
                if(p <= r && p >= l) cout << p << " ";
            }
            cout << endl;
        }
        

        int printPrimeSum(){
            int sum = 0;
            for(int i = l; i < r+1; i++){
                if(isPrime(i)){
                    sum += i;
                }
            }
            return sum;
        }
};

class NumberAnalyzer{
    private:
        int x;
        vector<int> div;

        bool isSquare(int x){
            for(int i = 0; (i*i)<= x; i++){
                if(i*i == x) return true;
            }
            return false;
        }
    
    public:
        NumberAnalyzer(int n) : x(n) {};

        void findDiv(){
            for(int i = 1; (i*i)<= x; i++){
                if(x % i == 0) {
                    div.push_back(i);
                }
            }
        }

        bool isSquareFree(){
            for(size_t i = 0; i<div.size(); i++){
                if(isSquare(div[i])) return false;
            }
            return true;
        }

        int countDivisors(){
            return div.size();
        }

        int sumOfDivisors(){
            int sum = 0;
            for(auto d : div) sum += d;
            return sum; 
        }

};


int main(){
    int type;
    cin >> type;

    if(type == 1){
        int t;
        cin >> t;
        while(t--){
            int n;
            cin >> n;

            vector<int> arr(n);
            for(int i = 0; i<n; i++){
                cin >> arr[i];
            }
            CurrencySorter curr;
            curr(arr);
            for(auto x : arr) cout << x << " ";
            cout << endl;
        }
    }

    else if(type == 2){
        int t;
        cin >> t;
        int mod = 1000000007;
        FibonacciGenerator fib;
        
        while(t--){
            int i;
            cin >> i;
            cout << (fib(i)%(mod))<< endl;
        }
    }

    else if(type == 3){
        int t;
        cin >> t;
        vector<string> commands;
        vector<pair<int, int>> points;
        int minL = INT_MAX;
        int maxR = INT_MIN;

        while(t--){
            string c;
            int l, r;
            cin >> c;
            commands.push_back(c);
            cin >> l >> r;
            points.push_back({l, r});
            minL = min(l, minL);
            maxR = max(r, maxR);
        }

        PrimeCalculator p(minL, maxR);

        for(size_t i = 0; i<commands.size(); i++){
            if(commands[i] == "printPrimes"){
                p.printPrimes(points[i].first, points[i].second);
            }
    
            else if(commands[i] == "printPrimeSum"){
                cout << p.printPrimeSum() << endl;
            }
        }
    }
    
    else if(type == 4){
        int t;
        cin >> t;

        while(t--){
            string c;
            int x;
            cin >> c >> x;
            NumberAnalyzer num(x);

            if(c == "isSquareFree"){
                if(num.isSquareFree()) cout << "yes" << endl;
                else cout << "no" << endl;
            }

            else if (c == "countDivisors") {
                cout << num.countDivisors() << endl;
            }

            else if(c == "sumOfDivisors") {
                cout << num.sumOfDivisors() << endl;
            }
        }
    }

    return 0;
}

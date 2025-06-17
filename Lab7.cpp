#include <bits/stdc++.h>
using namespace std;

class Comparator{
    private:
        string type;

    public:
        Comparator(string order) : type(order){}

        bool operator()(long long a, long long b){
            if(type == "descending"){
                return a >= b;
            } 
            return a <= b;
        }
};

class Matrix{
    private:
        vector<vector<long long>> mat;
        long long n;

        void insertionSort(vector<long long> &vec, long long left, long long right, Comparator comparator){
            for(long long i = left + 1; i <= right; i++){
                long long key = vec[i];
                long long j = i - 1;
                while(j >= left && !comparator(vec[j], key)){ 
                    vec[j + 1] = vec[j];
                    j--;
                }
                vec[j+1] = key;
            }
        }

        void merge(vector<long long> &vec, long long left, long long mid, long long right, Comparator comparator){
            vector<long long> temp;
            long long i = left, j = mid + 1;

            while(i <= mid && j <= right){
                if(comparator(vec[i], vec[j])){
                    temp.push_back(vec[i]);
                    i++;
                }
                else{
                    temp.push_back(vec[j]);
                    j++;
                }
            }

            while(i <= mid) temp.push_back(vec[i++]);
            while(j <= right) temp.push_back(vec[j++]);

            for(long long k = left; k <= right; k++) vec[k] = temp[k - left];
        }

        void mergeSort(vector<long long> &vec, long long left, long long right, Comparator comparator){
            if(right - left + 1 <= 32){ 
                insertionSort(vec, left, right, comparator);
                return;
            }
            if(left >= right) return;

            long long mid = left + (right - left) / 2;
            mergeSort(vec, left, mid, comparator);
            mergeSort(vec, mid + 1, right, comparator);
            merge(vec, left, mid, right, comparator);
        }

        long long mergeCount(vector<long long> &vec, long long left, long long  mid, long long right){
            long long count = 0;
            vector<long long> temp;
            long long i = left, j = mid+1;

            while(i <= mid && j<=right){
                if(vec[i] <= vec[j]){
                    temp.push_back(vec[i]);
                    i++;   
                }
                else{
                    temp.push_back(vec[j]);
                    j++;
                    count += (mid - i + 1);
                }
            }

            while(i <= mid){ temp.push_back(vec[i]); i++; }
            while(j <= right){ temp.push_back(vec[j]); j++; }

            for(long long k = left; k <=right; k++) vec[k] = temp[k - left];

            return count;
        }

        long long mergeSortCount(vector<long long> &vec, long long l, long long r){
            if(l >= r) return 0;

            long long mid = l + (r-l)/2;
            long long count = mergeSortCount(vec, l, mid);
            count += mergeSortCount(vec, mid+1, r);
            count += mergeCount(vec, l, mid, r);
            return count;
        }

    public:
        Matrix(long long sz = 0){
            n = sz;
            for(long long i = 0; i<sz; i++){
                vector<long long> row;
                for(long long j = 0; j<sz; j++){
                    long long x;
                    cin >> x; 
                    row.push_back(x);
                }
                mat.push_back(row);
            }
        }

        void sortRows(Comparator comparator){
            for(long long i = 0; i<n; i++){
                mergeSort(mat[i], 0, mat[i].size()-1, comparator);
            }
        }

        long long countInversions(){
            vector<long long> flat;

            for(auto row : mat){
                for(long long x : row){
                    flat.push_back(x);
                }
            }
            return mergeSortCount(flat, 0, flat.size() -1);
        }

        void display(){
            for(long long i = 0; i<n; i++){
                for(long long j = 0; j<n; j++){
                    cout << mat[i][j] << " ";
                }
                cout << endl;
            }
        }
};

class Stations{
    private:
        struct Point{
            long long x, y, index;
        };
    
        long long dist(Point &p, Point &q){
            long long dx = (long long)p.x - q.x;
            long long dy = (long long)p.y - q.y;
            return dx * dx + dy * dy;
        }
    
        bool closerPair(pair<Point, Point> &a, pair<Point, Point> &b){
            long long da = dist(a.first, a.second);
            long long db = dist(b.first, b.second);
        
            if(da != db) return da < db;
            if(a.first.x != b.first.x) return a.first.x < b.first.x;
            if(a.first.y != b.first.y) return a.first.y < b.first.y;
            if(a.second.x != b.second.x) return a.second.x < b.second.x;
            if(a.second.y != b.second.y) return a.second.y < b.second.y;
            if(a.first.index != b.first.index) return a.first.index < b.first.index;
            return a.second.index < b.second.index;
        }

        pair<Point, Point> acrossPair(vector<Point> &strip, long long minD, pair<Point, Point> closest){
            sort(strip.begin(), strip.end(), [](Point &a, Point &b){
                return a.y < b.y;
            });
    
            long long sz = strip.size();
            for(long long i = 0; i < sz; i++){
                for(long long j = i + 1; j < sz && pow((strip[j].y - strip[i].y), 2) < minD; j++){
                    pair<Point, Point> candidate = {strip[i], strip[j]};
                    if (closerPair(candidate, closest)) closest = candidate;
                }
            }
            return closest;
        }
    
        pair<Point, Point> findClosest(vector<Point> &pts, long long left, long long right){
            if (right - left + 1 <= 3) {
                pair<Point, Point> closest = {pts[left], pts[left + 1]};
                for (long long i = left; i <= right; i++){
                    for (long long j = i + 1; j <= right; j++){
                        pair<Point, Point> candidate = {pts[i], pts[j]};
                        if (closerPair(candidate, closest)) closest = candidate;
                    }
                }
                return closest;
            }
    
            long long mid = (left + right) / 2;
            Point midpt = pts[mid];
    
            auto qPair = findClosest(pts, left, mid);
            auto rPair = findClosest(pts, mid + 1, right);
            auto minDist = closerPair(qPair, rPair) ? qPair : rPair;
            long long delta = dist(minDist.first, minDist.second);
    
            vector<Point> strip;
            for (long long i = left; i <= right; i++){
                if (((pts[i].x - midpt.x)*(pts[i].x - midpt.x)) < delta) strip.push_back(pts[i]);
            }
            return acrossPair(strip, delta, minDist);
        }
    
    public:
        void closestPair(long long num){
            vector<Point> points(num);

            for(long long i = 0; i < num; i++){
                cin >> points[i].x >> points[i].y;
                points[i].index = i;
            }
    
            if(num == 2){
                cout << points[0].x << " " << points[0].y << " "
                     << points[1].x << " " << points[1].y << endl;
                return;
            }

            sort(points.begin(), points.end(), [](Point &a, Point &b){
                return a.x < b.x;
            });
    
            auto res = findClosest(points, 0, num - 1);
            if (res.first.index > res.second.index){
                Point temp = res.first;
                res.first = res.second;
                res.second = temp;
            }
    
            cout << res.first.x << " " << res.first.y << " "
                 << res.second.x << " " << res.second.y << endl;
        }
    };

int main(){
    string command;
    Matrix mat;
    Stations st;

    while(cin >> command && command != "END"){

        if(command == "CREATE_2D"){
            long long sz;
            cin >> sz;
            mat = Matrix(sz);   
        }

        else if(command == "DISPLAY_2D"){
            mat.display();
        }

        else if(command == "INVERSION_2D"){
            cout << mat.countInversions() << endl;
        }

        else if(command == "SORT_2D"){
            string type; 
            cin >> type;
            
            Comparator comp(type);
            mat.sortRows(comp);
        }

        else if(command == "CLOSEST_2D"){
            long long num_points;
            cin >> num_points;
            st.closestPair(num_points);
        }

        else if(command == "END"){
            break;
        }
    }
    return 0;
}

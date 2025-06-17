#include <bits/stdc++.h>
using namespace std;

const long double val = 1e-8;

struct Point{
    long double x, y;
    Point operator-(const Point& p) const{ return{x - p.x, y - p.y}; }
    long double cross(const Point& p) const{ return x * p.y - y * p.x; }
    long double dot(const Point& p) const{ return x * p.x + y * p.y; }
    long double dist2(const Point& p) const{
        long double dx = x - p.x, dy = y - p.y;
        return dx * dx + dy * dy;
    }
};

bool onSegment(Point p, Point q, Point r){
    return min(p.x, r.x) - val <= q.x && q.x <= max(p.x, r.x) + val &&
           min(p.y, r.y) - val <= q.y && q.y <= max(p.y, r.y) + val;
}

int orientation(Point a, Point b, Point c){
    long double val = (b - a).cross(c - a);
    if (fabs(val) < val) return 0;
    return (val > 0) ? 1 : 2;
}

bool segmentsIntersect(Point p1, Point q1, Point p2, Point q2){
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    if (o1 != o2 && o3 != o4) return true;

    return (o1 == 0 && onSegment(p1, p2, q1)) ||
           (o2 == 0 && onSegment(p1, q2, q1)) ||
           (o3 == 0 && onSegment(p2, p1, q2)) ||
           (o4 == 0 && onSegment(p2, q1, q2));
}

class Island{
protected:
    string id;
public:
    virtual ~Island(){}
    virtual bool touches(const Island* other) const = 0;
    virtual string getId() const{ return id; }
    virtual vector<Point> getEndPoints() const = 0;

    friend istream& operator>>(istream& in, Island*& island);
    friend ostream& operator<<(ostream& out, const Island& island);
};

class Triangle : public Island{
    Point pts[3];
public:
    Triangle(string id_, vector<Point> p){
        id = id_;
        for(int i = 0; i < 3; i++) pts[i] = p[i];
    }
    bool touches(const Island* other) const;
    vector<Point> getEndPoints() const {
        return{pts[0], pts[1], pts[2]};
    }
};

class Rectangle : public Island{
    Point pts[4];
public:
    Rectangle(string id_, vector<Point> p){
        id = id_;
        for(int i = 0; i < 4; i++) pts[i] = p[i];
    }
    bool touches(const Island* other) const;
    vector<Point> getEndPoints() const {
        return{pts[0], pts[1], pts[2], pts[3]};
    }
};

class Circle : public Island{
    Point center;
    long double radius;
public:
    Circle(string id_, Point c, long double r){
        id = id_; center = c; radius = r;
    }
    bool touches(const Island* other) const;
    vector<Point> getEndPoints() const {
        return{center};
    }
};

istream& operator>>(istream& in, Island*& island){
    string type, id;
    in >> type >> id;
    if (type == "TRIANGLE"){
        vector<Point> p(3);
        for(int i = 0; i < 3; i++) in >> p[i].x >> p[i].y;
        island = new Triangle(id, p);
    }
    else if (type == "RECTANGLE"){
        vector<Point> p(4);
        for(int i = 0; i < 4; i++) in >> p[i].x >> p[i].y;
        island = new Rectangle(id, p);
    }
    else if (type == "CIRCLE"){
        Point c; long double r;
        in >> c.x >> c.y >> r;
        island = new Circle(id, c, r);
    }
    return in;
}

ostream& operator<<(ostream& out, const Island& island){
    out << island.getId();
    return out;
}

bool Intersection(const vector<Point>& a, const vector<Point>& b){
    int na = a.size(), nb = b.size();
    for(int i = 0; i < na; i++){
        Point p1 = a[i], q1 = a[(i+1)%na];
        for(int j = 0; j < nb; ++j){
            Point p2 = b[j], q2 = b[(j+1)%nb];
            if (segmentsIntersect(p1, q1, p2, q2)) return true;
        }
    }
    return false;
}

bool Circle::touches(const Island* other) const{
    auto pts = other->getEndPoints();
    if (dynamic_cast<const Circle*>(other)){
        const Circle* c = dynamic_cast<const Circle*>(other);
        long double d2 = center.dist2(c->center);
        long double rsum = radius + c->radius;
        return d2 <= rsum * rsum + val;
    } else{
        //polygon-circle
        int n = pts.size();
        for(int i = 0; i < n; i++){
            Point p1 = pts[i], p2 = pts[(i+1)%n];
            //project center to segment
            Point d = p2 - p1, f = center - p1;
            long double t = f.dot(d) / d.dot(d);
            t = max(0.0L, min(1.0L, t));
            Point proj ={p1.x + t * d.x, p1.y + t * d.y};
            if (center.dist2(proj) <= radius * radius + val)
                return true;
        }
    }
    return false;
}

bool Rectangle::touches(const Island* other) const{
    if (dynamic_cast<const Circle*>(other))
        return other->touches(this);
    return Intersection(this->getEndPoints(), other->getEndPoints());
}

bool Triangle::touches(const Island* other) const{
    if (dynamic_cast<const Circle*>(other))
        return other->touches(this);
    return Intersection(this->getEndPoints(), other->getEndPoints());
}

int main(){
    int n;
    cin >> n;

    vector<Island*> islands(n);
    for(int i = 0; i < n; i++) cin >> islands[i];

    vector<vector<int>> adj(n);
    for(int i = 0; i < n; i++)
        for(int j = i + 1; j < n; ++j)
            if (islands[i]->touches(islands[j])){
                adj[i].push_back(j);
                adj[j].push_back(i);
            }

    int full = (1 << n);
    vector<vector<bool>> dp(full, vector<bool>(n, false));
    vector<vector<int>> parent(full, vector<int>(n, -1));

    for(int i = 0; i < n; i++) dp[1 << i][i] = true;

    for(int mask = 0; mask < full; ++mask)
        for(int u = 0; u < n; ++u)
            if (dp[mask][u])
                for(int v : adj[u])
                    if (!(mask & (1 << v))){
                        int next = mask | (1 << v);
                        if (!dp[next][v]){
                            dp[next][v] = true;
                            parent[next][v] = u;
                        }
                    }

    vector<int> path;
    bool found = false;

    for(int u = 0; u < n; ++u){
        if (dp[full - 1][u]){
            int mask = full - 1;
            while (u != -1){
                path.push_back(u);
                int pu = parent[mask][u];
                mask ^= (1 << u);
                u = pu;
            }
            reverse(path.begin(), path.end());
            found = true;
            break;
        }
    }

    if (found){
        cout << "YES\n";
    } else{
        cout << "NO\n";
        int maxLen = 0;
        for(int mask = 0; mask < full; ++mask)
            for(int u = 0; u < n; ++u)
                if (dp[mask][u]){
                    int len = __builtin_popcount(mask);
                    if (len > maxLen){
                        maxLen = len;
                        path.clear();
                        int cur = u, m = mask;
                        while (cur != -1){
                            path.push_back(cur);
                            int p = parent[m][cur];
                            m ^= (1 << cur);
                            cur = p;
                        }
                        reverse(path.begin(), path.end());
                    }
                }
        cout << path.size() << "\n";
    }

    for(int i : path)
        cout << islands[i]->getId() << " ";
    cout << "\n";

    for(auto i : islands) delete i;
    return 0;
}


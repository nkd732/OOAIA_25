#include <bits/stdc++.h>
using namespace std;

template<typename T>
class minPQ{  //minimum priority queue class
    private:
        vector<T> arr;

        void percolateUp(size_t ind){  
            T val = move(arr[ind]);
            while(ind > 0){
                size_t parent = (ind-1)/2;
                if(val < arr[parent]){
                    arr[ind] = move(arr[parent]);
                    ind = parent;
                }
                else break;
            }
            arr[ind] = move(val);
        }

        void percolateDown(size_t ind){
            size_t l, r, eMin; //l = left, r = right, smallest element = eMin
            T val = move(arr[ind]);

            while((l = 2*ind +1) < arr.size()){
                r = l + 1;
                eMin = (r < arr.size() && arr[r] < arr[l]) ? r : l;
                if(arr[eMin] < val){
                    arr[ind] = move(arr[eMin]);
                    ind = eMin;
                }
                else break;
            }
            arr[ind] = move(val);
        }

    public:
        minPQ(size_t cap = 0){
            if(cap > 0) arr.reserve(cap);  //cap = capacity
        }

        const size_t size() const { return arr.size(); }  //returns size of priority queue

        void push(const T& val){  //to insert into pq
            arr.emplace_back(val);
            percolateUp(arr.size() - 1);  //call percolateUp to preserve order (min pq)
        }

        void pop(){  //to delete top element of pq
            if(arr.empty()) return;
            arr[0] = arr.back();
            arr.pop_back();
            if(!arr.empty()) percolateDown(0); //call percolateUp to preserve order (min pq)
        };

        const T& top() const{ return arr.front(); }  //to peek the top ele of pq
        const bool empty() const {return arr.empty(); } //to check if pq is empty
};

class Node{  //Node class
    private: 
        string roomID; //id of vertex
        long long dist;  //dist of vertex from src;
        long long parity;

    public:
        Node(string id, long long d, long long p) : roomID(id), dist(d), parity(p) {}  //constructor

        bool operator<(const Node& other){
            return dist > other.dist;  //custom comparision operator for distance 
        }

        long long getDist() { return dist; }
        string getID() { return roomID; }
        long long getParity() { return parity; }
};

//dijkstra algo:
long long dijkstra(unordered_map<string, vector<pair<string, long long>>>& graph, string &src, string &dest){
    unordered_map<string, vector<long long>> dists;  //map of roomID to dist
    //stores roomID with even path distance, odd path distance

    for(auto& room : graph){
        dists[room.first] = {INT_MAX, INT_MAX};
    }

    dists[src][0] = 0; //intialize even length path distance to 0

    minPQ<Node*> pq;
    pq.push(new Node(src, 0, 0));

    while(!pq.empty()){
        Node* curr = pq.top();
        pq.pop();

        string id = curr->getID();
        long long d = curr->getDist();
        long long p = curr->getParity();
        delete curr;

        if(d > dists[id][p]) continue;  //skip if shortest even length dist was already found

        for(auto& nbor : graph.at(id)){
            string nborId = nbor.first;
            long long wght = nbor.second;
            long long nborP = 1 - p; //nbor parity

            long long newDist = d + wght; //curr vertex distnce frm src plus wght of edge

            if(newDist < dists[nborId][nborP]){
                dists[nborId][nborP] = newDist;
                pq.push(new Node(nborId, newDist, nborP));
            }
        }
    }

    if(dists[dest][0] == INT_MAX) return -1;
    return dists[dest][0];

}

int main(){
    long long n, m;
    cin >> n >> m;  //n = no of rooms, m = no of edges

    unordered_map<string, vector<pair<string, long long>>> graph;  //graph
    vector<string> rooms(n);  //vector of rooms(vertices);

    for(long long i = 0; i < n; i++){
        cin >> rooms[i];
        graph[rooms[i]] = {};   //set adjList of each room to empty intially
    }

    for(long long i = 0; i < m; i++){
        string a, b; //room ids
        long long w;  //the weight of edge btween them

        cin >> a >> b >> w; 
        graph[a].push_back({b, w});  //insert into adjList vector
        graph[b].push_back({a, w});
    }

    string src, dest;
    cin >> src >> dest;  //take in ids of source and destination

    cout << dijkstra(graph, src, dest) << endl;  //call dijkstra algo on graph
    return 0;
}


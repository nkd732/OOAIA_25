#include <bits/stdc++.h>
using namespace std;

// declaring global variables in case of repeated query calling for same graph
bool flag = false;
vector<int> topo;
int maxV = 0;
int num = 0;
int maxH = 0;

vector<int> comp;
vector<int> cardin;
vector<vector<int>> condensedG;
vector<int> hypeSum;

// class graph to store graph data
class Graph{
    private:
        int nv;

    public:
        vector<vector<int>> adjList;
        vector<vector<int>> revAdjList;

        Graph(int n){
            nv = n;
            adjList.resize(n + 1);
            revAdjList.resize(n+1);
        }

        void addEdge(int u, int v){
            if (u <= 0 || v <= 0) {
                //cout << "Invalid edge detected: (" << u << ", " << v << ")" << endl;
                return;
            }
            //cout << u << " " << v  << endl;
            adjList[u].push_back(v);
            revAdjList[v].push_back(u);
        }
    
        //vector<vector<int>> getAdjList(){ return adjList; }
        int getSize(){ return nv; }
};

// class with virtual function
class GraphAlgorithm{
    public:
        virtual void Query() = 0;
};

class isCycle : public GraphAlgorithm{
    private:
        Graph& g;

        void dfsCycle(int u, vector<int>& color){
            if(flag) return;  
            //cout << "checking: " << u << endl;
            color[u] = 1; 
            
            for(int v : g.adjList[u]){
                //if(v == 0) continue;
                //if (flag)  return;  
                
                if(color[v] == 0){ 
                    //cout << "vertex: " << v << endl;
                    dfsCycle(v, color);
                } 
                else if(color[v] == 1){  
                    flag = true;
                    return;
                }
            }

            color[u] = 2; 
            //cout << "done : " << u  << endl;
            return;
        }
    
        void isCyclic(){
            flag = false;
            int nv = g.getSize();
            vector<int> color(nv + 1, 0);  
        
            for(int i = 1; i <= nv; i++){
                if(color[i] == 0){
                    dfsCycle(i, color);
                    if(flag) return; 
                }
            }
        }
        

    public:
        isCycle(Graph& G) : g(G) {}

        void Query(){
            isCyclic();
        }
};

class validOrder: public GraphAlgorithm{
    private:
        Graph& g;

        void findTopo(queue<int> &q, vector<int>& topo){
            int nv = g.getSize();
            vector<int> inDeg(nv+1, 0);

            for(int i = 1; i <= nv; i++){
                for(int v : g.adjList[i]){
                    inDeg[v]++;
                }
            }   

            for(int i = 1; i<= nv; i++){
                if(inDeg[i] == 0){
                    q.push(i);
                }
            }

            while(!q.empty()){
                int u = q.front();
                q.pop();
                topo.push_back(u);
            
                for(int v : g.adjList[u]){
                    inDeg[v]--;
                    if(inDeg[v] == 0){
                        q.push(v);
                    }
                }
            }
        }


    public:
        validOrder(Graph& G) : g(G) {}
        
        void Query(){
            if(flag) return;
            queue<int> q;
            topo.clear();
            findTopo(q, topo);
        }
};


//indepComponent child class
class indepComponent: public GraphAlgorithm{
    private:
        Graph &g;
        vector<bool> vis;
        stack<int> st;
    
        void dfs1(int u){
            vis[u] = true;
            for(int v : g.adjList[u]){
                if(!vis[v]) dfs1(v);
            }
            st.push(u);
        }
    
        void dfs2(int u, int &sz, int sccId){
            vis[u] = true;
            sz++;
            comp[u] = sccId; //assign an sccID
            for(int v : g.revAdjList[u]){
                if(!vis[v]) dfs2(v, sz, sccId);
            }
        }
    
    public:
        indepComponent(Graph &G) : g(G) {}
    
        void findSCC(){
            int nv = g.getSize();
            num = 0;
            vis.assign(nv + 1, false);
            comp.assign(nv + 1, -1);
            cardin.clear();
            hypeSum.clear();
            condensedG.clear();
    
            //dfs on original graph
            for (int i = 1; i <= nv; i++){
                if (!vis[i]) dfs1(i);
            }
    
            //dfs on transposed graph
            vis.assign(nv + 1, false);
            while(!st.empty()){
                int u = st.top();
                st.pop();
                if(!vis[u]){
                    int sz = 0;
                    dfs2(u, sz, num);
                    cardin.push_back(sz);
                    hypeSum.push_back(0);  //hypesum intialized for SCC
                    maxV = max(maxV, sz);
                    num++;
                }
            }
        }

        void Query(){
            findSCC();
        }
     
};

class maxHype : public GraphAlgorithm{
    private:
        Graph &g;
        vector<int> &hypes;
    
        void makeCondensedG(){
            set<pair<int, int>> edges;
            int nv = g.getSize();
            condensedG.resize(num); //using global num (number of SCCs)
    
            for(int u = 1; u <= nv; u++){
                for(int v : g.adjList[u]){
                    if(comp[u] != comp[v]){
                        edges.insert({comp[u], comp[v]});
                    }
                }
            }
    
            for(auto e : edges){
                condensedG[e.first].push_back(e.second);
            }
        }
    
        void findHypePath(){
            vector<int> dp(num, 0);
            vector<int> indeg(num, 0);
    
            for(int u = 0; u < num; u++){
                for(int nv : condensedG[u]){
                    indeg[nv]++;
                }
            }
    
            queue<int> q;
            for(int i = 0; i < num; i++){
                if(indeg[i] == 0){
                    q.push(i);
                    dp[i] = hypeSum[i]; //use global hypeSum
                }
            }
    
            while(!q.empty()){
                int u = q.front();
                q.pop();
                maxH = max(maxH, dp[u]);
    
                for (int nv : condensedG[u]){
                    dp[nv] = max(dp[nv], dp[u] + hypeSum[nv]);
                    indeg[nv]--;
                    if (indeg[nv] == 0){
                        q.push(nv);
                    }
                }
            }
        }
    
    public:
        maxHype(Graph &G, vector<int> &h) : g(G), hypes(h) {}
    
        void Query(){
            for(int u = 1; u <= g.getSize(); u++){
                if(comp[u] >= 0 && comp[u] < num){
                    hypeSum[comp[u]] += hypes[u - 1];
                }
            }
    
            makeCondensedG();
            findHypePath();
        }
    };
    

int main(){
    int n, m;   //no of events and no of edges(dependencies)
    cin >> n >> m;
 

    Graph g(n);

    vector<int> hypes;
    for(int i = 0; i<n; i++){
        int h;
        cin >> h;
        hypes.push_back(h);
    }


    for(int i = 0; i<m; i++){  //adding edges(dependencies)
        int u, v;
        cin >> u >> v;
        g.addEdge(u, v);
    }

    GraphAlgorithm* gQuery = new isCycle(g);  
    gQuery->Query();

    
    gQuery = new indepComponent(g);
    gQuery->Query();


    gQuery = new validOrder(g);
    gQuery->Query();

    gQuery = new maxHype(g, hypes);
    gQuery->Query();

    //stored all the global variables for the Graph g;

    int q;
    cin >> q; //taking no of queries

    //outputing the result(stored global variables) based on the query type
    while(q--){
        int type;
        cin >> type;

        if(type == 1){
           if(flag) cout << "YES" << endl;
           else cout << "NO" << endl;
           //cout << "okks" << endl;
        }

        else if(type == 2){
           cout << num << " " << maxV << endl;
        }

        else if(type == 3){
            if(flag) cout << "NO" << endl;
            else{
                for(auto i : topo){
                    cout << i << " ";
                }
                cout << endl;
            }
        }

        else if(type == 4){
            cout << maxH << endl;
        }
    }

    return 0;
}




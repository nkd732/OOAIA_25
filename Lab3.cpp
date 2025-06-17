
#include <bits/stdc++.h>
using namespace std;
class Graph{
private:
unordered_map<int, set<int>> adjList;
int nv;
public:
Graph(int nv = 0){
this->nv = nv;
for(int i = 0; i<nv; i++){
adjList[i] = {};
}
}
void add_edge(int u, int v){
adjList[u].insert(v);
adjList[v].insert(u);
}
void remove_edge(int u, int v){
if(adjList.count(u)) adjList[u].erase(v);
if(adjList.count(v)) adjList[v].erase(u);
}
bool isReachable(int u, int v){
if(u == v) return true;
if(adjList.find(u) == adjList.end() || adjList.find(v) == adjList.end()) return false;
unordered_set<int> vis;
queue<int> q;
q.push(u);
vis.insert(u);
while(!q.empty()){
int node = q.front();
q.pop();
for(int nbor : adjList[node]){
if(nbor == v) return true;
if(!vis.count(nbor)){
q.push(nbor);
vis.insert(nbor);
}
}
}
return false;
}
Graph operator+(Graph& other){
Graph result(max(nv, other.nv));
result.adjList = adjList;
for(int i = 0; i<max(nv, other.nv); i++){
if(adjList.find(i) == adjList.end()){
result.adjList[i] = other.adjList[i];
}
else if(other.adjList.find(i) == other.adjList.end()){
result.adjList[i] = adjList[i];
}
else{
for(int v : other.adjList[i]){
result.adjList[i].insert(v);
}
}
}
return result;
}
Graph operator-(Graph& other){
Graph result(max(nv, other.nv));
result.adjList = adjList;
for(int i = 0; i<max(nv, other.nv); i++){
if(adjList.find(i) == adjList.end() || other.adjList.find(i) == other.adjList.end()){
result.adjList[i] = {};
}
else{
for(int v : adjList[i]){
if(!other.adjList[i].count(v)){ result.adjList[i].erase(v); }
}
}
}
return result;
}
Graph operator!(){
Graph comple(nv);
set<int> s;
for(int i = 0; i< nv; i++) s.insert(i);
for(int i = 0; i<nv; i++){
comple.adjList[i] = s;
comple.adjList[i].erase(i);
}
for(int i = 0; i<nv; i++){
for(int v : adjList[i]){
comple.adjList[i].erase(v);
}
}
return comple;
}
friend istream& operator>>(istream& is, Graph& G){
int ne;
is >> G.nv >> ne;
for(int i = 0; i < ne; i++){
int u, v;
is >> u >> v;
G.add_edge(u, v);
}
return is;
}
friend ostream& operator<<(ostream& os, Graph& G){
for(int i = 0; i < G.nv; i++) {
os << "Vertex " << i << ": ";
if (G.adjList.find(i) != G.adjList.end()) {
for (int v : G.adjList.at(i)) {
os << v << " ";
}
}
if(i != G.nv-1) os << endl;
}
return os;
}
};
int main(){
Graph G;
string command;
while(cin >> command && command != "end"){
if(command == "Graph"){
cin >> G;
}
else if(command == "union"){
Graph other;
string g;
cin >> g;
cin >> other;
G = G + other;
}
else if(command == "intersection"){
Graph other;
string g;
cin >> g;
cin >> other;
G = G - other;
}
else if(command == "complement"){
G = !G;
}
else if(command == "add_edge"){
int u, v;
cin >> u >> v;
G.add_edge(u, v);
}
else if(command == "remove_edge"){
int u, v;
cin >> u >> v;
G.remove_edge(u, v);
}
else if(command == "isReachable"){
int u, v;
cin >> u >> v;
if(G.isReachable(u, v)) cout << "Yes" << endl;
else cout << "No" << endl;
}
else if(command == "printGraph"){
cout << G << endl;
}
else if(command == "end"){
break;
}
}
return 0;
}

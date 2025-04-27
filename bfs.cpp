#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <omp.h>
#include <chrono>
using namespace std;
using namespace std::chrono;
struct Graph {
int V;
vector<vector<int>> adj;
Graph(int V) : V(V), adj(V) {}
void addEdge(int u, int v) {
adj[u].push_back(v);
adj[v].push_back(u);
}
};
void BFS_Normal(Graph& g, int start) {
set<int> visited;
queue<int> q;
visited.insert(start);
q.push(start);
while (!q.empty()) {
int u = q.front();
q.pop();
cout << "[" << u << "] ";
for (int v : g.adj[u]) {
if (visited.find(v) == visited.end()) {
visited.insert(v);
q.push(v);
}
}
}
cout << endl;
}
void BFS_Parallel(Graph& g, int start) {
set<int> visited;
queue<int> q;

visited.insert(start);
q.push(start);
while (!q.empty()) {
int u = q.front();
q.pop();
cout << "[" << u << "] ";
#pragma omp parallel for
for (size_t i = 0; i < g.adj[u].size(); i++) {
int v = g.adj[u][i];
if (visited.find(v) == visited.end()) {
#pragma omp criƟcal
{
visited.insert(v);
q.push(v);
}
}
}
}
cout << endl;
}
void DFSUƟl(Graph& g, int u, vector<bool>& visited) {
visited[u] = true;
cout << "{" << u << "} ";
for (int v : g.adj[u]) {
if (!visited[v]) {
DFSUƟl(g, v, visited);
}
}
}
void DFS_Normal(Graph& g, int start) {
vector<bool> visited(g.V, false);
DFSUƟl(g, start, visited);
cout << endl;
}
void DFS_Parallel(Graph& g, int start) {
vector<bool> visited(g.V, false);
#pragma omp parallel
{
#pragma omp single nowait
{
DFSUƟl(g, start, visited);
}

}
cout << endl;
}
int main() {
int V, edgeCount;
cout << "Input verƟces and edges: ";
cin >> V >> edgeCount;
Graph g(V);
cout << "Edges (format: u v):\n";
for (int i = 0; i < edgeCount; i++) {
int u, v;
cin >> u >> v;
g.addEdge(u, v);
}
// Measure BFS Normal Time
auto start = high_resoluƟon_clock::now();
BFS_Normal(g, 0);
auto stop = high_resoluƟon_clock::now();
cout << "BFS (Normal) Time: " << duraƟon_cast<microseconds>(stop - start).count() << "μs\n";
// Measure BFS Parallel Time
start = high_resoluƟon_clock::now();
BFS_Parallel(g, 0);
stop = high_resoluƟon_clock::now();
cout << "BFS (Parallel) Time: " << duraƟon_cast<microseconds>(stop - start).count() << "μs\n";
// Measure DFS Normal Time
start = high_resoluƟon_clock::now();
DFS_Normal(g, 0);
stop = high_resoluƟon_clock::now();
cout << "DFS (Normal) Time: " << duraƟon_cast<microseconds>(stop - start).count() << "μs\n";
// Measure DFS Parallel Time
start = high_resoluƟon_clock::now();
DFS_Parallel(g, 0);
stop = high_resoluƟon_clock::now();
cout << "DFS (Parallel) Time: " << duraƟon_cast<microseconds>(stop - start).count() << "μs\n";
return 0;
}

Output:
Enter number of verƟces and edges: 6 7
Enter the edges (format: source desƟnaƟon):
0 1
0 2
1 3
1 4
2 4
3 5
4 5
BFS (Normal):
[0] [1] [2] [3] [4] [5]
Time taken for BFS (Normal): 32 microseconds
BFS (Parallel):
[0] [1] [2] [3] [4] [5]
Time taken for BFS (Parallel): 27 microseconds
DFS (Normal):
{0} {1} {3} {5} {4} {2}
Time taken for DFS (Normal): 22 microseconds
DFS (Parallel):
{0} {1} {3} {5} {4} {2}
Time taken for DFS (Parallel): 19 microseconds

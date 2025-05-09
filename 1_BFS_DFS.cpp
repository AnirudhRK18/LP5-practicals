#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

class Graph {
    int V;
    vector<vector<int>> adj;

public:
    Graph(int V) : V(V), adj(V) {}

    void addEdge(int v, int w) {
        adj[v].push_back(w);
        adj[w].push_back(v); // Assuming undirected graph
    }

    void parallelDFS(int startVertex) {
        vector<bool> visited(V, false);

        #pragma omp parallel
        {
            #pragma omp single nowait
            {
                parallelDFSUtil(startVertex, visited);
            }
        }
    }

    void parallelDFSUtil(int v, vector<bool>& visited) {
        // Check if the vertex is visited
        if (visited[v]) {
            return; // Early exit without using OpenMP critical
        }

        // Mark the current vertex as visited
        visited[v] = true;
        cout << v << " ";

        // Iterate over neighbors sequentially
        for (int neighbor : adj[v]) {
            // If the neighbor is not visited, process it in parallel
            if (!visited[neighbor]) {
                parallelDFSUtil(neighbor, visited);  // Sequential call to avoid issues
            }
        }
    }

    void parallelBFS(int startVertex) {
        vector<bool> visited(V, false);
        queue<int> q;

        visited[startVertex] = true;
        q.push(startVertex);

        #pragma omp parallel
        {
            #pragma omp single
            {
                while (!q.empty()) {
                    int v;

                    #pragma omp critical
                    {
                        if (!q.empty()) {
                            v = q.front();
                            q.pop();
                            cout << v << " ";
                        } else {
                            v = -1;
                        }
                    }

                    if (v == -1) continue;

                    for (int neighbor : adj[v]) {
                        #pragma omp critical
                        {
                            if (!visited[neighbor]) {
                                visited[neighbor] = true;
                                q.push(neighbor);
                            }
                        }
                    }
                }
            }
        }
    }
};

int main() {
    int V, E;
    cout << "Enter number of vertices: ";
    cin >> V;
    cout << "Enter number of edges: ";
    cin >> E;

    Graph g(V);
    cout << "Enter edges (u v):" << endl;
    for (int i = 0; i < E; ++i) {
        int v, w;
        cin >> v >> w;
        g.addEdge(v, w);
    }

    int startVertex;
    cout << "Enter starting vertex for DFS and BFS: ";
    cin >> startVertex;

    cout << "Parallel Depth-First Search (DFS): ";
    g.parallelDFS(startVertex);
    cout << endl;

    cout << "Parallel Breadth-First Search (BFS): ";
    g.parallelBFS(startVertex);
    cout << endl;

    return 0;
}

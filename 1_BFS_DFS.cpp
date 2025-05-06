#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

class Graph {
    int V;
    vector<vector<int>> adj;

public:
    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u); // Undirected graph
    }

    void parallelDFS(int start) {
        vector<bool> visited(V, false);

        #pragma omp parallel
        {
            #pragma omp single
            dfsUtil(start, visited);
        }
    }

    void dfsUtil(int v, vector<bool>& visited) {
        bool doVisit = false;

        #pragma omp critical
        {
            if (!visited[v]) {
                visited[v] = true;
                cout << v << " ";
                doVisit = true;
            }
        }

        if (doVisit) {
            for (int neighbor : adj[v]) {
                #pragma omp task
                dfsUtil(neighbor, visited);
            }
        }

        #pragma omp taskwait
    }

    void parallelBFS(int start) {
        vector<bool> visited(V, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

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
    for (int i = 0; i < E; i++) {
        int u, v;
        cin >> u >> v;
        g.addEdge(u, v);
    }

    int start;
    cout << "Enter starting vertex: ";
    cin >> start;

    cout << "Parallel DFS: ";
    g.parallelDFS(start);
    cout << "\nParallel BFS: ";
    g.parallelBFS(start);
    cout << endl;

    return 0;
}

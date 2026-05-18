#include <iostream>
#include <vector>
#include <omp.h>
#include <queue>
using namespace std;

class Graph
{
    int V;
    vector<int> adj[100];

public:
    Graph(int V)
    {
        this->V = V;
        // adj.resize(V);
    }

    void add_edges(int u, int v)
    {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void parallelBFS(int start)
    {
        vector<int> visited(V, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        cout << "Parallel BFS: ";

        while (!q.empty())
        {
            int node = q.front();
            q.pop();

            cout << node << " ";

#pragma omp parallel for
            for (int i = 0; i < adj[node].size(); i++)
            {
                int neighbor = adj[node][i];

                if (!visited[neighbor])
                {
#pragma omp critical
                    {
                        if (!visited[neighbor])
                        {
                            visited[neighbor] = true;
                            q.push(neighbor);
                        }
                    }
                }
            }
        }
        cout << endl;
    }

    // DFS

    void dfsUtil(int node, int *visited)
    {
        // Print node safely
#pragma omp critical
        {
            cout << node << " ";
        }

        for (int neighbor : adj[node])
        {
            int old_val;

#pragma omp atomic capture
            {
                old_val = visited[neighbor];
                visited[neighbor] = 1;
            }

            if (old_val == 0)
            {
#pragma omp task firstprivate(neighbor)
                dfsUtil(neighbor, visited);
            }
        }

#pragma omp taskwait
    }

    void parallelDFS(int start)
    {
        vector<int> visited(V, 0);
        visited[start] = 1;

        cout << "Parallel DFS:\n";

#pragma omp parallel
        {
#pragma omp single
            {
                dfsUtil(start, visited.data());
            }
        }
        cout << endl;
    }
};

int main()
{
    int V, E;

    cout << "Enter number of vertices: ";
    cin >> V;

    cout << "Enter number of edges: ";
    cin >> E;

    Graph g(V);

    cout << "Enter edges (u v):\n";
    for (int i = 0; i < E; i++)
    {
        int u, v;
        cin >> u >> v;
        g.add_edges(u, v);
    }

    int start;
    cout << "Enter start node: ";
    cin >> start;

    g.parallelDFS(start);
    g.parallelBFS(start);

    return 0;
}

/*
Enter number of vertices: 5
Enter number of edges: 5
Enter edges (u v):
0 1
0 2
1 3
1 4
2 4
Enter start node: 0
*/
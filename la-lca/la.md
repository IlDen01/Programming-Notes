```c++
#include <bits/stdc++.h>

constexpr int MAXLOG = 20;

int n; // количество вершин
std::vector<std::vector<int>> graph;
int m; // количество запросов
std::vector<int> depth;
std::vector<std::vector<int>> up;

void dfs(const int v, const int parent) {
    depth[v] = parent == -1 ? 0 : depth[parent] + 1;
    up[v][0] = parent;
    for (const int u : graph[v]) {
        if (u != parent) {
            dfs(u, v);
        }
    }
}

void build() {
    for (int k = 1; k < MAXLOG; k++) {
        for (int v = 0; v < n; v++) {
            if (up[v][k - 1] != -1) {
                up[v][k] = up[up[v][k - 1]][k - 1];
            }
        }
    }
}

int la(int v, const int d) {
    if (d > depth[v]) {
        return 0;
    }

    for (int k = 0; k < MAXLOG; k++) {
        if (d >> k & 1) {
            v = up[v][k];
        }
    }
    return v;
}

void solution() {
    std::cin >> n;
    graph.assign(n, std::vector<int>());
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        std::cin >> u >> v;
        u--, v--;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }
    std::cin >> m;
    depth.assign(n, -1);
    up.assign(n, std::vector(MAXLOG, -1));

    dfs(0, -1);
    build();

    int v, d;
    while (m--) {
        std::cin >> v >> d;
        v--;
        std::cout << la(v, d) + 1 << '\n';
    }
}

```
# LCA

```c++
constexpr int MAXLOG = 20;

int n;
std::vector<std::vector<int>> graph;
int m;
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

int lca(int u, int v) {
    if (depth[u] < depth[v]) {
        std::swap(u, v);
    }

    int diff = depth[u] - depth[v];
    for (int k = 0; k < MAXLOG; k++) {
        if (diff >> k & 1) {
            u = up[u][k];
            diff -= 1 << k;
        }
    }

    if (u == v) {
        return u;
    }

    for (int k = MAXLOG - 1; k >= 0; k--) {
        if (up[u][k] != up[v][k]) {
            u = up[u][k];
            v = up[v][k];
        }
    }

    return up[u][0];
}

void solution() {
    std::cin >> n;
    graph.assign(n, std::vector<int>());
    for (int i = 1; i < n; i++) {
        int p;
        std::cin >> p;
        p--;
        graph[p].push_back(i);
        graph[i].push_back(p);
    }
    depth.assign(n, -1);
    up.assign(n, std::vector(MAXLOG, -1));

    dfs(0, -1);
    build();

    std::cin >> m;
    while (m--) {
        int u, v;
        std::cin >> u >> v;
        u--, v--;
        std::cout << lca(u, v) + 1 << '\n';
    }
}
```

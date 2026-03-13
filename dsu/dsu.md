```c++
std::vector<int> parents, sizes;

for (int i = 0; i < n; i++) {
    parents[i] = i;
    sizes[i] = 1;
}

int get_c(const int v) {
    if (v == parents[v]) {
        return v;
    }
    return parents[v] = get_c(parents[v]);
}

void merge_c(int u, int v) {
    u = get_c(u), v = get_c(v);
    if (u == v) {
        return;
    }
    if (sizes[u] < sizes[v]) {
        std::swap(u, v);
    }
    parents[v] = u;
    sizes[u] += sizes[v];
}
```
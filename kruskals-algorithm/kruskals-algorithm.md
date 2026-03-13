```c++
void solution() {
    int n, m;
    std::cin >> n >> m;
    std::vector edges(m, std::vector(3, -1));
    for (auto& edge : edges) {
        std::cin >> edge[1] >> edge[2] >> edge[0];
        edge[1]--, edge[2]--;
    }

    std::ranges::sort(edges);
    long long weight = 0;
    for (auto edge : edges) {
        if (get_c(edge[1]) != get_c(edge[2])) { // get_c реализуется с помощью СНМ
            merge_c(edge[1], edge[2]); // merge_c реализуется с помощью СНМ
            weight += edge[0];
        }
    }

    std::cout << weight;
}
```
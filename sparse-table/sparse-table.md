```c++
int n, m; // количество элементов и количество запросов соответственно
std::cin >> n >> m;
std::vector<int> a(n); // массив чисел

// подсчет массива логов
std::vector log(n + 1, -1);
for (int i = 1; i < n + 1; i++) {
    log[i] = log[i / 2] + 1;
}

// инициализация и заполнение разреженной таблицы
std::vector st(n, std::vector(log[n] + 1, INT_MAX));
for (int i = 0; i < n; i++) {
    st[i][0] = a[i];
}
for (int k = 1; k <= log[n]; k++) {
    for (int i = 0; i + (1 << k) <= n; i++) {
        st[i][k] = std::min(st[i][k - 1], st[i + (1 << (k - 1))][k - 1]);
    }
}

// ответ для отрезка [l, r]
int l, r;
int k = log[r - l + 1];
int ans = std::min(st[l][k], st[r - (1 << k) + 1][k]);
```
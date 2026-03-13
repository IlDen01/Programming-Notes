```c++
struct fenwick {
    std::vector<long long> els;
    std::vector<long long> t;

    static long long f(const long long i) {
        return i & i + 1;
    }

    static long long ref(const long long i) {
        return i | i + 1;
    }

    explicit fenwick(const std::vector<long long>& a) : els(a),  t(a.size(), 0) {
        for (long long i = 0; i < els.size(); i++) {
            for (long long j = f(i); j <= i; j++) {
                t[i] += els[j];
            }
        }
    }

    explicit fenwick(const long long size) : els(size, 0), t(size, 0) {}

    // в этой вариации происходит замена элемента, а не прибавление или вычетание
    void update(long long ind, const long long new_val) {
        const long long delta = new_val - els[ind];
        els[ind] = new_val;
        while (ind < t.size()) {
            t[ind] += delta;
            ind = ref(ind);
        }
    }

    [[nodiscard]] long long pref_sum(long long ind) const {
        long long res = 0;
        while (ind >= 0) {
            res += t[ind];
            ind = f(ind) - 1;
        }
        return res;
    }

    [[nodiscard]] long long segment_sum(const long long l, const long long r) const {
        return pref_sum(r) - pref_sum(l - 1);
    }
};
```
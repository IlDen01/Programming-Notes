# Fenwick tree (дерево Фенвика, дерево бинарных отрезков)

## Теория

### Задача

Нужно уметь быстро над массивом:

* находить сумму на префиксе `[0, i]`
* находить сумму на отрезке `[l, r]`
* изменять значение элемента

Обычный массив префиксных сумм отвечает на запрос суммы за `O(1)`, но пересчёт после изменения элемента стоит `O(n)`. Дерево Фенвика делает оба вида запросов за `O(log n)`.

---

## Идея

Хранится массив `t`, где `t[i]` — сумма элементов исходного массива на отрезке `[f(i), i]`.

Границы этого отрезка задаются двумя функциями над битовым представлением индекса:

```c++
static long long f(const long long i) {
    return i & i + 1;
}

static long long ref(const long long i) {
    return i | i + 1;
}
```

* `f(i)` — обнуляет младшие единичные биты `i` → начало отрезка, за который отвечает `t[i]`
* `ref(i)` — заполняет младшие нулевые биты единицами → ближайший индекс, в сумму которого входит `t[i]`

За счёт этого:

* **подъём** по индексам через `ref` идёт по возрастанию, пока не выйдем за границу массива
* **спуск** по индексам через `f(i) - 1` идёт по убыванию, пока не станет меньше нуля

---

## Инициализация

```c++
std::vector<long long> els;
std::vector<long long> t;

explicit fenwick(const std::vector<long long>& a) : els(a), t(a.size(), 0) {
    for (long long i = 0; i < els.size(); i++) {
        for (long long j = f(i); j <= i; j++) {
            t[i] += els[j];
        }
    }
}

explicit fenwick(const long long size) : els(size, 0), t(size, 0) {}
```

### Построение дерева

* первый конструктор строит дерево из готового массива `els`
* для каждого `i` явно суммирует элементы отрезка `[f(i), i]`
* длина этого отрезка равна младшему единичному биту числа `i + 1`, поэтому суммарно по всем `i` работа получается `O(n log n)`, а не `O(n^2)`
* второй конструктор просто создаёт пустое дерево нужного размера

---

## Обновление элемента

```c++
void update(long long ind, const long long new_val) {
    const long long delta = new_val - els[ind];
    els[ind] = new_val;
    while (ind < t.size()) {
        t[ind] += delta;
        ind = ref(ind);
    }
}
```

### Подъём по дереву

* здесь реализация заменяет значение элемента, а не добавляет к нему
* считаем разницу `delta` между новым и старым значением
* поднимаемся вверх по индексам через `ref(ind)`, прибавляя `delta` во все `t[i]`, которые покрывают изменённый элемент
* останавливаемся, когда индекс выходит за пределы массива

---

## Нахождение префиксной суммы

```c++
[[nodiscard]] long long pref_sum(long long ind) const {
    long long res = 0;
    while (ind >= 0) {
        res += t[ind];
        ind = f(ind) - 1;
    }
    return res;
}
```

### Спуск по дереву

* берём сумму текущего блока `t[ind]`
* переходим к индексу `f(ind) - 1` — это последний индекс перед началом текущего блока
* повторяем, пока индекс не станет отрицательным
* так покрываем весь префикс `[0, ind]` непересекающимися блоками

---

## Сумма на отрезке

```c++
[[nodiscard]] long long segment_sum(const long long l, const long long r) const {
    return pref_sum(r) - pref_sum(l - 1);
}
```

* сумма на `[l, r]` — это разность двух префиксных сумм

---

## Асимптотика

* `update` и `pref_sum` — `O(log n)`, т.к. на каждом шаге меняется хотя бы один бит индекса, а битов у числа `O(log n)`
* построение дерева из массива — `O(n log n)`

```
O(log n) на запрос / обновление
O(n log n) на построение
```

---

## Применения

* суммы на отрезке с точечными обновлениями
* подсчёт инверсий в массиве
* k-я порядковая статистика (BIT как дерево частот)
* offline-обработка запросов
* двумерное обобщение (Fenwick tree 2D) для сумм на прямоугольнике

---

## Готовая реализация

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

    explicit fenwick(const std::vector<long long>& a) : els(a), t(a.size(), 0) {
        for (long long i = 0; i < els.size(); i++) {
            for (long long j = f(i); j <= i; j++) {
                t[i] += els[j];
            }
        }
    }

    explicit fenwick(const long long size) : els(size, 0), t(size, 0) {}

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

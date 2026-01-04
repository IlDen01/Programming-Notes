# Дерево Отрезков.

## База.

На полуинтервалах, номеруем с 0.

```c++
struct Node {
    int l, r;
    long long value;
};

vector<Node> tree;
vector<int> a;
```

## Код.

НУО код для поиска максимума на отрезке, реализации для других задач пишутся аналогично.

### Функция `update`

```c++
void update(const int v) {
    tree[v].value = max(tree[2 * v + 1].value, tree[2 * v + 2].value);
}
```

### Функция `build`

```c++
void build(const int v, const int l, const int r) {
    tree[v].l = l;
    tree[v].r = r;
    tree[v].lazy = 0;

    if (r - l == 1) {
        tree[v].value = a[l];
        return;
    }

    build(v * 2 + 1, l, (l + r) / 2);
    build(v * 2 + 2, (l + r) / 2, r);
    update(v);
}
```

### Функция `get`

Три случая:

 - $l \leqslant tree[v].l$ && $tree[v].r \leqslant r$ $\rightarrow tree[v].value$
 - $tree[v].r \leqslant l$ || $r \leqslant tree[v].l$ $\rightarrow -INF$
 - Иначе $\rightarrow max(get(2v + 1, l, r), get(2v + 2, l, r))$

```c++
long long get_max_value(const int v, const int l, const int r) {
    if (l <= tree[v].l && tree[v].r <= r) {
        return tree[v].value;
    }

    if (tree[v].r <= l || r <= tree[v].l) {
        return LLONG_MIN;
    }

    return max(get_max_value(v * 2 + 1, l, r), get_max_value(v * 2 + 2, l, r));
}
```

### Функция `add`

Для точечной операции, для массовой будет дальше.

```c++
void add(const int v, const int i, const int k) {
    if (tree[v].r - tree[v].l == 1) {
        tree[v].value = k;
        return;
    }
    
    if (i < (tree[v].l + tree[v].r) / 2) {
        add(2 * v + 1, i, k);
    } else {
        add(2 * v + 2, i, k);
    }

    update(v);
}
```

## Массовые операции.

Ниже представлена ленивая реализация.

### Структура `Node`

```c++
struct Node {
    // ...
    
    long long lazy;
};
```

### `push`

```c++
void push(const int v) {
    if (tree[v].r - tree[v].l != 1 && tree[v].lazy != 0) {
        tree[2 * v + 1].lazy += tree[v].lazy;
        tree[2 * v + 2].lazy += tree[v].lazy;
        tree[2 * v + 1].value += tree[v].lazy;
        tree[2 * v + 2].value += tree[v].lazy;
        tree[v].lazy = 0;
    }
}
```

### `build`

```c++
void build(const int v, const int l, const int r) {
    tree[v].l = l;
    tree[v].r = r;
    tree[v].lazy = 0;

    if (r - l == 1) {
        tree[v].value = a[l];
        return;
    }

    build(v * 2 + 1, l, (l + r) / 2);
    build(v * 2 + 2, (l + r) / 2, r);
    update(v);
}
```

### `get`

```c++
long long get_max_value(const int v, const int l, const int r) {
    if (l <= tree[v].l && tree[v].r <= r) {
        return tree[v].value;
    }

    if (tree[v].r <= l || r <= tree[v].l) {
        return LLONG_MIN;
    }

    push(v);
    return max(get_max_value(v * 2 + 1, l, r), get_max_value(v * 2 + 2, l, r));
}
```

### `add`

```c++
void add(const int v, const int l, const int r, const int k) {
    if (l <= tree[v].l && tree[v].r <= r) {
        tree[v].value += k;
        tree[v].lazy += k;
        return;
    }

    if (tree[v].r <= l || r <= tree[v].l) {
        return;
    }

    push(v);
    add(2 * v + 1, l, r, k);
    add(2 * v + 2, l, r, k);
    update(v);
}
```

# Декартово Дерево (Cartesian Tree)

## Теория

Двоичная куча на максимуме (по y) + двоичное дерево поиска (по x).

Нужны `merge` и `split`. И базовые `insert`, `erase`, `contains`.

### `split`

Разделяет дерево на два поддерева, одно меньше x, второе больше x.

Три случая:
1. В корне x. Тогда просто возвращаем дерево.
2. В корне y < x. Рекурсивно делим правое дерево y на A и B. Тогда левое дерево — корень y и левое поддерево y + A,
правое поддерево — B.
3. В корне y > x. Аналогично.

### `merge`

Объединяет два дерева в одно.

A < B. $root_{A_y} > root_{B_y} \Rightarrow$ к $root_A$ подвешиваем $L_A$ слева и все остально справо (рекусрсивно).
$root_{A_y} < root_{B_y}$ аналогично.

### `contains`

Спуск по дереву.

### `insert`

`split` по x, затем `merge` x и меньше x, а потом `merge` этого и больше x.

### `erase`

`split` по x, строгий и не строгий, затем `merge`.

## Код

```c++
std::mt19937 rnd(std::random_device{}());

struct node {
    int key, prior;
    node* left;
    node* right;

    node(const int key) : key(key), prior(static_cast<int>(rnd())), size(1), left(nullptr), right(nullptr) {}
};

node* merge(node* root1, node* root2) {
    if (root1 == nullptr) {
        return root2;
    }
    if (root2 == nullptr) {
        return root1;
    }

    if (root1->prior > root2->prior) {
        root1->right = merge(root1->right, root2);
        return root1;
    }
    root2->left = merge(root1, root2->left);
    return root2;
}

std::pair<node*, node*> split(node* root, const int x) {
    if (root == nullptr) {
        return {nullptr, nullptr};
    }

    if (root->key < x) {
        auto [root1, root2] = split(root->right, x);
        root->right = root1;
        return {root, root2};
    }
    auto [root1, root2] = split(root->left, x);
    root->left = root2;
    return {root1, root};
}

bool contains(node* root, int x);

bool insert(node*& root, int x) {
    if (contains(root, x)) {
        return false;
    }

    auto [root1, root2] = split(root, x);
    node* rootx = new node(x);
    root = merge(merge(root1, rootx), root2);
    return true;
}

void erase(node* root, int x);
```

# Декартово Дерево по неявному ключу

```c++
std::mt19937 rnd(std::random_device{}());

struct node {
    int key, prior;
    int size;
    node* left;
    node* right;

    node(const int key) : key(key), prior(static_cast<int>(rnd())), size(1), left(nullptr), right(nullptr) {}
};

int get_size(node* root) {
    return root == nullptr ? 0 : root->size;
}

void update_size(node* root) {
    if (root == nullptr) {
        return;
    }

    root->size = 1 + get_size(root->left) + get_size(root->right);
}

node* merge(node* root1, node* root2) {
    if (root1 == nullptr) {
        return root2;
    }
    if (root2 == nullptr) {
        return root1;
    }

    if (root1->prior > root2->prior) {
        root1->right = merge(root1->right, root2);
        update_size(root1);
        return root1;
    }
    root2->left = merge(root1, root2->left);
    return root2;
}

std::pair<node*, node*> split_k(node* root, const int k) {
    if (root == nullptr) {
        return {nullptr, nullptr};
    }

    if (k == 0) {
        return {nullptr, root};
    }

    if (get_size(root->left) < k) {
        auto [root1, root2] = split_k(root->right, k - get_size(root->left) - 1);
        root->right = root1;
        update_size(root);
        return {root, root2};
    } else {
        auto [root1, root2] = split_k(root->left, k);
        root->left = root2;
        update_size(root);
        return {root1, root};
    }
}

bool contains(node* root, int x);

bool insert(node*& root, int x) {
    if (contains(root, x)) {
        return false;
    }

    auto [root1, root2] = split(root, x);
    node* rootx = new node(x);
    root = merge(merge(root1, rootx), root2);
    return true;
}

void erase(node* root, int x);
```

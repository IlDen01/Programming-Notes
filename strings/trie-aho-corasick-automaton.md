# Бор и Ахо-Корасик (Trie & Aho-Corasick automaton)

## Теория

**Бор** — дерево, в котором хранится множество строк так, что у строк с общим префиксом общий путь от корня.

Что нужно уметь делать (базовый бор):

* добавить строку в структуру
* проверить, есть ли строка в множестве
* проверить, есть ли строка как префикс какого-то слова из множества

Если строк много и нужно искать вхождения сразу всех строк множества в тексте — на боре достраивается **автомат Ахо-Корасик**: он позволяет за один проход по тексту найти все вхождения всех строк множества.

---

## Идея

* каждая вершина хранит массив детей `child[ALPHA_SIZE]` — по одному на каждый символ алфавита
* путь от корня до вершины — префикс какой-то строки из множества
* `is_term` отмечает, что в этой вершине заканчивается слово
* для автомата Ахо-Корасик в вершине заранее заложены:
    * `parent` — родитель в дереве
    * `suff` — **суффиксная ссылка**
    * `term` — **терминальная (exit) ссылка**

```c++
constexpr int ALPHA_SIZE = 26;
char START_CHILD = 'a';

struct node {
    char symbol;
    node* child[ALPHA_SIZE];
    bool is_term;

    node* parent;
    node* suff;
    node* term;

    node() : symbol(-1), is_term(false) {
        parent = suff = term = nullptr;
        for (auto& i : child) {
            i = nullptr;
        }
    }
};
```

---

## Инициализация

```c++
node* trie_init() {
    node* root = new node();
    root->suff = root;
    return root;
}
```

### Объяснение

* создаём корень бора
* `root->suff = root` — суффиксная ссылка корня замкнута сама на себя

Это база рекурсии для построения суффиксных ссылок: у корня нет родителя, и без этой самоссылки построение суффиксных ссылок пришлось бы обрабатывать отдельным случаем.

---

## Добавление строки

```c++
void trie_add(node* root, const std::string& s) {
    node* cur = root;
    for (auto c : s) {
        if (!cur->child[c - START_CHILD]) {
            node* new_node = new node();
            new_node->symbol = c;
            new_node->parent = cur;
            cur->child[c - START_CHILD] = new_node;
        }
        cur = cur->child[c - START_CHILD];
    }
    cur->is_term = true;
}
```

### Объяснение

* идём по строке слева направо
* на каждом шаге спускаемся в дочернюю вершину, соответствующую символу
* если такой вершины нет — создаём, запоминаем родителя (нужен для построения суффиксных ссылок)
* дошли до конца строки — отмечаем вершину как терминальную

---

## Поиск строки (базовый бор)

```c++
bool trie_search(node* root, const std::string& s, bool exact = true) {
    node* cur = root;
    for (auto c : s) {
        if (!cur->child[c - START_CHILD]) {
            return false;
        }
        cur = cur->child[c - START_CHILD];
    }
    return exact ? cur->is_term : true;
}
```

### Объяснение

* спускаемся по бору по символам строки
* если на каком-то шаге нужного ребёнка нет — строки (или префикса) нет в множестве
* дошли до конца пути:
    * если нужен **точный поиск** — проверяем `is_term`
    * если нужен поиск **по префиксу** — достаточно того, что путь дошёл до конца

---

## Дополнительная задача: максимальный XOR

Есть множество чисел. Вводится `x`. Среди множества чисел надо найти такое `y`, что `x ⊕ y` максимален.

Строим бор по битовым представлениям чисел (`ALPHA_SIZE = 2`, алфавит — биты `0` и `1`). Для запроса жадно проходим по дереву и на каждом шаге проверяем, можно ли взять противоположный текущему биту символ. Если можем — берём, иначе берём то, что есть.

### Добавление числа

```c++
const int BITS = 30;

void insert_bits(node* root, int x) {
    node* cur = root;
    for (int i = BITS - 1; i >= 0; i--) {
        int bit = (x >> i) & 1;
        if (!cur->child[bit]) {
            cur->child[bit] = new node();
        }
        cur = cur->child[bit];
    }
    cur->is_term = true;
}
```

#### Объяснение

* идём по числу `x` от старшего бита к младшему
* на каждом шаге спускаемся в дочернюю вершину, соответствующую биту
* если такой вершины нет — создаём
* дошли до конца — отмечаем вершину как терминальную

---

### Запрос максимального XOR

```c++
int query_max_xor(node* root, int x) {
    node* cur = root;
    int result = 0;
    for (int i = BITS - 1; i >= 0; i--) {
        int bit = (x >> i) & 1;
        int want = bit ^ 1;
        if (cur->child[want]) {
            result |= (1 << i);
            cur = cur->child[want];
        } else {
            cur = cur->child[bit];
        }
    }
    return result;
}
```

#### Объяснение

* идём по битам `x` от старшего к младшему
* на каждом шаге хотим выбрать **противоположный** бит — тогда в этом разряде XOR даст `1`
* если противоположная ветка существует — идём в неё, добавляем бит в ответ
* если нет — идём в единственную существующую ветку (с тем же битом), в этом разряде XOR даст `0`

---

## Ахо-Корасик: идея

Автомат Ахо-Корасик — это бор, дополненный **суффиксными ссылками** (обобщение префикс-функции из КМП на множество строк).

* **суффиксная ссылка** вершины `v` — вершина бора, соответствующая наибольшему собственному суффиксу строки `v`, который сам является префиксом какой-то строки из множества (т.е. существует как вершина бора)
* **терминальная (exit) ссылка** вершины `v` — ближайшая по цепочке суффиксных ссылок вершина, которая является терминальной

Терминальная ссылка нужна, чтобы за `O(1)` понять, заканчивается ли в текущей позиции текста хоть одна строка из множества, не проходя всю цепочку суффиксных ссылок каждый раз заново.

Суффиксные ссылки достраиваются **лениво**, с мемоизацией: каждая вершина считает свою ссылку один раз и запоминает её.

---

## Переход по автомату

Функция `move(cur, c)` — функция переходов автомата: из вершины `cur` по символу `c` всегда есть куда перейти (в отличие от обычного бора).

```c++
node* move(node* cur, char c) {
    if (cur->child[c - START_CHILD]) {
        return cur->child[c - START_CHILD];
    }

    if (!cur->parent) {
        return cur;
    }
    return move(get_suff_link(cur), c);
}
```

### Объяснение

* если по символу `c` есть прямое ребро в боре — идём по нему
* если ребра нет и `cur` — корень (у корня нет родителя) — остаёмся в корне (**самопереход**)
* иначе — переходим по суффиксной ссылке `cur` и пробуем сделать переход по `c` уже оттуда (рекурсивно)

---

## Суффиксная и терминальная ссылки

```c++
node* get_suff_link(node* cur) {
    if (cur->suff) {
        return cur->suff;
    }

    if (!cur->parent->parent) {
        cur->suff = cur->parent;
    } else {
        cur->suff = move(get_suff_link(cur->parent), cur->symbol);
    }

    if (cur->suff->is_term) {
        cur->term = cur->suff;
    } else {
        get_suff_link(cur->suff);
        cur->term = cur->suff->term;
    }
    return cur->suff;
}
```

### Объяснение

* если суффиксная ссылка уже посчитана — сразу возвращаем её (**мемоизация**)
* если родитель `cur` — корень (`cur->parent->parent == nullptr`), то `cur` — прямой потомок корня, его суффиксная ссылка — сам корень
* иначе: берём суффиксную ссылку родителя, из неё пробуем перейти по тому же символу, каким мы попали в `cur` — это и есть суффиксная ссылка `cur` (та же рекуррентность, что и в префикс-функции КМП)
* терминальная ссылка:
    * если суффиксная ссылка сама терминальна — она и есть терминальная ссылка
    * иначе — берём терминальную ссылку суффиксной ссылки (достраивая её при необходимости), продолжая цепочку

---

## Поиск вхождения подстроки

```c++
bool trie_find(node* root, const std::string& s) {
    node* cur = root;
    for (const auto c : s) {
        cur = move(cur, c);
        get_suff_link(cur);
        if (cur->is_term || cur->term) {
            return true;
        }
    }
    return false;
}
```

### Объяснение

* прогоняем текст `s` посимвольно через автомат (`move`)
* на каждом шаге досчитываем суффиксную/терминальную ссылку текущей вершины
* если текущая вершина терминальна, либо у неё есть терминальная ссылка — значит, в этой позиции заканчивается хотя бы одна строка из множества

---

## Асимптотика

* добавление строки: `O(|s|)`
* построение суффиксной/терминальной ссылки одной вершины — амортизированно `O(1)` за счёт мемоизации (каждая ссылка считается ровно один раз)
* построение всего автомата (все ссылки по всем вершинам): `O(N * ALPHA_SIZE)`, где `N` — суммарное число вершин бора
* один переход `move` — амортизированно `O(1)`
* поиск вхождений в тексте `t`: `O(|t|)`
* память: `O(N * ALPHA_SIZE)`

---

## Применения

* хранение и поиск строк по префиксу, автодополнение
* максимальный XOR пары чисел / подмассива (бор по битам)
* множественный поиск подстрок (автомат Ахо-Корасик) — поиск всех вхождений всех строк множества в тексте за один проход
* Ахо-Корасик + динамическое программирование (например, подсчёт числа строк длины `n`, не содержащих запрещённых подстрок)
* потоковая фильтрация текста по словарю (сигнатурный поиск)

---

## Готовая реализация

```c++
constexpr int ALPHA_SIZE = 26;
char START_CHILD = 'a';

struct node {
    char symbol;
    node* child[ALPHA_SIZE];
    bool is_term;

    node* parent;
    node* suff;
    node* term;

    node() : symbol(-1), is_term(false) {
        parent = suff = term = nullptr;
        for (auto& i : child) {
            i = nullptr;
        }
    }
};

node* trie_init() {
    node* root = new node();
    root->suff = root;
    return root;
}

void trie_add(node* root, const std::string& s) {
    node* cur = root;
    for (auto c : s) {
        if (!cur->child[c - START_CHILD]) {
            node* new_node = new node();
            new_node->symbol = c;
            new_node->parent = cur;
            cur->child[c - START_CHILD] = new_node;
        }
        cur = cur->child[c - START_CHILD];
    }
    cur->is_term = true;
}

bool trie_search(node* root, const std::string& s, bool exact = true) {
    node* cur = root;
    for (auto c : s) {
        if (!cur->child[c - START_CHILD]) {
            return false;
        }
        cur = cur->child[c - START_CHILD];
    }
    return exact ? cur->is_term : true;
}

const int BITS = 30;

void insert_bits(node* root, int x) {
    node* cur = root;
    for (int i = BITS - 1; i >= 0; i--) {
        int bit = (x >> i) & 1;
        if (!cur->child[bit]) {
            cur->child[bit] = new node();
        }
        cur = cur->child[bit];
    }
    cur->is_term = true;
}

int query_max_xor(node* root, int x) {
    node* cur = root;
    int result = 0;
    for (int i = BITS - 1; i >= 0; i--) {
        int bit = (x >> i) & 1;
        int want = bit ^ 1;
        if (cur->child[want]) {
            result |= (1 << i);
            cur = cur->child[want];
        } else {
            cur = cur->child[bit];
        }
    }
    return result;
}

node* move(node* cur, char c);

node* get_suff_link(node* cur) {
    if (cur->suff) {
        return cur->suff;
    }

    if (!cur->parent->parent) {
        cur->suff = cur->parent;
    } else {
        cur->suff = move(get_suff_link(cur->parent), cur->symbol);
    }
    
    get_suff_link(cur->suff);

    if (cur->suff->is_term) {
        cur->term = cur->suff;
    } else {
        get_suff_link(cur->suff);
        cur->term = cur->suff->term;
    }
    return cur->suff;
}

node* move(node* cur, char c) {
    if (cur->child[c - START_CHILD]) {
        return cur->child[c - START_CHILD];
    }

    if (!cur->parent) {
        return cur;
    }
    return move(get_suff_link(cur), c);
}

bool trie_find(node* root, const std::string& s) {
    node* cur = root;
    for (const auto c : s) {
        cur = move(cur, c);
        get_suff_link(cur);
        if (cur->is_term || cur->term) {
            return true;
        }
    }
    return false;
}
```

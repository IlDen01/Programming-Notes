# Корневая оптимизация (Root Optimization)

## Теория

### Задача

1. ? $\sum$ на $[l, r]$
2. $a_i = val$

### Решение

Разобьем массив на блоки длинной $\approx \sqrt{n}$.

Номер блока i-го элемента — $\dfrac{i}{BlockSize}$

Позиция i-го элемента внутри блока — $i \mod{BlockSize}$

$l_b = \left\lceil \dfrac{l}{BlockSize} \right\rceil$ — первый полностью покрытый блок.

$r_b = \left\lfloor \dfrac{r}{BlockSize} \right\rfloor$ — первый не полносью покрытый блок.

## Код


```c++
constexpr int BLOCK_SIZE = 300;

void solution() {
    int n;
    std::cin >> n;
    std::vector<int> a;
    std::vector<int> block;
    int block_quantity = (n + BLOCK_SIZE - 1) / BLOCK_SIZE;
    int n1 = block_quantity * BLOCK_SIZE;
    int l, r;
    int lb = (l + BLOCK_SIZE - 1) / BLOCK_SIZE;
    int rb = r / BLOCK_SIZE;
    int ans = 0;
    /*if (lb < rb) {
        for (int i = lb; i < rb; i++) {
            ans += block[i];
        }
        for (int i = l; i < lb * BLOCK_SIZE; i++) {
            ans += a[i];
        }
        for (int i = rb * BLOCK_SIZE; i < r; i++) {
            ans += a[i];
        }
        for (int i = l; i < r; i++) {
            ans += a[i];
        }
    } else {
        for (int i = l; i < r; i++) {
            ans += a[i];
        }
    }*/
    for (int i = l; i < r;) {
        if (i % BLOCK_SIZE == 0 && i <= r) {
            ans += block[i / BLOCK_SIZE];
            i += BLOCK_SIZE;
        } else {
            ans += a[i];
            i++;
        }
    }
}
```

# Геометрия.

## База.

### Скалярное (псевдо-скалярное) произведение векторов.

$\vec{a} \cdot \vec{b} = |\vec{a}| \cdot |\vec{b}| \cdot \cos \alpha = a.x \cdot b.x + a.y \cdot b.y$

В коде обозначается `*`.

### Векторное произведение векторов.

$\vec{a} \times \vec{b} = |\vec{a}| \cdot |\vec{b}| \cdot \sin \alpha = a.x \cdot b.y - a.y \cdot b.x$

В коде обозначается `^`.

## Класс точки.

Реализация класса точки находится в [файле](point.cpp).

## Принадлежность точки промежутку.

### Принадлежность точки прямой.

Точка $P$ принадлежит прямой $AB$, если $\vec{AB}$ коллинеарен $\vec{AP}$.

Проверка:

```c++
if (sign(AB ^ AP) == 0) {
    return true;
} else {
    return false;
}
```

### Принадлежность точки лучу.

Точка $P$ принадлежит лучу $AB$, если принадлежит прямой и $\vec{AB}$ сонаправлен с $\vec{AP}$.

Проверка:

```c++
if (sign(AB ^ AP) == 0 && sign(AP * AB) >= 0) {
    return true;
} else {
    return false;
}
```

### Принадлежность точки отрезку.

Точка $P$ принадлежит отрезку $AB$, если она принадлежит лучу и $\vec{BA}$ сонаправлен с $\vec{BP}$.

Проверка:

```c++
if (sign(AB ^ AP) == 0 && sign(AP * AB) >= 0 && sign(BA * BP) >= 0) {
    return true;
} else {
    return false;
}
```

Также, если точка уже на прямой, можно написать bounding box.

## Расстояния.

### Расстояние от точки до прямой.

Вычисляется из площади треугольника, образованного точками $A$ и $B$, которые задают прямую, и точкой $P$.

$S_{\triangle ABP} = \frac{1}{2} |AB| \cdot |PH|$, также $S_{\triangle ABP} = \frac{1}{2} (\vec{AB} \times \vec{AP})$.

Тогда $|PH| = \frac{|\vec{AB} \times \vec{AP}|}{|AB|}$.

```c++
long double dist = abs(AB ^ AP) / AB.len();
```

### Расстояние от точки до луча (отрезка).

Для вычисления расстояния от точки до луча (отрезка) надо проверить, что высота из $P$ к прямой $AB$ падает также и на
луч (отрезок).

Проверка (НУО начало луча (отрезка) в точке $A$):

```c++
if (AB * AP < 0) {
    return |AP|;
} else {
    return |PH|;
}
```

Для проверки отрезка нужно аналогично проверить другой конец.

## Пересечение отрезков.

Чтобы отрезки пересекались $sign(\vec{AB} \times \vec{AD}) \cdot sign(\vec{AB} \times \vec{AC}) \leqslant 0$,
$sign(\vec{CD} \times \vec{CA}) \cdot sign(\vec{CD} \times \vec{CD}) \leqslant 0$, $max(L_x) \leqslant min(R_x)$
и $max(L_y) \leqslant min(R_y)$.

```c++
if (sign(AB ^ AD) * sign(AB ^ AC) <= 0 && sign(CD ^ CA) * sign(CD ^ CB) <= 0 &&
    max(L_x) <= min(R_x) && max(L_y) <= min(R_y)) {
    return true;
} else {
    return false;
}
```

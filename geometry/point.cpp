constexpr long double EPS = 1e-9;

struct Point {
    long double x, y;

    Point() = default;

    Point(const long double x, const long double y) : x(x), y(y) {
    }

    Point(const Point &p_1, const Point &p_2) {
        x = p_2.x - p_1.x;
        y = p_2.y - p_1.y;
    }

    [[nodiscard]] long double len2() const { return x * x + y * y; }

    [[nodiscard]] long double len() const { return sqrtl(len2()); }

    void operator+=(const Point &other) {
        x += other.x;
        y += other.y;
    }

    Point operator+(const Point &other) const {
        Point res = *this;
        res += other;
        return res;
    }

    void operator-=(const Point &other) {
        x -= other.x;
        y -= other.y;
    }

    Point operator-(const Point &other) const {
        Point res = *this;
        res -= other;
        return res;
    }

    void operator*=(const long double k) {
        x *= k;
        y *= k;
    }

    Point operator*(const long double k) const {
        Point res = *this;
        res *= k;
        return res;
    }

    long double operator*(const Point &other) const {
        return x * other.x + y * other.y;
    }

    long double operator^(const Point &other) const {
        return x * other.y - y * other.x;
    }

    [[nodiscard]] long double angle() const {
        return atan2(y, x) < 0 ? atan2(y, x) + 2 * M_PI : atan2(y, x);
    }

    bool operator==(const Point &other) const {
        return abs(x - other.x) < EPS && abs(y - other.y) < EPS;
    }

    bool operator!=(const Point &other) const {
        return !(*this == other);
    }
};

long double angle(const Point &a, const Point &b) {
    return atan2(a ^ b, a * b) < 0
               ? atan2(a ^ b, a * b) + 2 * M_PI
               : atan2(a ^ b, a * b);
}

int sign(const long double x) {
    if (x > EPS) {
        return 1;
    }
    if (x < -EPS) {
        return -1;
    }
    return 0;
}

istream &operator>>(istream &in, Point &p) {
    in >> p.x >> p.y;
    return in;
}

ostream &operator<<(ostream &out, const Point &p) {
    out << p.x << ' ' << p.y;
    return out;
}

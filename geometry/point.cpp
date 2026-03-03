constexpr long double EPS = 1e-9;
// constexpr long double M_PI = 3.14159265358979323846;

struct point {
    long double x, y;

    point() = default;

    point(const long double x, const long double y) : x(x), y(y) {}

    point(const point &p_1, const point &p_2)
        : x(p_2.x - p_1.x), y(p_2.y - p_1.y) {}

    [[nodiscard]] long double len2() const { return x * x + y * y; }

    [[nodiscard]] long double len() const { return sqrtl(len2()); }

    void operator+=(const point &other) {
        x += other.x;
        y += other.y;
    }

    point operator+(const point &other) const {
        point res = *this;
        res += other;
        return res;
    }

    void operator-=(const point &other) {
        x -= other.x;
        y -= other.y;
    }

    point operator-(const point &other) const {
        point res = *this;
        res -= other;
        return res;
    }

    void operator*=(const long double k) {
        x *= k;
        y *= k;
    }

    point operator*(const long double k) const {
        point res = *this;
        res *= k;
        return res;
    }

    long double operator*(const point &other) const {
        return x * other.x + y * other.y;
    }

    long double operator^(const point &other) const {
        return x * other.y - y * other.x;
    }

    [[nodiscard]] long double angle() const {
        return atan2(y, x) < 0 ? atan2(y, x) + 2 * M_PI : atan2(y, x);
    }

    bool operator==(const point &other) const {
        return std::abs(x - other.x) < EPS && std::abs(y - other.y) < EPS;
    }

    bool operator!=(const point &other) const { return !(*this == other); }
};

long double angle(const point &a, const point &b) {
    return atan2(a ^ b, a * b) < 0 ? atan2(a ^ b, a * b) + 2 * M_PI
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

std::istream &operator>>(std::istream &in, point &p) {
    in >> p.x >> p.y;
    return in;
}

std::ostream &operator<<(std::ostream &out, const point &p) {
    out << p.x << ' ' << p.y;
    return out;
}

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Fraction {
public:
    int num, den;
    Fraction() : num(0), den(1) {}
    Fraction(int n) : num(n), den(1) {}
    Fraction(int n, int d) : num(n), den(d) {
        int gcd = __gcd(num, den);
        this->num /= gcd;
        this->den /= gcd;
        if (this->den < 0) {
            this->den *= -1;
            this->num *= -1;
        }
    }
    Fraction(const Fraction &f) {
        this->num = f.num;
        this->den = f.den;
    }
    Fraction &operator=(const Fraction &f) {
        this->num = f.num;
        this->den = f.den;
        return *this;
    }
    Fraction operator+(const Fraction &f) {
        int num_ = num * f.den + den * f.num;
        int den_ = den * f.den;
        int gcd = __gcd(num_, den_);
        num_ /= gcd;
        den_ /= gcd;
        return Fraction(num_, den_);
    }
    Fraction operator+(const int n) {
        int num_ = num + n * den;
        int den_ = den;
        int gcd = __gcd(num_, den_);
        num_ /= gcd;
        den_ /= gcd;
        return Fraction(num_, den_);
    }
    Fraction operator-(const Fraction &f) {
        int num_ = num * f.den - den * f.num;
        int den_ = den * f.den;
        int gcd = __gcd(num_, den_);
        num_ /= gcd;
        den_ /= gcd;
        return Fraction(num_, den_);
    }
    Fraction operator-(const int n) {
        int num_ = num - n * den;
        int den_ = den;
        int gcd = __gcd(num_, den_);
        num_ /= gcd;
        den_ /= gcd;
        return Fraction(num_, den_);
    }
    Fraction operator*(const Fraction &f) {
        int num_ = num * f.num;
        int den_ = den * f.den;
        int gcd = __gcd(num_, den_);
        num_ /= gcd;
        den_ /= gcd;
        return Fraction(num_, den_);
    }
    Fraction operator*(const int n) {
        int num_ = num * n;
        int den_ = den;
        int gcd = __gcd(num_, den_);
        num_ /= gcd;
        den_ /= gcd;
        return Fraction(num_, den_);
    }
    Fraction operator/(const Fraction &f) {
        int num_ = num * f.num;
        int den_ = den * f.den;
        int gcd = __gcd(num_, den_);
        num_ /= gcd;
        den_ /= gcd;
        if (den_ < 0) {
            den_ *= -1;
            num_ *= -1;
        }
        return Fraction(num_, den_);
    }
    Fraction operator/(const int n) {
        int num_ = num;
        int den_ = den * n;
        int gcd = __gcd(num_, den_);
        num_ /= gcd;
        den_ /= gcd;
        return Fraction(num_, den_);
    }
    bool operator==(const Fraction &f) {
        return den == f.den && num == f.num;
    }
    bool operator==(const int n) {
        return den == 1 && num == n;
    }
};

ostream& operator<<(ostream &ost, Fraction &f) {
    if (f.num == 0)
        ost << 0;
    else if (f.den != 1 && f.num > 0)
        ost << '(' << f.num << '/' << f.den << ')';
    else if (f.den != 1 && f.num < 0)
        ost << "-(" << -f.num << '/' << f.den << ')';
    else
        ost << f.num;
    return ost;
}

Fraction nCr(int n, int r) {
    Fraction res(1);
    for (int i = 1; i <= r; i++, n--) {
        Fraction f(n, i);
        res = res * f;
    }
    return res;
}

int main() {
    vector<Fraction> vec;
    int p;
    while (true) {
        cout << "----------------------------------------------------------\n" << "Please input the value of p: ";
        cin >> p;
        vec.clear();
        for (int i = 1; i <= p + 1; i++) {
            Fraction f = nCr(p, i - 1);
            for (int j = 1; j < i; j++)
                f = f - (vec[j - 1] * nCr(p + 2 - j, i + 1 - j));
            f = f / (p + 2 - i);
            vec.push_back(f);
        }
        cout << "Sum of 1^" << p << "+2^" << p << "+...+n^" << p << " is" << endl;
        if (vec[0] == 1) cout << "n^" << p + 1;
        else cout << vec[0] << "n^" << p + 1;
        for (int i = 1; i < vec.size(); i++) {
            if (vec[i] == 0) continue;
            if (vec[i].num > 0) {
                if (vec[i] == 1) cout << "+n^" << p + 1 - i;
                else cout << '+' << vec[i] << "n^" << p + 1 - i;
            } else {
                if (vec[i] == -1) cout << "-n^" << p + 1 - i;
                else cout << vec[i] << "n^" << p + 1 - i;
            }
        }
        cout << endl << endl;
    }
}

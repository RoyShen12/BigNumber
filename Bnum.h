#pragma once
#include "stdafx.h"
using namespace std;

#define MAX(a,b) a>b?a:b
// DLEN个9
#define MAXN 99999999
// BigNum::a的每个数字代表大数的位数
#define DLEN 8
// 初始位数
#define A_LENGTH 10

struct sizing {
	size_t rate = 2;
	const size_t operator++(int) {
		size_t old = rate;
		rate = rate * 2;
		// printf("sizing to %zux\n", rate);
		return old;
	}
};

class BigNum {
public:
	basic_string<int64_t> a;
	int64_t len;
	sizing AMPFY_RT;
	BigNum() {
		// printf("new instance ()\n");
		len = 1;
		a.resize(A_LENGTH, 0);
	}
	BigNum(const int64_t);
	BigNum(const char*);
	BigNum(const BigNum &);
	BigNum &operator = (const BigNum &);

	friend istream& operator>>(istream&, BigNum&);
	friend ostream& operator<<(ostream&, BigNum&);

	BigNum operator+(const BigNum &)  const;
	BigNum& operator++();
	const BigNum operator++(int);
	BigNum operator-(const BigNum &)  const;
	BigNum operator*(const BigNum &)  const;
	BigNum operator/(const int64_t &) const;

	BigNum operator^(const BigNum &)    const;
	int64_t operator%(const int64_t &)  const;  
	bool   operator>(const BigNum & T)  const;
	bool   operator<(const BigNum & T)  const;
	bool   operator==(const BigNum & T) const;
	BigNum operator!() const;

	void print() const;
	void print(FILE *f) const;
};
inline BigNum::BigNum(const int64_t b)
{
	// printf("new instance (int64_t)\n");
	int64_t c, d = b;
	len = 0;
	a.resize(A_LENGTH, 0);
	while (d > MAXN) {
		c = d - (d / (MAXN + 1)) * (MAXN + 1);
		d = d / (MAXN + 1);
		a[len++] = c;
	}
	a[len++] = d;
}
inline BigNum::BigNum(const char*s)
{
	// printf("new instance (const char*s)\n");
	int64_t t, k, index, l, i;
	l = strlen(s);
	a.resize(MAX(l / DLEN + 4, A_LENGTH), 0);
	len = l / DLEN;
	if (l % DLEN)
		len++;
	index = 0;
	for (i = l - 1; i >= 0; i -= DLEN) {
		t = 0;
		k = i - DLEN + 1;
		if (k < 0)
			k = 0;
		for (int64_t j = k; j <= i; j++)
			t = t * 10 + s[j] - '0';
		a[index++] = t;
	}
}
inline BigNum::BigNum(const BigNum & T) : len(T.len)
{
	// printf("new instance (const BigNum & T)\n");
	/*int64_t i;
	a.resize(MAX(len, A_LENGTH), 0);
	for (i = 0; i < len; i++)
		a[i] = T.a[i];*/
	a = T.a;
}
inline BigNum &BigNum::operator=(const BigNum & n)
{
	// printf("new instance =\n");
	//int64_t i;
	len = n.len;
	a = n.a;
	/*a.resize(MAX(len, A_LENGTH), 0);
	for (i = 0; i < len; i++)
		a[i] = n.a[i];*/
	return *this;
}
inline istream& operator>>(istream & in, BigNum & b)
{
	string ch;
	int64_t i = -1;
	in >> ch;
	int64_t l = ch.size();
	b.a.resize(MAX(l / (DLEN - 2), A_LENGTH), 0);
	int64_t count = 0, sum = 0;
	for (i = l - 1; i >= 0;) {
		sum = 0;
		int64_t t = 1;
		for (int64_t j = 0; j < DLEN && i >= 0; j++, i--, t *= 10) {
			sum += (ch[i] - '0')*t;
		}
		b.a[count] = sum;
		count++;
	}
	b.len = count++;
	return in;

}
inline ostream& operator<<(ostream& out, BigNum& b)
{
	int64_t i;
	out << b.a[b.len - 1];
	for (i = b.len - 2; i >= 0; i--) {
		//printf("\nloop: %lld,value: %lld\n", i, b.a[i]);
		out.width(DLEN);
		out.fill('0');
		out << b.a[i];
	}
	return out;
}
inline BigNum BigNum::operator+(const BigNum & T) const
{
	BigNum t(*this);
	int64_t i, big;
	big = T.len > len ? T.len : len;
	if (big > len)
		t.a.resize(big + 1, 0);
	for (i = 0; i < big; i++) {
		t.a[i] += T.a[i];
		if (t.a[i] > MAXN) {
			t.a[i + 1]++;
			t.a[i] -= MAXN + 1;
		}
	}
	if (t.a[big] != 0)
		t.len = big + 1;
	else
		t.len = big;
	return move(t);
}
inline BigNum& BigNum::operator++()
{
	this->a[len - 1]++;
	// 进位
	if (this->a[len - 1] > MAXN) {
		this->a.resize(this->a.size() + 1, 0);
		this->a[len]++;
		this->a[len - 1] -= MAXN + 1;
	}
	return (*this);
}
inline const BigNum BigNum::operator++(int)
{
	BigNum old(*this);
	++(*this);
	return move(old);
}
inline BigNum BigNum::operator-(const BigNum & T) const
{
	int64_t i, j, big;
	bool flag;
	BigNum t1, t2;
	if (*this > T) {
		t1 = *this;
		t2 = T;
		flag = 0;
	}
	else {
		t1 = T;
		t2 = *this;
		flag = 1;
	}
	big = t1.len;
	// resize t2 to {big}
	t2.a.resize(MAX(t1.len, t2.len), 0);
	for (i = 0; i < big; i++) {
		if (t1.a[i] < t2.a[i]) {
			j = i + 1;
			while (t1.a[j] == 0)
				j++;
			t1.a[j--]--;
			while (j > i)
				t1.a[j--] += MAXN;
			t1.a[i] += MAXN + 1 - t2.a[i];
		}
		else
			t1.a[i] -= t2.a[i];
	}
	t1.len = big;
	while (t1.a[t1.len - 1] == 0 && t1.len > 1) {
		t1.len--;
		big--;
	}
	if (flag)
		t1.a[big - 1] = 0 - t1.a[big - 1];
	return move(t1);
}

inline BigNum BigNum::operator*(const BigNum & T) const
{
	BigNum ret;
	int64_t i, j, up;
	int64_t temp, temp1;
	for (i = 0; i < len; i++) {
		up = 0;
		for (j = 0; j < T.len; j++) {
			// dynamic 2x resizing
			while (ret.a.size() <= (i + j))
				ret.a.resize(ret.AMPFY_RT++ * ret.a.size(), 0);
			temp = a[i] * T.a[j] + ret.a[i + j] + up;
			if (temp > MAXN) {
				temp1 = temp - temp / (MAXN + 1) * (MAXN + 1);
				up = temp / (MAXN + 1);
				ret.a[i + j] = temp1;
			}
			else {
				up = 0;
				ret.a[i + j] = temp;
			}
		}
		if (up != 0) {
			// dynamic 2x resizing
			while (ret.a.size() <= (i + j))
				ret.a.resize(ret.AMPFY_RT++ * ret.a.size(), 0);
			ret.a[i + j] = up;
		}
	}
	ret.len = i + j;
	while (ret.a.size() <= (i + j))
		ret.a.resize(ret.AMPFY_RT++ * ret.a.size(), 0);
	while (ret.a[ret.len - 1] == 0 && ret.len > 1)
		ret.len--;
	return move(ret);
}
inline BigNum BigNum::operator/(const int64_t & b) const
{
	BigNum ret;
	// resize ret
	ret.a.resize(len, 0);
	int64_t i, down = 0;
	for (i = len - 1; i >= 0; i--) {
		ret.a[i] = (a[i] + down * (MAXN + 1)) / b;
		down = a[i] + down * (MAXN + 1) - ret.a[i] * b;
	}
	ret.len = len;
	while (ret.a[ret.len - 1] == 0 && ret.len > 1)
		ret.len--;
	return move(ret);
}
inline int64_t BigNum::operator %(const int64_t & b) const
{
	int64_t i, d = 0;
	for (i = len - 1; i >= 0; i--) {
		d = ((d * (MAXN + 1)) % b + a[i]) % b;
	}
	return d;
}
inline BigNum BigNum::operator^(const BigNum &n) const
{
	if (*this == 1) {
		return BigNum(1);
	}
	BigNum t, ret(1);
	int64_t i;
	BigNum m(n);
	while (m > 1) {
		t = *this;
		for (i = 1; !(BigNum(i << 1) > m); i <<= 1) {
			t = t * t;
		}
		m = m - i;
		ret = ret * t;
		if (m.len == 1 && m.a[0] == 1)
			ret = ret * (*this);
	}
	return move(ret);
}
inline bool BigNum::operator>(const BigNum & T) const
{
	int64_t ln;
	if (len > T.len)
		return true;
	else if (len == T.len) {
		ln = len - 1;
		while (a[ln] == T.a[ln] && ln > 0)
			ln--;
		if (ln >= 0 && a[ln] > T.a[ln])
			return true;
		else
			return false;
	}
	else
		return false;
}


inline bool BigNum::operator<(const BigNum & T) const
{
	return !((*this) > T || (*this) == T);
}

inline bool BigNum::operator==(const BigNum & T) const
{
	return this->len == T.len && this->a == T.a;
}

inline BigNum BigNum::operator!() const
{
	BigNum ret(1), n(*this);
	while (n > 1) {
		ret = ret * n;
		n = n - 1;
	}
	return move(ret);
}

inline void BigNum::print() const
{
	int64_t i;
	printf("%lld", a[len - 1]);
	for (i = len - 2; i >= 0; i--) {
		printf("%08lld", a[i]);
	}
	printf("\n");
}

inline void BigNum::print(FILE *f) const
{
	int64_t i;
	fprintf(f, "%lld", a[len - 1]);
	for (i = len - 2; i >= 0; i--) {
		fprintf(f, "%08lld", a[i]);
	}
	fprintf(f, "\n");
}
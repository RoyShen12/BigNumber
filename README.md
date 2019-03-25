# BigNumber
a C++ big integer class


ensure to compile under x64 architecture.

use std::basic_string<int64_t> to store splited interger.

#example:

```
// created by int64_t
BigNum a(123456789);
```

```
// created by const char *
BigNum b("999999999999999999999999999999999999999999999999999999999");
```

```
// empty constructor, c = 0
BigNum c();
```

```
// precise calculating operator + - * / % both with int64_t or another BigNum('/' and '%' only support int64_t)
// on a very fast speed.

auto ans = ((((a * b + b * b * b) / 655) * b) - 12345) % 255;

// ans = 213 , this line only cost about 20μs.
```

```
// very fast pre++ and post++
auto ans2 = b++; ++ans2.print();
```

```
// unlimited digits ^ and ! operation.
// ^
BigNum c(12345);
auto ans3 = c ^ c;

// ans3 has 50510 digits, but this line only cost about 70ms.

// operator! in BigNum has been overrided as factorial
BigNum c(10000);
auto ans4 = !c;
// this line cost about 400ms, the ans4 has 35660 digits.
```

```
// use .print() to print a BigNum to console

b.print();
```

```
// *: also use .print(FILE*) to output a BigNum to _iobuf*

//    or >> and << operator to input/output a BigNum
```

#include "../Bnum.h"

#include <chrono>

#define PAD_LENGTH 40

string make_colored_str(const string& text, const char* color_code) {
  return string("\033[3") + string(color_code) + string("m ") + text + string("\033[0m");
}

string to_red(const string& text) {
  return make_colored_str(text, "1");
}

string to_green(const string& text) {
  return make_colored_str(text, "2");
}

void run_test_suit(const string& suit_description, const function<bool ()>&& __Fx) {
  auto is_ok = __Fx();
  auto pad_end = PAD_LENGTH - suit_description.length();
  string pad;
  for (int32_t i = 0; i < pad_end; i++) {
    pad.append(" ");
  }
  std::cout << suit_description << pad << ( is_ok ? to_green("ok.") : to_red("failed.") ) << std::endl;
}

int main() {
  run_test_suit("constructor empty", []() {
    BigNum test;
    return test.to_string() == "0";
  });

  run_test_suit("constructor from int", []() {
    BigNum test(1);
    BigNum test2(1245214364563234212);
    return test.to_string() == "1" && test2.to_string() == "1245214364563234212";
  });

  run_test_suit("constructor from const char*", []() {
    BigNum test("1232142543456743543");
    return test.to_string() == "1232142543456743543";
  });

  run_test_suit("constructor from another BigNum", []() {
    BigNum test(1);
    BigNum test2(test);
    return test.to_string() == test2.to_string();
  });

  run_test_suit("operator == / != with another BigNum", []() {
    BigNum test(1);
    BigNum test2(2);
    BigNum test3(1);
    return test != test2 && test3 == test;
  });

  run_test_suit("operator +", []() {
    BigNum test1(1);
    BigNum test2(2);

    BigNum test3("214364564325243");
    BigNum test4("768765474567545");
    return (test1 + test2).to_string() == "3" && (test3 + test4).to_string() == "983130038892788";
  });

  run_test_suit("operator ++", []() {
    BigNum test1(1);
    BigNum test2("214364564325243");
    BigNum test3(2);
    BigNum test4("543754743535324");
    return (test1++).to_string() == "1" && test1.to_string() == "2"
      && (test2++).to_string() == "214364564325243" && test2.to_string() == "214364564325244"
      && (++test3).to_string() == "3" && test3.to_string() == "3"
      && (++test4).to_string() == "543754743535325" && test4.to_string() == "543754743535325";
  });

  return EXIT_SUCCESS;
}

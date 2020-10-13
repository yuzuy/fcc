#!/bin/bash

assert() {
  expected="$1"
  input="$2"

  ./fcc "$input" > tmp.s
  cc -o tmp tmp.s
  ./tmp
  actual="$?"

  if [ "$actual" == "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected. got=$actual"
    exit 1
  fi
}

assert 123 "123;"
assert 55 "55;"
assert 178 "123+55;"
assert 68 "123-55;"
assert 59 "23 + 50 - 14;"
assert 42 "50-4*4/2;"
assert 100 "(5+20)*4;"
assert 33 "(55+44)/3;"
assert 10 "-10+20;"
assert 1 "1 == 1;"
assert 1 "1 != 2;"
assert 0 "1 == 2;"
assert 0 "1 != 1;"
assert 1 "1 < 2;"
assert 1 "2 > 1;"
assert 1 "1 <= 2;"
assert 1 "1 <= 1;"
assert 1 "2 >= 1;"
assert 1 "1 >= 1;"
assert 0 "1 > 2;"
assert 0 "2 < 1;"
assert 0 "1 >= 2;"
assert 0 "2 <= 1;"
assert 42 "a=42; a;"
assert 50 "foo=50; foo;"
assert 100 "bar = 100; return bar; 1;"

echo OK

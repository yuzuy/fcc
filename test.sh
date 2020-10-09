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

assert 123 123
assert 55 55
assert 178 "123+55"
assert 68 "123-55"
assert 59 "23 + 50 - 14"
assert 42 "50-4*4/2"
assert 100 "(5+20)*4"
assert 33 "(55+44)/3"
assert 10 "-10+20"

echo OK

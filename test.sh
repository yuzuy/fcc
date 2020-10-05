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

echo OK

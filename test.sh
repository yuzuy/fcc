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

echo OK

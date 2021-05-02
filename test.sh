#!/bin/bash

assert() {
  expected="$1"
  input="$2"

  echo -e "\n\n\n===== $input ====="

  ./chibicc "$input" > tmp.s || exit
  # gcc -static -o tmp tmp.s

  echo -e "\n[tmp.s]"
  cat tmp.s

  gcc -o tmp tmp.s
  ./tmp
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo -e "\n[result]"
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

assert 0 0
assert 42 42
assert 21 '5+20-4'
assert 41 ' 12 + 34 - 5 '
assert 47 '5+6*7'
assert 15 '5*(9-6)'
assert 4 '(3+5)/2'
assert 10 '-10+20'
assert 10 '- -10'
assert 10 '- - +10'

echo -e "\n\nOK"

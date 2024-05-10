#!/usr/bin/bash

assert() {
	expected="$1"
	input="$2"

	eval "./atrcc $input > tmp.s"
	eval "cc -o tmp tmp.s"
	eval "./tmp"
	actual="$?"

	if [[ "$expected" == "$actual" ]]; then
		echo "$input => $actual"
	else
		echo "$input => $expected expected, but got $actual"
		exit 1
	fi
}

assert 0 0
assert 42 42

echo OK

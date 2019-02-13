#!/usr/bin/env bash

for f in *.cpp; do
    echo $f
    clang-format -style=google -i $f
done

for f in *.hpp; do
    echo $f
    clang-format -style=google -i $f
done

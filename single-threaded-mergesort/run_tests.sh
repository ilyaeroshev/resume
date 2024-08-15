#!/bin/bash

TEMP_FILE="test.out"

echo -e "UNIT TESTS:\n"
g++ --std=c++20 -Wall -Wextra -fsanitize=address,undefined -I . test/unit.cpp -o $TEMP_FILE && \
./$TEMP_FILE && \

echo -e "\nRANDOM TESTS:\n"
g++ --std=c++20 -Wall -Wextra -fsanitize=address,undefined -I . test/random.cpp -o $TEMP_FILE && \
./$TEMP_FILE && \

echo -e "\nPERFORMANCE TESTS:\n" && \
g++ --std=c++20 -Wall -Wextra -O3 -I . test/performance.cpp -o $TEMP_FILE && \
./$TEMP_FILE

test -f $TEMP_FILE && rm $TEMP_FILE

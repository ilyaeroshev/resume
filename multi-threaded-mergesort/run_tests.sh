#!/bin/bash

TEMP_FILE="test.out"

echo -e "UNIT TESTS:\n"
g++ --std=c++20 -Wall -Wextra -fsanitize=address,undefined -I . src/*/*.cpp test/unit.cpp -o $TEMP_FILE && \
./$TEMP_FILE && \

echo -e "\nRANDOM TESTS:\n"
g++ --std=c++20 -Wall -Wextra -fsanitize=address,undefined -I . src/*/*.cpp test/random.cpp -o $TEMP_FILE && \
./$TEMP_FILE && \

echo -e "\nSTRESS TESTS WITH ADDRESS SANITIZER:\n" && \
g++ --std=c++20 -Wall -Wextra -O2 -fsanitize=address -I . src/*/*.cpp test/stress.cpp -o $TEMP_FILE && \
./$TEMP_FILE && \

echo -e "\nSTRESS TESTS WITH THREAD SANITIZER:\n" && \
g++ --std=c++20 -Wall -Wextra -O2 -fsanitize=thread -I . src/*/*.cpp test/stress.cpp -o $TEMP_FILE && \
./$TEMP_FILE && \

echo -e "\nPERFORMANCE TESTS:\n" && \
g++ --std=c++20 -Wall -Wextra -O3 -I . src/*/*.cpp test/performance.cpp -o $TEMP_FILE && \
./$TEMP_FILE

test -f $TEMP_FILE && rm $TEMP_FILE

#!/bin/bash

TEMP_FILE="play.out"

g++ --std=c++20 -Wall -Wextra -fsanitize=thread -I . src/*/*.cpp play/main.cpp -o $TEMP_FILE && \
# g++ --std=c++20 -Wall -Wextra -fsanitize=address,undefined -I . src/*/*.cpp play/main.cpp -o $TEMP_FILE && \
./$TEMP_FILE

test -f $TEMP_FILE && rm $TEMP_FILE
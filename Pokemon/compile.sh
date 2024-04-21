#!/bin/bash

# 현재 디렉토리에 있는 main.c와 pokemon.h 파일을 컴파일
gcc -o pokemon main.c

# 컴파일이 성공적으로 완료되었는지 확인
if [ $? -eq 0 ]; then
    echo "컴파일이 성공적으로 완료되었습니다."
else
    echo "컴파일에 실패하였습니다."
fi

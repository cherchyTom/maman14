cmake_minimum_required(VERSION 3.5)
project(maman14 C)

set(CMAKE_C_STANDARD 90)

add_library(maman14 test.c test.h assembler.h symbolList.c symbolList.h constsTables.c constsTables.h txtAnalyzer.c txtAnalyzer.h firstRead.c firstRead.h utils.c utils.h memoryWord.c memoryWord.h labelsQueue.c labelsQueue.h)
add_executable(prog test.c utils.c symbolList.c constsTables.c txtAnalyzer.c firstRead.c memoryWord.c memoryWord.h memoryWord.c labelsQueue.c labelsQueue.h)
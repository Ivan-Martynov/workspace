cmake_minimum_required(VERSION 3.10)

# project(CommandDesignPattern VERSION 1.0)

set(TARGETS
    example1
    giant
    number
    person
)

foreach(TARGET ${TARGETS})
  add_executable(command_${TARGET}.exe src/${TARGET}.cpp)
endforeach(TARGET)

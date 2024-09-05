set(TARGETS
    example1
    giant
    number
    person
)

foreach(TARGET ${TARGETS})
    add_executable(command_${TARGET} src/${TARGET}.cpp)
    target_link_libraries(command_${TARGET} PUBLIC compiler_flags)
endforeach(TARGET)

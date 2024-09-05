set(TARGETS
    concept
    justify
)

foreach(TARGET ${TARGETS})
    add_executable(strategy_${TARGET} src/${TARGET}.cpp)
    target_link_libraries(strategy_${TARGET} PUBLIC compiler_flags)
endforeach(TARGET)

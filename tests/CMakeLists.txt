find_package(Qt6 REQUIRED COMPONENTS Test)

function(birch_add_test name)
    qt_add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} PRIVATE birch_core Qt6::Test)
    target_compile_definitions(${name} PRIVATE
        BIRCH_EXAMPLES_DIR="${PROJECT_SOURCE_DIR}/resources/examples")
    birch_set_warnings(${name})
    # Консольное приложение, без бандла
    set_target_properties(${name} PROPERTIES MACOSX_BUNDLE OFF)
    add_test(NAME ${name} COMMAND ${name})
    # Без окон: тесты работают и на CI без дисплея
    set_tests_properties(${name} PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
endfunction()

birch_add_test(tst_parser)
birch_add_test(tst_renderer)

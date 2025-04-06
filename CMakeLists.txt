# CMakeLists.txt: проект CMake для AbstractScriptC (версия на C); включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required(VERSION 3.8)

# Настройка для компилятора MSVC (по желанию)
if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_CLANG)
    # Добавление флагов компиляции для GCC и Clang
    add_compile_options(-Wall -Wextra -Wpedantic) # Включаем больше предупреждений
    # Добавление флагов компиляции для отладки
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-g) # Добавляем информацию для отладки
    endif()
endif()

project("AbstractScriptC" C) # Указываем язык C

# Добавьте источник в исполняемый файл этого проекта.
add_executable(AbstractScriptC "AbstractScriptC.c" "AbstractScriptC.h") # Указываем расширение .c

# Установка стандарта C (по желанию)
set(CMAKE_C_STANDARD 17) # Или другой стандарт, например, 99 или 17
set(CMAKE_C_STANDARD_REQUIRED TRUE) # Требовать указанный стандарт

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
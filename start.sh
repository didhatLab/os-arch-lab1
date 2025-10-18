#!/bin/bash

# Скрипт для полной пересборки и запуска юнит-теста Eco.Lab1
# Автор: AI Assistant
# Дата: $(date)

set -e  # Остановка при любой ошибке

# Проверяем переменную окружения ECO_FRAMEWORK
if [ -z "$ECO_FRAMEWORK" ]; then
    echo "ОШИБКА: Переменная окружения ECO_FRAMEWORK не установлена!"
    echo "Установите её, например:"
    echo "export ECO_FRAMEWORK=/home/dan/shit/Eco.Framework"
    exit 1
fi

echo "=========================================="
echo "Eco.Lab1 - Полная пересборка и запуск теста"
echo "ECO_FRAMEWORK: $ECO_FRAMEWORK"
echo "=========================================="

# Переходим в директорию сборки
cd "$(dirname "$0")/AssemblyFiles/Linux/x86_64_gcc_11_4_0"

echo "1. Очистка старых файлов..."
# Удаляем старые библиотеки и объектные файлы
rm -f ../../../BuildFiles/Linux/x86_64/StaticDebug/lib*.a
rm -f ../../../BuildFiles/Linux/x86_64/StaticDebug/*.o
rm -f ../../../BuildFiles/Linux/x86_x64/StaticDebug/EcoLab1
rm -f ../../../SourceFiles/*.o
rm -f ../../../UnitTestFiles/SourceFiles/*.o

echo "2. Сборка CalculatorA..."
# Собираем CalculatorA как статическую библиотеку
gcc -I ../../../Eco.CalculatorA/HeaderFiles \
    -I ../../../Eco.CalculatorA/SharedFiles \
    -I $ECO_FRAMEWORK/Eco.Core1/SharedFiles \
    -I $ECO_FRAMEWORK/Eco.InterfaceBus1/SharedFiles \
    -I $ECO_FRAMEWORK/Eco.MemoryManager1/SharedFiles \
    -DECO_LIB -DECO_LINUX -DECO_X86_64 -DUGUID_UTILITY \
    -Wall -O3 -g -c ../../../Eco.CalculatorA/SourceFiles/CEcoCalculatorA.c \
    -o ../../../BuildFiles/Linux/x86_64/StaticDebug/CEcoCalculatorA.o

gcc -I ../../../Eco.CalculatorA/HeaderFiles \
    -I ../../../Eco.CalculatorA/SharedFiles \
    -I $ECO_FRAMEWORK/Eco.Core1/SharedFiles \
    -I $ECO_FRAMEWORK/Eco.InterfaceBus1/SharedFiles \
    -I $ECO_FRAMEWORK/Eco.MemoryManager1/SharedFiles \
    -DECO_LIB -DECO_LINUX -DECO_X86_64 -DUGUID_UTILITY \
    -Wall -O3 -g -c ../../../Eco.CalculatorA/SourceFiles/CEcoCalculatorAFactory.c \
    -o ../../../BuildFiles/Linux/x86_64/StaticDebug/CEcoCalculatorAFactory.o

ar rcs ../../../BuildFiles/Linux/x86_64/StaticDebug/lib4828F6552E4540E78121EBD220DC360E.a \
    ../../../BuildFiles/Linux/x86_64/StaticDebug/CEcoCalculatorA.o \
    ../../../BuildFiles/Linux/x86_64/StaticDebug/CEcoCalculatorAFactory.o

echo "3. Сборка CalculatorB..."
# Собираем CalculatorB как статическую библиотеку
gcc -I ../../../Eco.CalculatorB/HeaderFiles \
    -I ../../../Eco.CalculatorB/SharedFiles \
    -I $ECO_FRAMEWORK/Eco.Core1/SharedFiles \
    -I $ECO_FRAMEWORK/Eco.InterfaceBus1/SharedFiles \
    -I $ECO_FRAMEWORK/Eco.MemoryManager1/SharedFiles \
    -DECO_LIB -DECO_LINUX -DECO_X86_64 -DUGUID_UTILITY \
    -Wall -O3 -g -c ../../../Eco.CalculatorB/SourceFiles/CEcoCalculatorB.c \
    -o ../../../BuildFiles/Linux/x86_64/StaticDebug/CEcoCalculatorB.o

gcc -I ../../../Eco.CalculatorB/HeaderFiles \
    -I ../../../Eco.CalculatorB/SharedFiles \
    -I $ECO_FRAMEWORK/Eco.Core1/SharedFiles \
    -I $ECO_FRAMEWORK/Eco.InterfaceBus1/SharedFiles \
    -I $ECO_FRAMEWORK/Eco.MemoryManager1/SharedFiles \
    -DECO_LIB -DECO_LINUX -DECO_X86_64 -DUGUID_UTILITY \
    -Wall -O3 -g -c ../../../Eco.CalculatorB/SourceFiles/CEcoCalculatorBFactory.c \
    -o ../../../BuildFiles/Linux/x86_64/StaticDebug/CEcoCalculatorBFactory.o

ar rcs ../../../BuildFiles/Linux/x86_64/StaticDebug/libAE202E543CE54550899603BD70C62565.a \
    ../../../BuildFiles/Linux/x86_64/StaticDebug/CEcoCalculatorB.o \
    ../../../BuildFiles/Linux/x86_64/StaticDebug/CEcoCalculatorBFactory.o

echo "4. Сборка CalculatorD..."
# Собираем CalculatorD как статическую библиотеку
gcc -I ../../../Eco.CalculatorD/HeaderFiles \
    -I ../../../Eco.CalculatorD/SharedFiles \
    -I ../../../Eco.CalculatorB/SharedFiles \
    -I ../../../Eco.CalculatorA/SharedFiles \
    -I $ECO_FRAMEWORK/Eco.Core1/SharedFiles \
    -I $ECO_FRAMEWORK/Eco.InterfaceBus1/SharedFiles \
    -I $ECO_FRAMEWORK/Eco.MemoryManager1/SharedFiles \
    -DECO_LIB -DECO_LINUX -DECO_X86_64 -DUGUID_UTILITY \
    -Wall -O3 -g -c ../../../Eco.CalculatorD/SourceFiles/CEcoCalculatorD.c \
    -o ../../../BuildFiles/Linux/x86_64/StaticDebug/CEcoCalculatorD.o

gcc -I ../../../Eco.CalculatorD/HeaderFiles \
    -I ../../../Eco.CalculatorD/SharedFiles \
    -I ../../../Eco.CalculatorB/SharedFiles \
    -I ../../../Eco.CalculatorA/SharedFiles \
    -I $ECO_FRAMEWORK/Eco.Core1/SharedFiles \
    -I $ECO_FRAMEWORK/Eco.InterfaceBus1/SharedFiles \
    -I $ECO_FRAMEWORK/Eco.MemoryManager1/SharedFiles \
    -DECO_LIB -DECO_LINUX -DECO_X86_64 -DUGUID_UTILITY \
    -Wall -O3 -g -c ../../../Eco.CalculatorD/SourceFiles/CEcoCalculatorDFactory.c \
    -o ../../../BuildFiles/Linux/x86_64/StaticDebug/CEcoCalculatorDFactory.o

ar rcs ../../../BuildFiles/Linux/x86_64/StaticDebug/lib3A8E44677E82475CB4A3719ED8397E61.a \
    ../../../BuildFiles/Linux/x86_64/StaticDebug/CEcoCalculatorD.o \
    ../../../BuildFiles/Linux/x86_64/StaticDebug/CEcoCalculatorDFactory.o

echo "5. Сборка CalculatorC..."
# Собираем CalculatorC как статическую библиотеку
gcc -I ../../../Eco.CalculatorC/HeaderFiles \
    -I ../../../Eco.CalculatorC/SharedFiles \
    -I $ECO_FRAMEWORK/Eco.Core1/SharedFiles \
    -I $ECO_FRAMEWORK/Eco.InterfaceBus1/SharedFiles \
    -I $ECO_FRAMEWORK/Eco.MemoryManager1/SharedFiles \
    -DECO_LIB -DECO_LINUX -DECO_X86_64 -DUGUID_UTILITY \
    -Wall -O3 -g -c ../../../Eco.CalculatorC/SourceFiles/CEcoCalculatorC.c \
    -o ../../../BuildFiles/Linux/x86_64/StaticDebug/CEcoCalculatorC.o

gcc -I ../../../Eco.CalculatorC/HeaderFiles \
    -I ../../../Eco.CalculatorC/SharedFiles \
    -I $ECO_FRAMEWORK/Eco.Core1/SharedFiles \
    -I $ECO_FRAMEWORK/Eco.InterfaceBus1/SharedFiles \
    -I $ECO_FRAMEWORK/Eco.MemoryManager1/SharedFiles \
    -DECO_LIB -DECO_LINUX -DECO_X86_64 -DUGUID_UTILITY \
    -Wall -O3 -g -c ../../../Eco.CalculatorC/SourceFiles/CEcoCalculatorCFactory.c \
    -o ../../../BuildFiles/Linux/x86_64/StaticDebug/CEcoCalculatorCFactory.o

ar rcs ../../../BuildFiles/Linux/x86_64/StaticDebug/lib4828F6552E4540E78121EBD220DC360E.a \
    ../../../BuildFiles/Linux/x86_64/StaticDebug/CEcoCalculatorC.o \
    ../../../BuildFiles/Linux/x86_64/StaticDebug/CEcoCalculatorCFactory.o

echo "6. Сборка CalculatorE..."
# Собираем CalculatorE как статическую библиотеку
gcc -I ../../../Eco.CalculatorE/HeaderFiles \
    -I ../../../Eco.CalculatorE/SharedFiles \
    -I ../../../Eco.CalculatorB/SharedFiles \
    -I $ECO_FRAMEWORK/Eco.Core1/SharedFiles \
    -I $ECO_FRAMEWORK/Eco.InterfaceBus1/SharedFiles \
    -I $ECO_FRAMEWORK/Eco.MemoryManager1/SharedFiles \
    -DECO_LIB -DECO_LINUX -DECO_X86_64 -DUGUID_UTILITY \
    -Wall -O3 -g -c ../../../Eco.CalculatorE/SourceFiles/CEcoCalculatorE.c \
    -o ../../../BuildFiles/Linux/x86_64/StaticDebug/CEcoCalculatorE.o

gcc -I ../../../Eco.CalculatorE/HeaderFiles \
    -I ../../../Eco.CalculatorE/SharedFiles \
    -I ../../../Eco.CalculatorB/SharedFiles \
    -I $ECO_FRAMEWORK/Eco.Core1/SharedFiles \
    -I $ECO_FRAMEWORK/Eco.InterfaceBus1/SharedFiles \
    -I $ECO_FRAMEWORK/Eco.MemoryManager1/SharedFiles \
    -DECO_LIB -DECO_LINUX -DECO_X86_64 -DUGUID_UTILITY \
    -Wall -O3 -g -c ../../../Eco.CalculatorE/SourceFiles/CEcoCalculatorEFactory.c \
    -o ../../../BuildFiles/Linux/x86_64/StaticDebug/CEcoCalculatorEFactory.o

ar rcs ../../../BuildFiles/Linux/x86_64/StaticDebug/lib872FEF1DE3314B87AD44D1E7C232C2F0.a \
    ../../../BuildFiles/Linux/x86_64/StaticDebug/CEcoCalculatorE.o \
    ../../../BuildFiles/Linux/x86_64/StaticDebug/CEcoCalculatorEFactory.o

echo "7. Сборка CEcoLab1..."
# Собираем CEcoLab1 как статическую библиотеку
TARGET=1 DEBUG=1 make -f Makefile

echo "8. Сборка юнит-теста..."
# Собираем юнит-тест
TARGET=1 DEBUG=1 make -f MakefileExe

echo "9. Запуск юнит-теста..."
echo "=========================================="
echo "РЕЗУЛЬТАТЫ ТЕСТИРОВАНИЯ:"
echo "=========================================="

# Запускаем тест
../../../BuildFiles/Linux/x86_x64/StaticDebug/EcoLab1

echo "=========================================="
echo "Тест завершен успешно!"
echo "=========================================="

# Очистка временных файлов
echo "10. Очистка временных файлов..."
rm -f ../../../BuildFiles/Linux/x86_64/StaticDebug/CEcoCalculator*.o
rm -f ../../../SourceFiles/*.o
rm -f ../../../UnitTestFiles/SourceFiles/*.o

echo "Готово! Все компоненты собраны и протестированы."

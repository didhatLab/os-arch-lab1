# Eco.Lab1 - Реализация Connection Points и Events

## Оглавление

1. [Описание проекта](#описание-проекта)
2. [Архитектура Connection Points](#архитектура-connection-points)
3. [Реализованные события](#реализованные-события)
4. [Схемы подключения](#схемы-подключения)
5. [Структура проекта](#структура-проекта)
6. [Сборка и запуск](#сборка-и-запуск)
7. [Примеры использования](#примеры-использования)
8. [Анализ и выводы](#анализ-и-выводы)

---

## Описание проекта

Проект **Eco.Lab1** представляет собой реализацию компонентной архитектуры на основе Eco.Framework с использованием механизма **Connection Points** для обработки событий. Основная функциональность включает:

- Реализацию алгоритма **Bucket Sort** для различных типов данных (int, long, float, double, long double)
- Интеграцию механизма событий через Connection Points
- Демонстрацию различных схем подключения (один-ко-многим, многие-к-одному, многие-ко-многим)
- Детальное логирование всех этапов сортировки через события

---

## Архитектура Connection Points

### Основные компоненты

#### 1. **IEcoConnectionPointContainer**
Интерфейс контейнера точек подключения, который позволяет:
- Находить точки подключения по IID (`FindConnectionPoint`)
- Перечислять все точки подключения (`EnumConnectionPoints`)

#### 2. **IEcoConnectionPoint**
Интерфейс точки подключения, которая:
- Управляет подключениями sink'ов (`Advise`/`Unadvise`)
- Перечисляет активные подключения (`EnumConnections`)
- Возвращает IID интерфейса событий (`GetConnectionInterface`)

#### 3. **IEcoLab1Events**
Интерфейс событий, который реализуется в sink'ах для получения уведомлений о событиях сортировки.

#### 4. **CEcoLab1ConnectionPoint**
Реализация точки подключения, которая:
- Хранит список подключенных sink'ов через `IEcoList1`
- Управляет жизненным циклом подключений
- Выдает cookie для каждого подключения

#### 5. **CEcoLab1Sink**
Реализация приемника событий (sink), который:
- Реализует интерфейс `IEcoLab1Events`
- Обрабатывает все события сортировки
- Логирует события в консоль

### Диаграмма архитектуры

```
┌─────────────────────────────────────────────────────────────┐
│                    CEcoLab1                                 │
│  ┌──────────────────────────────────────────────────────┐  │
│  │  IEcoLab1 (основной интерфейс)                      │  │
│  │  - BucketSortInt/Long/Float/Double/LongDouble       │  │
│  └──────────────────────────────────────────────────────┘  │
│  ┌──────────────────────────────────────────────────────┐  │
│  │  IEcoConnectionPointContainer                        │  │
│  │  - FindConnectionPoint                               │  │
│  │  - EnumConnectionPoints                              │  │
│  └──────────────────────────────────────────────────────┘  │
│                            │                               │
│                            ▼                               │
│  ┌──────────────────────────────────────────────────────┐  │
│  │  CEcoLab1ConnectionPoint                            │  │
│  │  ┌──────────────────────────────────────────────┐  │  │
│  │  │  IEcoConnectionPoint                         │  │  │
│  │  │  - Advise(sink) → cookie                     │  │  │
│  │  │  - Unadvise(cookie)                          │  │  │
│  │  │  - EnumConnections                           │  │  │
│  │  └──────────────────────────────────────────────┘  │  │
│  │  ┌──────────────────────────────────────────────┐  │  │
│  │  │  IEcoList1 (список sink'ов)                 │  │  │
│  │  │  - Sink1, Sink2, Sink3, ...                 │  │  │
│  │  └──────────────────────────────────────────────┘  │  │
│  └──────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────┘
                            │
                            │ Fire Events
                            ▼
┌─────────────────────────────────────────────────────────────┐
│              CEcoLab1Sink (Sink1, Sink2, ...)               │
│  ┌──────────────────────────────────────────────────────┐  │
│  │  IEcoLab1Events                                      │  │
│  │  - OnBucketSortStart                                 │  │
│  │  - OnMinMaxFound                                     │  │
│  │  - OnBucketsCreated                                  │  │
│  │  - OnElementDistributed                              │  │
│  │  - OnBucketSortingStarted                            │  │
│  │  - OnSortSwap                                        │  │
│  │  - OnBucketSorted                                    │  │
│  │  - OnBucketSortCompleted                             │  │
│  └──────────────────────────────────────────────────────┘  │
│  ┌──────────────────────────────────────────────────────┐  │
│  │  Обработка событий и логирование                    │  │
│  └──────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────┘
```

---

## Реализованные события

Все события связаны с процессом сортировки Bucket Sort и предоставляют детальную информацию о каждом этапе алгоритма.

### Детальные события Bucket Sort

1. **OnBucketSortStart(arrayLength)**
   - Вызывается в начале сортировки
   - Параметры: длина массива
   - Пример: `OnBucketSortStart: Starting bucket sort for array of length 9`

2. **OnMinMaxFound(minValue, maxValue)**
   - Вызывается после нахождения минимального и максимального значений
   - Параметры: минимальное и максимальное значения, диапазон
   - Пример: `OnMinMaxFound: Min=-4.00, Max=9.00, Range=13.00`

3. **OnBucketsCreated(bucketCount)**
   - Вызывается после создания корзин
   - Параметры: количество корзин
   - Пример: `OnBucketsCreated: Created 9 buckets`

4. **OnElementDistributed(elementIndex, bucketIndex)**
   - Вызывается при распределении каждого элемента в корзину
   - Параметры: индекс элемента, индекс корзины
   - Пример: `OnElementDistributed: Element[0] -> Bucket[4]`

5. **OnBucketSortingStarted(bucketIndex, bucketSize)**
   - Вызывается перед сортировкой каждой корзины
   - Параметры: индекс корзины, размер корзины
   - Пример: `OnBucketSortingStarted: Sorting bucket[0] with 6 elements`

6. **OnSortSwap(indexSrc, indexDst)**
   - Вызывается при каждом обмене элементов во время сортировки
   - Параметры: индекс источника, индекс назначения
   - Пример: `Step 1 : swapping places 6 <-> 5`

7. **OnBucketSorted(bucketIndex)**
   - Вызывается после сортировки каждой корзины
   - Параметры: индекс корзины
   - Пример: `OnBucketSorted: Bucket[0] sorted`

8. **OnBucketSortCompleted()**
   - Вызывается после завершения всей сортировки
   - Пример: `OnBucketSortCompleted: Bucket sort finished successfully`

### Последовательность вызова событий

```
BucketSortInt/Long/Float/Double/LongDouble()
    │
    ├─► OnBucketSortStart(length)
    │
    ├─► OnMinMaxFound(min, max)
    │
    ├─► OnBucketsCreated(bucketCount)
    │
    ├─► OnElementDistributed(elementIndex, bucketIndex) [для каждого элемента]
    │
    ├─► Для каждой корзины:
    │   ├─► OnBucketSortingStarted(bucketIndex, bucketSize)
    │   ├─► OnSortSwap(indexSrc, indexDst) [для каждого обмена]
    │   └─► OnBucketSorted(bucketIndex)
    │
    ├─► OnBucketSortCompleted()
```

---

## Схемы подключения

Проект демонстрирует три основные схемы подключения событий:

### Схема 1: Несколько приемников → Один объект (многие-к-одному)

**Описание:** Несколько sink'ов подключаются к одному объекту CEcoLab1.

**Диаграмма:**
```
    Sink1 ──┐
    Sink2 ──┼──► CEcoLab1 (Object 1)
    Sink3 ──┘
```

**Характеристики:**
- Каждое событие вызывается **трижды** (по одному разу для каждого sink'а)
- Все sink'и получают одинаковые события от одного объекта
- Полезно для: логирования, мониторинга, статистики

**Пример вывода:**
```
--- Схема 1: Несколько приемников -> Один объект (многие-к-одному) ---
Sink1 подключен к объекту 1
Sink2 подключен к объекту 1
Sink3 подключен к объекту 1

Тест сортировки с несколькими sink'ами (объект 1): 3 1 4 1 5 
OnBucketSortStart: Starting bucket sort for array of length 5
OnBucketSortStart: Starting bucket sort for array of length 5  ← дублирование
OnBucketSortStart: Starting bucket sort for array of length 5  ← дублирование
OnMinMaxFound: Min=1.00, Max=5.00, Range=4.00
OnMinMaxFound: Min=1.00, Max=5.00, Range=4.00  ← дублирование
OnMinMaxFound: Min=1.00, Max=5.00, Range=4.00  ← дублирование
...
```

### Схема 2: Один приемник → Несколько объектов (один-ко-многим)

**Описание:** Один sink подключается к нескольким объектам CEcoLab1.

**Диаграмма:**
```
    Sink1 ──► CEcoLab1 (Object 1)
    Sink1 ──► CEcoLab1 (Object 2)
    Sink1 ──► CEcoLab1 (Object 3)
```

**Пример вывода:**
```
--- Схема 2: Один приемник -> Несколько объектов (один-ко-многим) ---
Sink1 подключен к объекту 1
Sink1 подключен к объекту 2
Sink1 подключен к объекту 3


```

### Схема 3: Несколько приемников ↔ Несколько объектов (многие-ко-многим)

**Описание:** Несколько sink'ов подключаются к нескольким объектам CEcoLab1.

**Диаграмма:**
```
    Sink1 ──► CEcoLab1 (Object 1)
    Sink2 ──► CEcoLab1 (Object 2)
    Sink3 ──► CEcoLab1 (Object 3)
```

**Характеристики:**
- Каждый объект имеет свой sink
- Возможность расширения: один sink может подключаться к нескольким объектам
- Полезно для: сложных сценариев, когда нужна гибкая конфигурация

**Пример вывода:**
```
--- Схема 3: Несколько приемников <-> Несколько объектов (многие-ко-многим) ---
Sink1 подключен к объекту 1
Sink2 подключен к объекту 2
Sink3 подключен к объекту 3

Тест сортировки на объекте 1 (Sink1 слушает): 6 3 9 
OnBucketSortStart: Starting bucket sort for array of length 3
...

Тест сортировки на объекте 2 (Sink2 слушает): 4 7 2 
OnBucketSortStart: Starting bucket sort for array of length 3
...

Тест сортировки на объекте 3 (Sink3 слушает): 8 1 5 
OnBucketSortStart: Starting bucket sort for array of length 3
...
```

---

## Структура проекта

```
Eco.Lab1/
├── HeaderFiles/                    # Заголовочные файлы компонентов
│   ├── CEcoLab1.h                 # Основной компонент
│   ├── CEcoLab1Factory.h          # Фабрика компонента
│   ├── CEcoLab1ConnectionPoint.h  # Точка подключения
│   ├── CEcoLab1EnumConnectionPoints.h
│   └── CEcoLab1EnumConnections.h
│
├── SourceFiles/                    # Исходные файлы компонентов
│   ├── CEcoLab1.c                 # Реализация основного компонента
│   ├── CEcoLab1Factory.c          # Реализация фабрики
│   ├── CEcoLab1ConnectionPoint.c  # Реализация точки подключения
│   ├── CEcoLab1EnumConnectionPoints.c
│   └── CEcoLab1EnumConnections.c
│
├── SharedFiles/                    # Общие интерфейсы
│   ├── IEcoLab1.h                 # Основной интерфейс
│   ├── IEcoLab1Events.h           # Интерфейс событий
│   ├── IEcoConnectionPoint.h
│   ├── IEcoConnectionPointContainer.h
│   ├── IEcoEnumConnectionPoints.h
│   └── IEcoEnumConnections.h
│
├── UnitTestFiles/                  # Юнит-тесты
│   ├── HeaderFiles/
│   │   └── CEcoLab1Sink.h         # Заголовок sink'а
│   └── SourceFiles/
│       ├── EcoLab1.c              # Основной тест
│       └── CEcoLab1Sink.c         # Реализация sink'а
│
└── AssemblyFiles/                  # Файлы сборки
    └── Linux/
        └── x86_64_gcc_11_4_0/
            ├── Makefile            # Сборка библиотеки
            └── MakefileExe         # Сборка исполняемого файла
```

### Ключевые файлы

#### `SourceFiles/CEcoLab1.c`
- Реализация алгоритма Bucket Sort
- Функции `Fire_*` для вызова событий
- Интеграция Connection Point Container

#### `UnitTestFiles/SourceFiles/CEcoLab1Sink.c`
- Реализация всех обработчиков событий
- Логирование событий в консоль
- Методы `Advise`/`Unadvise` для подключения/отключения

#### `UnitTestFiles/SourceFiles/EcoLab1.c`
- Демонстрация всех трех схем подключения
- Тесты сортировки для различных типов данных
- Примеры использования Connection Points

---

## Сборка и запуск

### Требования

- Linux (x86_64)
- GCC 11.4.0 или выше
- Eco.Framework (должен быть установлен в `$ECO_FRAMEWORK`)

### Сборка

Используйте скрипт `start.sh` для полной пересборки и запуска тестов:

```bash
bash start.sh
```

Скрипт выполняет:
1. Очистку старых файлов
2. Сборку всех зависимых компонентов (CalculatorA-E)
3. Сборку основного компонента CEcoLab1
4. Сборку исполняемого файла с тестами
5. Запуск юнит-тестов
6. Очистку временных файлов

### Ручная сборка

```bash
# Сборка библиотеки
cd AssemblyFiles/Linux/x86_64_gcc_11_4_0
TARGET=1 DEBUG=1 make -f Makefile

# Сборка исполняемого файла
TARGET=1 DEBUG=1 make -f MakefileExe

# Запуск тестов
cd ../../..
./BuildFiles/Linux/x86_x64/StaticDebug/EcoLab1
```

---

## Примеры использования

### Пример 1: Базовое подключение sink'а

```c
// Создание sink'а
IEcoLab1Events* pIEvents = 0;
CEcoLab1Sink* pSink = 0;
int16_t result = createCEcoLab1Sink(pIMem, &pIEvents);

if (result == 0 && pIEvents != 0) {
    pSink = (CEcoLab1Sink*)pIEvents;
    
    // Подключение к объекту
    if (pSink->Advise != 0) {
        pSink->Advise(pSink, pIEcoLab1);
    }
    
    // Выполнение сортировки (события будут автоматически вызываться)
    int arr[] = {5, 2, 8, 1, 9};
    pIEcoLab1->pVTbl->BucketSortInt(pIEcoLab1, arr, 5);
    
    // Отключение
    if (pSink->Unadvise != 0) {
        pSink->Unadvise(pSink, pIEcoLab1);
    }
    
    // Освобождение
    pIEvents->pVTbl->Release(pIEvents);
}
```

### Пример 2: Несколько sink'ов к одному объекту

```c
// Создание трех sink'ов
IEcoLab1Events* pIEvents1, *pIEvents2, *pIEvents3;
CEcoLab1Sink* pSink1, *pSink2, *pSink3;

createCEcoLab1Sink(pIMem, &pIEvents1);
createCEcoLab1Sink(pIMem, &pIEvents2);
createCEcoLab1Sink(pIMem, &pIEvents3);

pSink1 = (CEcoLab1Sink*)pIEvents1;
pSink2 = (CEcoLab1Sink*)pIEvents2;
pSink3 = (CEcoLab1Sink*)pIEvents3;

// Подключение всех sink'ов к одному объекту
pSink1->Advise(pSink1, pIEcoLab1);
pSink2->Advise(pSink2, pIEcoLab1);
pSink3->Advise(pSink3, pIEcoLab1);

// При сортировке каждое событие будет вызвано трижды
int arr[] = {3, 1, 4, 1, 5};
pIEcoLab1->pVTbl->BucketSortInt(pIEcoLab1, arr, 5);
```

### Пример 3: Использование QueryInterface для получения Connection Point Container

```c
IEcoConnectionPointContainer* pCPC = 0;
IEcoConnectionPoint* pCP = 0;

// Получение Connection Point Container
result = pIEcoLab1->pVTbl->QueryInterface(
    pIEcoLab1, 
    &IID_IEcoConnectionPointContainer, 
    (void**)&pCPC
);

if (result == 0 && pCPC != 0) {
    // Поиск точки подключения для событий
    result = pCPC->pVTbl->FindConnectionPoint(
        pCPC, 
        &IID_IEcoLab1Events, 
        &pCP
    );
    
    if (result == 0 && pCP != 0) {
        // Подключение sink'а
        uint32_t cookie = 0;
        result = pCP->pVTbl->Advise(
            pCP, 
            (IEcoUnknown*)pSink, 
            &cookie
        );
        
        // ... использование ...
        
        // Отключение
        pCP->pVTbl->Unadvise(pCP, cookie);
        pCP->pVTbl->Release(pCP);
    }
    
    pCPC->pVTbl->Release(pCPC);
}
```

---

## Анализ и выводы

### Преимущества реализации Connection Points

1. **Разделение ответственности**
   - Компонент CEcoLab1 не знает о конкретных реализациях sink'ов
   - Sink'и могут быть реализованы независимо
   - Легко добавлять новые sink'и без изменения основного компонента

2. **Гибкость подключения**
   - Поддержка множественных подключений
   - Динамическое подключение/отключение во время выполнения
   - Возможность реализации различных схем (один-ко-многим, многие-к-одному, многие-ко-многим)

3. **Детальное логирование**
   - Все этапы алгоритма сортировки отслеживаются через события
   - Возможность анализа производительности
   - Удобство отладки

4. **Расширяемость**
   - Легко добавлять новые типы событий
   - Расширяемая система событий для детального отслеживания процесса сортировки
   - Возможность создания специализированных sink'ов для разных целей

### Производительность

- **Overhead событий:** Минимальный, так как события вызываются только при наличии подключенных sink'ов
- **Память:** Каждое подключение требует хранения cookie и ссылки на sink
- **Масштабируемость:** Поддержка множественных подключений без деградации производительности

### Ограничения

1. **Синхронные события:** Все события вызываются синхронно, что может замедлить выполнение при большом количестве sink'ов
2. **Нет фильтрации:** Все sink'и получают все события, нет механизма подписки на конкретные события
3. **Управление памятью:** Необходимо явно управлять жизненным циклом подключений

### Рекомендации по улучшению

1. **Асинхронные события:** Реализация очереди событий для асинхронной обработки
2. **Фильтрация событий:** Механизм подписки на конкретные типы событий
3. **Статистика:** Встроенная статистика по количеству подключений и вызовам событий
4. **Валидация:** Проверка корректности sink'ов перед вызовом событий

### Выводы

Реализация Connection Points и Events успешно интегрирована в проект Eco.Lab1. Механизм работает корректно, поддерживает все требуемые схемы подключения и обеспечивает детальное логирование процесса сортировки. Проект демонстрирует правильное использование паттерна Observer через Connection Points в контексте компонентной архитектуры Eco.Framework.



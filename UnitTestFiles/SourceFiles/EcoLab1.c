/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   EcoLab1
 * </сводка>
 *
 * <описание>
 *   Данный исходный файл является точкой входа
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */


/* Eco OS */
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"
#include "IdEcoInterfaceBus1.h"
#include "IdEcoFileSystemManagement1.h"
#include "IdEcoLab1.h"
#include "../../HeaderFiles/CEcoLab1.h"
#include "../../Eco.CalculatorB/SharedFiles/IdEcoCalculatorB.h"
#include "../../Eco.CalculatorB/SharedFiles/IEcoCalculatorX.h"
/* Calculator A (X interface) */
#include "../../Eco.CalculatorA/SharedFiles/IdEcoCalculatorA.h"
/* Calculator D (Y interface) */
#include "../../Eco.CalculatorD/SharedFiles/IdEcoCalculatorD.h"
#include "../../Eco.CalculatorD/SharedFiles/IEcoCalculatorY.h"
/* Calculator C (delegation) */
#include "../../Eco.CalculatorC/SharedFiles/IdEcoCalculatorC.h"
/* Calculator E (delegation) */
#include "../../Eco.CalculatorE/SharedFiles/IdEcoCalculatorE.h"
#include "../HeaderFiles/CEcoLab1Sink.h"
#include "../../SharedFiles/IEcoLab1Events.h"
#include "IdEcoList1.h"
#include "stdio.h"

/*
 *
 * <сводка>
 *   Функция EcoMain
 * </сводка>
 *
 * <описание>
 *   Функция EcoMain - точка входа
 * </описание>
 *
 */
int16_t EcoMain(IEcoUnknown* pIUnk) {
    printf("EcoLab1 unit tests\n");
    int16_t result = -1;
    /* Указатель на системный интерфейс */
    IEcoSystem1* pISys = 0;
    /* Указатель на интерфейс работы с системной интерфейсной шиной */
    IEcoInterfaceBus1* pIBus = 0;
    /* Указатель на интерфейс работы с памятью */
    IEcoMemoryAllocator1* pIMem = 0;
    char_t* name = 0;
    char_t* copyName = 0;
    /* Указатель на тестируемый интерфейс */
    IEcoLab1* pIEcoLab1 = 0;
    IEcoCalculatorX* pIX = 0;
    /* Указатели на sink'и для событий */
    IEcoLab1Events* pIEvents1 = 0;
    IEcoLab1Events* pIEvents2 = 0;
    IEcoLab1Events* pIEvents3 = 0;
    CEcoLab1Sink* pSink1 = 0;
    CEcoLab1Sink* pSink2 = 0;
    CEcoLab1Sink* pSink3 = 0;
    
    /* Указатели на дополнительные объекты для демонстрации */
    IEcoLab1* pIEcoLab1_2 = 0;
    IEcoLab1* pIEcoLab1_3 = 0;

    /* Проверка и создание системного интрефейса */
    if (pISys == 0) {
        result = pIUnk->pVTbl->QueryInterface(pIUnk, &GID_IEcoSystem, (void **)&pISys);
        if (result != 0 && pISys == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
            goto Release;
        }
    }

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);
    if (result != 0 || pIBus == 0) {
        /* Освобождение в случае ошибки */
        goto Release;
    }
#ifdef ECO_LIB
    /* Регистрация статического компонента для работы со списком */
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoLab1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_1F5DF16EE1BF43B999A434ED38FE8F3A);
    if (result != 0 ) {
        /* Освобождение в случае ошибки */
        goto Release;
    }
    /* Регистрация статического компонента калькулятора B */
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoCalculatorB, (IEcoUnknown*)GetIEcoComponentFactoryPtr_AE202E543CE54550899603BD70C62565);
    if (result != 0 ) {
        goto Release;
    }
    /* Регистрация статического компонента калькулятора A */
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoCalculatorA, (IEcoUnknown*)GetIEcoComponentFactoryPtr_4828F6552E4540E78121EBD220DC360E);
    if (result != 0 ) {
        goto Release;
    }
    /* Регистрация статического компонента калькулятора D */
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoCalculatorD, (IEcoUnknown*)GetIEcoComponentFactoryPtr_3A8E44677E82475CB4A3719ED8397E61);
    if (result != 0 ) {
        goto Release;
    }
    /* Регистрация статического компонента калькулятора C */
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoCalculatorC, (IEcoUnknown*)GetIEcoComponentFactoryPtr_4828F6552E4540E78121EBD220DC360E);
    if (result != 0 ) {
        goto Release;
    }
    /* Регистрация статического компонента калькулятора E */
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoCalculatorE, (IEcoUnknown*)GetIEcoComponentFactoryPtr_872FEF1DE3314B87AD44D1E7C232C2F0);
    if (result != 0 ) {
        goto Release;
    }
    /* Регистрация статического компонента EcoList1 */
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoList1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_53884AFC93C448ECAA929C8D3A562281);
    if (result != 0 ) {
        goto Release;
    }
#endif

    /* Динамическая загрузка не используется: фабрика калькулятора регистрируется статически */
    /* Быстрая проверка: можем ли получить IEcoCalculatorY напрямую */
    {
        IEcoCalculatorY* pIYcheck = 0;
        int16_t rc = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorD, 0, &IID_IEcoCalculatorY, (void**)&pIYcheck);
        if (rc == 0 && pIYcheck != 0) {
            int32_t mul = pIYcheck->pVTbl->Multiplication(pIYcheck, 2, 7);
            printf("Direct Y: 2*7=%d\n", (int)mul);
            pIYcheck->pVTbl->Release(pIYcheck);
        } else {
            printf("Direct Y: QueryComponent failed\n");
        }
    }

    /* Получение интерфейса управления памятью */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoMemoryManager1, 0, &IID_IEcoMemoryAllocator1, (void**) &pIMem);
    /* Проверка */
    if (result != 0 || pIMem == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
        goto Release;
    }

    /* Выделение блока памяти */
    name = (char_t *)pIMem->pVTbl->Alloc(pIMem, 10);
    /* Заполнение блока памяти */
    pIMem->pVTbl->Fill(pIMem, name, 'a', 9);


    /* Получение тестируемого интерфейса */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoLab1, 0, &IID_IEcoLab1, (void**) &pIEcoLab1);
    if (result != 0 || pIEcoLab1 == 0) {
        /* Освобождение интерфейсов в случае ошибки */
        goto Release;
    }


    result = pIEcoLab1->pVTbl->MyFunction(pIEcoLab1, name, &copyName);

    /* ---------------- Calculator X tests (Addition/Subtraction) ---------------- */
    {
        int16_t rqi = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorX, (void**)&pIX);
        if (rqi == 0 && pIX != 0) {
            int32_t sum = pIX->pVTbl->Addition(pIX, 3, 5);
            int16_t diff = pIX->pVTbl->Subtraction(pIX, 10, 3);
            printf("CalculatorX: 3+5=%d, 10-3=%d\n", (int)sum, (int)diff);
            pIX->pVTbl->Release(pIX);
            pIX = 0;
        } else {
            printf("CalculatorX not available (QueryInterface failed)\n");
        }
    }

    /* ---------------- Calculator Y tests (Multiplication/Division) ---------------- */
    {
        IEcoCalculatorY* pIY = 0;
        int16_t rqiY = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorY, (void**)&pIY);
        if (rqiY == 0 && pIY != 0) {
            int32_t mul = pIY->pVTbl->Multiplication(pIY, 4, 6);
            int16_t div = pIY->pVTbl->Division(pIY, 15, 3);
            printf("CalculatorY: 4*6=%d, 15/3=%d\n", (int)mul, (int)div);
            pIY->pVTbl->Release(pIY);
            pIY = 0;
        } else {
            printf("CalculatorY not available (QueryInterface failed)\n");
        }
    }

    /* ---------------- Calculator methods via direct access to aggregated components ---------------- */
    {
        /* Получаем прямой доступ к структуре CEcoLab1 */
        CEcoLab1* pCMe = (CEcoLab1*)pIEcoLab1;
        
        /* Вызываем методы агрегированных компонентов напрямую */
        if (pCMe->m_pICalculatorC_X != 0) {
            int32_t add_result = pCMe->m_pICalculatorC_X->pVTbl->Addition(pCMe->m_pICalculatorC_X, 7, 8);
            int16_t sub_result = pCMe->m_pICalculatorC_X->pVTbl->Subtraction(pCMe->m_pICalculatorC_X, 20, 7);
            printf("Dilegate CalculatorC calls: 7+8=%d, 20-7=%d\n", (int)add_result, (int)sub_result);
        }
        
        if (pCMe->m_pICalculatorE_Y != 0) {
            int32_t mul_result = pCMe->m_pICalculatorE_Y->pVTbl->Multiplication(pCMe->m_pICalculatorE_Y, 6, 9);
            int16_t div_result = pCMe->m_pICalculatorE_Y->pVTbl->Division(pCMe->m_pICalculatorE_Y, 24, 4);
            printf("Dilegate CalculatorE calls: 6*9=%d, 24/4=%d\n", (int)mul_result, (int)div_result);
        }
    }

    /* ---------------- Connection Point tests ---------------- */
    printf("\n========== Демонстрация схем подключения событий ==========\n\n");
    
    /* Создание нескольких sink'ов */
    int16_t rSink1 = createCEcoLab1Sink(pIMem, &pIEvents1);
    int16_t rSink2 = createCEcoLab1Sink(pIMem, &pIEvents2);
    int16_t rSink3 = createCEcoLab1Sink(pIMem, &pIEvents3);
    
    if (rSink1 == 0 && pIEvents1 != 0) {
        pSink1 = (CEcoLab1Sink*)pIEvents1;
    }
    if (rSink2 == 0 && pIEvents2 != 0) {
        pSink2 = (CEcoLab1Sink*)pIEvents2;
    }
    if (rSink3 == 0 && pIEvents3 != 0) {
        pSink3 = (CEcoLab1Sink*)pIEvents3;
    }
    
    /* Создание дополнительных объектов CEcoLab1 */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoLab1, 0, &IID_IEcoLab1, (void**) &pIEcoLab1_2);
    if (result != 0 || pIEcoLab1_2 == 0) {
        pIEcoLab1_2 = 0;
    }
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoLab1, 0, &IID_IEcoLab1, (void**) &pIEcoLab1_3);
    if (result != 0 || pIEcoLab1_3 == 0) {
        pIEcoLab1_3 = 0;
    }
    
    /* ========== Схема 1: Несколько приемников - один объект (многие-к-одному) ========== */
    printf("--- Схема 1: Несколько приемников -> Один объект (многие-к-одному) ---\n");
    if (pSink1 != 0 && pSink1->Advise != 0) {
        pSink1->Advise(pSink1, pIEcoLab1);
        printf("Sink1 подключен к объекту 1\n");
    }
    if (pSink2 != 0 && pSink2->Advise != 0) {
        pSink2->Advise(pSink2, pIEcoLab1);
        printf("Sink2 подключен к объекту 1\n");
    }
    if (pSink3 != 0 && pSink3->Advise != 0) {
        pSink3->Advise(pSink3, pIEcoLab1);
        printf("Sink3 подключен к объекту 1\n");
    }
    printf("Все три sink'а подключены к одному объекту CEcoLab1\n\n");
    
    /* Тест сортировки с несколькими sink'ами */
    {
        if (pSink1 != 0) pSink1->m_iStep = 0;
        if (pSink2 != 0) pSink2->m_iStep = 0;
        if (pSink3 != 0) pSink3->m_iStep = 0;
        int test_arr[] = {3, 1, 4, 1, 5};
        uint32_t n_test = sizeof(test_arr)/sizeof(test_arr[0]);
        printf("Тест сортировки с несколькими sink'ами (объект 1): ");
        for (uint32_t i = 0; i < n_test; ++i) printf("%d ", test_arr[i]);
        printf("\n");
        pIEcoLab1->pVTbl->BucketSortInt(pIEcoLab1, test_arr, n_test);
        printf("Результат: ");
        for (uint32_t i = 0; i < n_test; ++i) printf("%d ", test_arr[i]);
        printf("\n\n");
    }
    
    /* Отключение sink'ов от объекта 1 */
    if (pSink1 != 0 && pSink1->Unadvise != 0) {
        pSink1->Unadvise(pSink1, pIEcoLab1);
    }
    if (pSink2 != 0 && pSink2->Unadvise != 0) {
        pSink2->Unadvise(pSink2, pIEcoLab1);
    }
    if (pSink3 != 0 && pSink3->Unadvise != 0) {
        pSink3->Unadvise(pSink3, pIEcoLab1);
    }
    
    /* ========== Схема 2: Один приемник - несколько объектов (один-ко-многим) ========== */
    printf("--- Схема 2: Один приемник -> Несколько объектов (один-ко-многим) ---\n");
    if (pIEcoLab1_2 != 0 && pIEcoLab1_3 != 0) {
        if (pSink1 != 0 && pSink1->Advise != 0) {
            pSink1->Advise(pSink1, pIEcoLab1);
            printf("Sink1 подключен к объекту 1\n");
        }
        if (pSink1 != 0 && pSink1->Advise != 0) {
            /* Для демонстрации один sink к нескольким объектам нужно создать отдельный sink */
            /* Но так как Advise принимает только один объект, создадим еще один sink для объекта 2 */
            if (pSink2 != 0 && pSink2->Advise != 0) {
                pSink2->Advise(pSink2, pIEcoLab1_2);
                printf("Sink2 подключен к объекту 2\n");
            }
            if (pSink3 != 0 && pSink3->Advise != 0) {
                pSink3->Advise(pSink3, pIEcoLab1_3);
                printf("Sink3 подключен к объекту 3\n");
            }
        }
        printf("Один sink подключен к нескольким объектам (через разные sink'и для демонстрации)\n\n");
        
        /* Тест сортировки на разных объектах */
        {
            if (pSink1 != 0) pSink1->m_iStep = 0;
            int test_arr1[] = {9, 2, 7};
            uint32_t n_test1 = sizeof(test_arr1)/sizeof(test_arr1[0]);
            printf("Тест сортировки на объекте 1: ");
            for (uint32_t i = 0; i < n_test1; ++i) printf("%d ", test_arr1[i]);
            printf("\n");
            pIEcoLab1->pVTbl->BucketSortInt(pIEcoLab1, test_arr1, n_test1);
            printf("Результат: ");
            for (uint32_t i = 0; i < n_test1; ++i) printf("%d ", test_arr1[i]);
            printf("\n");
        }
        
        {
            if (pSink2 != 0) pSink2->m_iStep = 0;
            int test_arr2[] = {5, 8, 1};
            uint32_t n_test2 = sizeof(test_arr2)/sizeof(test_arr2[0]);
            printf("Тест сортировки на объекте 2: ");
            for (uint32_t i = 0; i < n_test2; ++i) printf("%d ", test_arr2[i]);
            printf("\n");
            pIEcoLab1_2->pVTbl->BucketSortInt(pIEcoLab1_2, test_arr2, n_test2);
            printf("Результат: ");
            for (uint32_t i = 0; i < n_test2; ++i) printf("%d ", test_arr2[i]);
            printf("\n\n");
        }
        
        /* Отключение */
        if (pSink1 != 0 && pSink1->Unadvise != 0) {
            pSink1->Unadvise(pSink1, pIEcoLab1);
        }
        if (pSink2 != 0 && pSink2->Unadvise != 0) {
            pSink2->Unadvise(pSink2, pIEcoLab1_2);
        }
        if (pSink3 != 0 && pSink3->Unadvise != 0) {
            pSink3->Unadvise(pSink3, pIEcoLab1_3);
        }
    }
    
    /* ========== Схема 3: Несколько приемников - несколько объектов (многие-ко-многим) ========== */
    printf("--- Схема 3: Несколько приемников <-> Несколько объектов (многие-ко-многим) ---\n");
    if (pIEcoLab1_2 != 0 && pIEcoLab1_3 != 0) {
        /* Sink1 подключается к объектам 1 и 2 */
        if (pSink1 != 0 && pSink1->Advise != 0) {
            pSink1->Advise(pSink1, pIEcoLab1);
            printf("Sink1 подключен к объекту 1\n");
        }
        /* Sink2 подключается к объектам 2 и 3 */
        if (pSink2 != 0 && pSink2->Advise != 0) {
            pSink2->Advise(pSink2, pIEcoLab1_2);
            printf("Sink2 подключен к объекту 2\n");
        }
        /* Sink3 подключается к объектам 1 и 3 */
        if (pSink3 != 0 && pSink3->Advise != 0) {
            pSink3->Advise(pSink3, pIEcoLab1_3);
            printf("Sink3 подключен к объекту 3\n");
        }
        printf("Множественные подключения: Sink1->Obj1, Sink2->Obj2, Sink3->Obj3\n\n");
        
        /* Тест сортировки на всех объектах */
        {
            if (pSink1 != 0) pSink1->m_iStep = 0;
            int test_arr1[] = {6, 3, 9};
            uint32_t n_test1 = sizeof(test_arr1)/sizeof(test_arr1[0]);
            printf("Тест сортировки на объекте 1 (Sink1 слушает): ");
            for (uint32_t i = 0; i < n_test1; ++i) printf("%d ", test_arr1[i]);
            printf("\n");
            pIEcoLab1->pVTbl->BucketSortInt(pIEcoLab1, test_arr1, n_test1);
            printf("Результат: ");
            for (uint32_t i = 0; i < n_test1; ++i) printf("%d ", test_arr1[i]);
            printf("\n");
        }
        
        {
            if (pSink2 != 0) pSink2->m_iStep = 0;
            int test_arr2[] = {4, 7, 2};
            uint32_t n_test2 = sizeof(test_arr2)/sizeof(test_arr2[0]);
            printf("Тест сортировки на объекте 2 (Sink2 слушает): ");
            for (uint32_t i = 0; i < n_test2; ++i) printf("%d ", test_arr2[i]);
            printf("\n");
            pIEcoLab1_2->pVTbl->BucketSortInt(pIEcoLab1_2, test_arr2, n_test2);
            printf("Результат: ");
            for (uint32_t i = 0; i < n_test2; ++i) printf("%d ", test_arr2[i]);
            printf("\n");
        }
        
        {
            if (pSink3 != 0) pSink3->m_iStep = 0;
            int test_arr3[] = {8, 1, 5};
            uint32_t n_test3 = sizeof(test_arr3)/sizeof(test_arr3[0]);
            printf("Тест сортировки на объекте 3 (Sink3 слушает): ");
            for (uint32_t i = 0; i < n_test3; ++i) printf("%d ", test_arr3[i]);
            printf("\n");
            pIEcoLab1_3->pVTbl->BucketSortInt(pIEcoLab1_3, test_arr3, n_test3);
            printf("Результат: ");
            for (uint32_t i = 0; i < n_test3; ++i) printf("%d ", test_arr3[i]);
            printf("\n\n");
        }
        
        /* Отключение всех sink'ов */
        if (pSink1 != 0 && pSink1->Unadvise != 0) {
            pSink1->Unadvise(pSink1, pIEcoLab1);
        }
        if (pSink2 != 0 && pSink2->Unadvise != 0) {
            pSink2->Unadvise(pSink2, pIEcoLab1_2);
        }
        if (pSink3 != 0 && pSink3->Unadvise != 0) {
            pSink3->Unadvise(pSink3, pIEcoLab1_3);
        }
    }
    
    printf("========== Конец демонстрации схем подключения ==========\n\n");
    
    /* Подключение одного sink'а для основных тестов */
    if (pSink1 != 0 && pSink1->Advise != 0) {
        pSink1->Advise(pSink1, pIEcoLab1);
        printf("Events connected successfully for main tests\n");
    }

    /* ---------------- Bucket sort tests ---------------- */
    {
        if (pSink1 != 0) {
            pSink1->m_iStep = 0;
        }
        int a_i[] = {5, -1, 3, 3, 2, 9, 0, -4, 1};
        uint32_t n_i = sizeof(a_i)/sizeof(a_i[0]);
        printf("Before int sort: ");
        for (uint32_t i = 0; i < n_i; ++i) printf("%d ", a_i[i]);
        printf("\n");
        pIEcoLab1->pVTbl->BucketSortInt(pIEcoLab1, a_i, n_i);
        printf("After  int sort: ");
        for (uint32_t i = 0; i < n_i; ++i) printf("%d ", a_i[i]);
        printf("\n");
    }
    {
        if (pSink1 != 0) {
            pSink1->m_iStep = 0;
        }
        long a_l[] = {100000L, -2L, 42L, 0L, 7L, 7L, -100L};
        uint32_t n_l = sizeof(a_l)/sizeof(a_l[0]);
        printf("Before long sort: ");
        for (uint32_t i = 0; i < n_l; ++i) printf("%ld ", a_l[i]);
        printf("\n");
        pIEcoLab1->pVTbl->BucketSortLong(pIEcoLab1, a_l, n_l);
        printf("After  long sort: ");
        for (uint32_t i = 0; i < n_l; ++i) printf("%ld ", a_l[i]);
        printf("\n");
    }
    {
        if (pSink1 != 0) {
            pSink1->m_iStep = 0;
        }
        float a_f[] = {3.14f, -2.5f, 0.0f, 1.0f, 1.0f, 10.5f, -7.25f};
        uint32_t n_f = sizeof(a_f)/sizeof(a_f[0]);
        printf("Before float sort: ");
        for (uint32_t i = 0; i < n_f; ++i) printf("%g ", a_f[i]);
        printf("\n");
        pIEcoLab1->pVTbl->BucketSortFloat(pIEcoLab1, a_f, n_f);
        printf("After  float sort: ");
        for (uint32_t i = 0; i < n_f; ++i) printf("%g ", a_f[i]);
        printf("\n");
    }
    {
        if (pSink1 != 0) {
            pSink1->m_iStep = 0;
        }
        double a_d[] = {2.718, -3.0, 0.0, 123.456, 1.618, -0.001};
        uint32_t n_d = sizeof(a_d)/sizeof(a_d[0]);
        printf("Before double sort: ");
        for (uint32_t i = 0; i < n_d; ++i) printf("%g ", a_d[i]);
        printf("\n");
        pIEcoLab1->pVTbl->BucketSortDouble(pIEcoLab1, a_d, n_d);
        printf("After  double sort: ");
        for (uint32_t i = 0; i < n_d; ++i) printf("%g ", a_d[i]);
        printf("\n");
    }
    {
        if (pSink1 != 0) {
            pSink1->m_iStep = 0;
        }
        long double a_ld[] = { (long double)1.0, (long double)-1.0, (long double)0.0, (long double)3.0, (long double)-2.0 };
        uint32_t n_ld = sizeof(a_ld)/sizeof(a_ld[0]);
        printf("Before long double sort: ");
        for (uint32_t i = 0; i < n_ld; ++i) printf("%Lg ", a_ld[i]);
        printf("\n");
        pIEcoLab1->pVTbl->BucketSortLongDouble(pIEcoLab1, a_ld, n_ld);
        printf("After  long double sort: ");
        for (uint32_t i = 0; i < n_ld; ++i) printf("%Lg ", a_ld[i]);
        printf("\n");
    }


    /* ---------------- Отключение событий ---------------- */
    if (pSink1 != 0 && pIEvents1 != 0) {
        if (pSink1->Unadvise != 0) {
            pSink1->Unadvise(pSink1, pIEcoLab1);
        }
        pIEvents1->pVTbl->Release(pIEvents1);
        pIEvents1 = 0;
        pSink1 = 0;
    }
    if (pSink2 != 0 && pIEvents2 != 0) {
        if (pSink2->Unadvise != 0) {
            if (pIEcoLab1_2 != 0) {
                pSink2->Unadvise(pSink2, pIEcoLab1_2);
            }
        }
        pIEvents2->pVTbl->Release(pIEvents2);
        pIEvents2 = 0;
        pSink2 = 0;
    }
    if (pSink3 != 0 && pIEvents3 != 0) {
        if (pSink3->Unadvise != 0) {
            if (pIEcoLab1_3 != 0) {
                pSink3->Unadvise(pSink3, pIEcoLab1_3);
            }
        }
        pIEvents3->pVTbl->Release(pIEvents3);
        pIEvents3 = 0;
        pSink3 = 0;
    }
    
    /* Освобождение дополнительных объектов */
    if (pIEcoLab1_2 != 0) {
        pIEcoLab1_2->pVTbl->Release(pIEcoLab1_2);
        pIEcoLab1_2 = 0;
    }
    if (pIEcoLab1_3 != 0) {
        pIEcoLab1_3->pVTbl->Release(pIEcoLab1_3);
        pIEcoLab1_3 = 0;
    }

    /* Освлбождение блока памяти */
    pIMem->pVTbl->Free(pIMem, name);

Release:

    /* Освобождение интерфейса для работы с интерфейсной шиной */
    if (pIBus != 0) {
        pIBus->pVTbl->Release(pIBus);
    }

    /* Освобождение интерфейса работы с памятью */
    if (pIMem != 0) {
        pIMem->pVTbl->Release(pIMem);
    }

    /* Освобождение тестируемого интерфейса */
    if (pIEcoLab1 != 0) {
        pIEcoLab1->pVTbl->Release(pIEcoLab1);
    }

    /* Освобождение системного интерфейса */
    if (pISys != 0) {
        pISys->pVTbl->Release(pISys);
    }

    return result;
}


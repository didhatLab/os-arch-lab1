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

    /* ---------------- Calculator methods via delegation (CalculatorC + CalculatorE) ---------------- */
    {
        int32_t add_result = pIEcoLab1->pVTbl->Addition(pIEcoLab1, 7, 8);
        int16_t sub_result = pIEcoLab1->pVTbl->Subtraction(pIEcoLab1, 20, 7);
        int32_t mul_result = pIEcoLab1->pVTbl->Multiplication(pIEcoLab1, 6, 9);
        int16_t div_result = pIEcoLab1->pVTbl->Division(pIEcoLab1, 24, 4);
        printf("Delegated CalculatorC+E: 7+8=%d, 20-7=%d, 6*9=%d, 24/4=%d\n", 
               (int)add_result, (int)sub_result, (int)mul_result, (int)div_result);
    }

    /* ---------------- Bucket sort tests ---------------- */
    {
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


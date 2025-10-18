/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   CEcoLab1
 * </сводка>
 *
 * <описание>
 *   Данный заголовок описывает реализацию компонента CEcoLab1
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#ifndef __C_ECOLAB1_H__
#define __C_ECOLAB1_H__

#include "IEcoLab1.h"
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"
/* Aggregated calculator interface */
#include "../Eco.CalculatorB/SharedFiles/IEcoCalculatorX.h"
/* Aggregated calculator Y interface */
#include "../Eco.CalculatorD/SharedFiles/IEcoCalculatorY.h"
/* CalculatorC interfaces for delegation */
#include "../Eco.CalculatorC/SharedFiles/IEcoCalculatorX.h"
#include "../Eco.CalculatorC/SharedFiles/IEcoCalculatorY.h"
/* CalculatorE interfaces for delegation */
#include "../Eco.CalculatorE/SharedFiles/IEcoCalculatorY.h"

typedef struct CEcoLab1 {

    /* Таблица функций интерфейса IEcoLab1 */
    IEcoLab1VTbl* m_pVTblIEcoLab1;


    /* Счетчик ссылок */
    uint32_t m_cRef;

    /* Интерфейс для работы с памятью */
    IEcoMemoryAllocator1* m_pIMem;

    /* Системный интерфейс */
    IEcoSystem1* m_pISys;

    /* Данные экземпляра */
    char_t* m_Name;

    /* Агрегированный интерфейс калькулятора (сложение/вычитание) */
    IEcoCalculatorX* m_pICalculatorX;
    /* Агрегированный интерфейс калькулятора (умножение/деление) */
    IEcoCalculatorY* m_pICalculatorY;
    /* CalculatorC для делегирования методов */
    IEcoCalculatorX* m_pICalculatorC_X;
    IEcoCalculatorY* m_pICalculatorC_Y;
    /* CalculatorE для делегирования методов умножения и деления */
    IEcoCalculatorY* m_pICalculatorE_Y;

} CEcoLab1, *CEcoLab1Ptr;

/* Инициализация экземпляра */
int16_t ECOCALLMETHOD initCEcoLab1(/*in*/ struct IEcoLab1* me, /* in */ IEcoUnknown *pIUnkSystem);
/* Создание экземпляра */
int16_t ECOCALLMETHOD createCEcoLab1(/* in */ IEcoUnknown* pIUnkSystem, /* in */ IEcoUnknown* pIUnkOuter, /* out */ IEcoLab1** ppIEcoLab1);
/* Удаление */
void ECOCALLMETHOD deleteCEcoLab1(/* in */ IEcoLab1* pIEcoLab1);

#endif /* __C_ECOLAB1_H__ */

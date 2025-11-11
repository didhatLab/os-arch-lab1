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
 *   Данный исходный код описывает реализацию интерфейсов CEcoLab1
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#include "IEcoSystem1.h"
#include "IEcoInterfaceBus1.h"
#include "IEcoInterfaceBus1MemExt.h"
#include "CEcoLab1.h"
#include "CEcoLab1EnumConnectionPoints.h"
#include "IEcoConnectionPointContainer.h"
#include "../Eco.CalculatorB/SharedFiles/IdEcoCalculatorB.h"
/* Include CID for CalculatorD (Y interface) */
#include "../Eco.CalculatorD/SharedFiles/IdEcoCalculatorD.h"
/* Include CID for CalculatorC (delegation) */
#include "../Eco.CalculatorC/SharedFiles/IdEcoCalculatorC.h"
/* Include CID for CalculatorE (delegation) */
#include "../Eco.CalculatorE/SharedFiles/IdEcoCalculatorE.h"
/* lazy load via interface bus; no direct create symbol needed */
#include <float.h>
#include <math.h>

/*
 *
 * <сводка>
 *   Функция QueryInterface
 * </сводка>
 *
 * <описание>
 *   Функция QueryInterface для интерфейса IEcoLab1
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoLab1_QueryInterface(/* in */ IEcoLab1Ptr_t me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoLab1* pCMe = (CEcoLab1*)me;

    /* Проверка указателей */
    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    /* Проверка и получение запрошенного интерфейса */
    if ( IsEqualUGUID(riid, &IID_IEcoLab1) ) {
        *ppv = &pCMe->m_pVTblIEcoLab1;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoConnectionPointContainer) ) {
        *ppv = &pCMe->m_pVTblICPC;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblIEcoLab1;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoCalculatorX) ) {
        if (pCMe->m_pICalculatorX == 0) {
            IEcoInterfaceBus1* pIBusQI = 0;
            if (pCMe->m_pISys != 0 && pCMe->m_pISys->pVTbl->QueryInterface(pCMe->m_pISys, &IID_IEcoInterfaceBus1, (void**)&pIBusQI) == 0 && pIBusQI != 0) {
                IEcoCalculatorX* pIX = 0;
                if (pIBusQI->pVTbl->QueryComponent(pIBusQI, &CID_EcoCalculatorB, 0, &IID_IEcoCalculatorX, (void**)&pIX) == 0 && pIX != 0) {
                    pCMe->m_pICalculatorX = pIX;
                }
                pIBusQI->pVTbl->Release(pIBusQI);
            }
        }
        if (pCMe->m_pICalculatorX != 0) {
            return pCMe->m_pICalculatorX->pVTbl->QueryInterface(pCMe->m_pICalculatorX, riid, ppv);
        }
        *ppv = 0;
        return ERR_ECO_NOINTERFACE;
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoCalculatorY) ) {
        /* Простой подход: делегируем прямо к CalculatorD через интерфейсную шину */
        IEcoSystem1* pISys = 0;
        IEcoInterfaceBus1* pIBusQI = 0;
        IEcoCalculatorY* pIY = 0;
        int16_t result = ERR_ECO_NOINTERFACE;
        
        if (pCMe->m_pISys != 0) {
            pISys = pCMe->m_pISys;
            pISys->pVTbl->AddRef(pISys);
            
            if (pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void**)&pIBusQI) == 0 && pIBusQI != 0) {
                if (pIBusQI->pVTbl->QueryComponent(pIBusQI, &CID_EcoCalculatorD, 0, &IID_IEcoCalculatorY, (void**)&pIY) == 0 && pIY != 0) {
                    /* Сохраняем ссылку для будущего использования */
                    if (pCMe->m_pICalculatorY == 0) {
                        pCMe->m_pICalculatorY = pIY;
                        pIY->pVTbl->AddRef(pIY);
                    }
                    result = pIY->pVTbl->QueryInterface(pIY, riid, ppv);
                }
                pIBusQI->pVTbl->Release(pIBusQI);
            }
            pISys->pVTbl->Release(pISys);
        }
        
        if (result != 0) {
            *ppv = 0;
        }
        return result;
    }
    else {
        *ppv = 0;
        return ERR_ECO_NOINTERFACE;
    }
    return ERR_ECO_SUCCESES;
}

/*
 *
 * <сводка>
 *   Функция AddRef
 * </сводка>
 *
 * <описание>
 *   Функция AddRef для интерфейса IEcoLab1
 * </описание>
 *
 */
static uint32_t ECOCALLMETHOD CEcoLab1_AddRef(/* in */ IEcoLab1Ptr_t me) {
    CEcoLab1* pCMe = (CEcoLab1*)me;

    /* Проверка указателя */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    return ++pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция Release
 * </сводка>
 *
 * <описание>
 *   Функция Release для интерфейса IEcoLab1
 * </описание>
 *
 */
static uint32_t ECOCALLMETHOD CEcoLab1_Release(/* in */ IEcoLab1Ptr_t me) {
    CEcoLab1* pCMe = (CEcoLab1*)me;

    /* Проверка указателя */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    /* Уменьшение счетчика ссылок на компонент */
    --pCMe->m_cRef;

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if ( pCMe->m_cRef == 0 ) {
        deleteCEcoLab1((IEcoLab1*)pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция MyFunction
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoLab1_MyFunction(/* in */ IEcoLab1Ptr_t me, /* in */ char_t* Name, /* out */ char_t** copyName) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    int16_t index = 0;

    /* Проверка указателей */
    if (me == 0 || Name == 0 || copyName == 0) {
        return ERR_ECO_POINTER;
    }

    /* Копирование строки */
    while(Name[index] != 0) {
        index++;
    }
    pCMe->m_Name = (char_t*)pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, index + 1);
    index = 0;
    while(Name[index] != 0) {
        pCMe->m_Name[index] = Name[index];
        index++;
    }
    *copyName = pCMe->m_Name;

    return ERR_ECO_SUCCESES;
}

/*
 *
 * <сводка>
 *   Функция Fire_OnSortSwap
 * </сводка>
 *
 * <описание>
 *   Функция вызова события OnSortSwap
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoLab1_Fire_OnSortSwap(/* in */ IEcoLab1Ptr_t me, /* in */ int16_t indexSrc, /* in */ int16_t indexDst) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    int16_t result = 0;
    IEcoEnumConnections* pEnum = 0;
    IEcoLab1Events* pIEvents = 0;
    EcoConnectionData cd;

    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    if (pCMe->m_pISinkCP != 0) {
        struct IEcoConnectionPoint* pICP = (struct IEcoConnectionPoint*)pCMe->m_pISinkCP;
        result = pICP->pVTbl->EnumConnections(pICP, (struct IEcoEnumConnections**)&pEnum);
        if ((result == 0) && (pEnum != 0)) {
            while (pEnum->pVTbl->Next((struct IEcoEnumConnections*)pEnum, 1, &cd, 0) == 0) {
                result = cd.pUnk->pVTbl->QueryInterface(cd.pUnk, &IID_IEcoLab1Events, (void**)&pIEvents);
                if ((result == 0) && (pIEvents != 0)) {
                    if (pIEvents->pVTbl->OnSortSwap != 0) {
                        result = pIEvents->pVTbl->OnSortSwap((struct IEcoLab1Events*)pIEvents, indexSrc, indexDst);
                    }
                    pIEvents->pVTbl->Release((struct IEcoLab1Events*)pIEvents);
                }
                cd.pUnk->pVTbl->Release(cd.pUnk);
            }
            pEnum->pVTbl->Release((struct IEcoEnumConnections*)pEnum);
        }
    }
    return result;
}

/*
 *
 * <сводка>
 *   Функция Fire_OnBucketSortStart
 * </сводка>
 *
 * <описание>
 *   Функция вызова события OnBucketSortStart
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoLab1_Fire_OnBucketSortStart(/* in */ IEcoLab1Ptr_t me, /* in */ uint32_t arrayLength) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    int16_t result = 0;
    IEcoEnumConnections* pEnum = 0;
    IEcoLab1Events* pIEvents = 0;
    EcoConnectionData cd;

    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    if (pCMe->m_pISinkCP != 0) {
        struct IEcoConnectionPoint* pICP = (struct IEcoConnectionPoint*)pCMe->m_pISinkCP;
        result = pICP->pVTbl->EnumConnections(pICP, (struct IEcoEnumConnections**)&pEnum);
        if ((result == 0) && (pEnum != 0)) {
            while (pEnum->pVTbl->Next((struct IEcoEnumConnections*)pEnum, 1, &cd, 0) == 0) {
                result = cd.pUnk->pVTbl->QueryInterface(cd.pUnk, &IID_IEcoLab1Events, (void**)&pIEvents);
                if ((result == 0) && (pIEvents != 0)) {
                    if (pIEvents->pVTbl->OnBucketSortStart != 0) {
                        result = pIEvents->pVTbl->OnBucketSortStart((struct IEcoLab1Events*)pIEvents, arrayLength);
                    }
                    pIEvents->pVTbl->Release((struct IEcoLab1Events*)pIEvents);
                }
                cd.pUnk->pVTbl->Release(cd.pUnk);
            }
            pEnum->pVTbl->Release((struct IEcoEnumConnections*)pEnum);
        }
    }
    return result;
}

/*
 *
 * <сводка>
 *   Функция Fire_OnMinMaxFound
 * </сводка>
 *
 * <описание>
 *   Функция вызова события OnMinMaxFound
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoLab1_Fire_OnMinMaxFound(/* in */ IEcoLab1Ptr_t me, /* in */ double minValue, /* in */ double maxValue) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    int16_t result = 0;
    IEcoEnumConnections* pEnum = 0;
    IEcoLab1Events* pIEvents = 0;
    EcoConnectionData cd;

    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    if (pCMe->m_pISinkCP != 0) {
        struct IEcoConnectionPoint* pICP = (struct IEcoConnectionPoint*)pCMe->m_pISinkCP;
        result = pICP->pVTbl->EnumConnections(pICP, (struct IEcoEnumConnections**)&pEnum);
        if ((result == 0) && (pEnum != 0)) {
            while (pEnum->pVTbl->Next((struct IEcoEnumConnections*)pEnum, 1, &cd, 0) == 0) {
                result = cd.pUnk->pVTbl->QueryInterface(cd.pUnk, &IID_IEcoLab1Events, (void**)&pIEvents);
                if ((result == 0) && (pIEvents != 0)) {
                    if (pIEvents->pVTbl->OnMinMaxFound != 0) {
                        result = pIEvents->pVTbl->OnMinMaxFound((struct IEcoLab1Events*)pIEvents, minValue, maxValue);
                    }
                    pIEvents->pVTbl->Release((struct IEcoLab1Events*)pIEvents);
                }
                cd.pUnk->pVTbl->Release(cd.pUnk);
            }
            pEnum->pVTbl->Release((struct IEcoEnumConnections*)pEnum);
        }
    }
    return result;
}

/*
 *
 * <сводка>
 *   Функция Fire_OnBucketsCreated
 * </сводка>
 *
 * <описание>
 *   Функция вызова события OnBucketsCreated
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoLab1_Fire_OnBucketsCreated(/* in */ IEcoLab1Ptr_t me, /* in */ uint32_t bucketCount) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    int16_t result = 0;
    IEcoEnumConnections* pEnum = 0;
    IEcoLab1Events* pIEvents = 0;
    EcoConnectionData cd;

    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    if (pCMe->m_pISinkCP != 0) {
        struct IEcoConnectionPoint* pICP = (struct IEcoConnectionPoint*)pCMe->m_pISinkCP;
        result = pICP->pVTbl->EnumConnections(pICP, (struct IEcoEnumConnections**)&pEnum);
        if ((result == 0) && (pEnum != 0)) {
            while (pEnum->pVTbl->Next((struct IEcoEnumConnections*)pEnum, 1, &cd, 0) == 0) {
                result = cd.pUnk->pVTbl->QueryInterface(cd.pUnk, &IID_IEcoLab1Events, (void**)&pIEvents);
                if ((result == 0) && (pIEvents != 0)) {
                    if (pIEvents->pVTbl->OnBucketsCreated != 0) {
                        result = pIEvents->pVTbl->OnBucketsCreated((struct IEcoLab1Events*)pIEvents, bucketCount);
                    }
                    pIEvents->pVTbl->Release((struct IEcoLab1Events*)pIEvents);
                }
                cd.pUnk->pVTbl->Release(cd.pUnk);
            }
            pEnum->pVTbl->Release((struct IEcoEnumConnections*)pEnum);
        }
    }
    return result;
}

/*
 *
 * <сводка>
 *   Функция Fire_OnElementDistributed
 * </сводка>
 *
 * <описание>
 *   Функция вызова события OnElementDistributed
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoLab1_Fire_OnElementDistributed(/* in */ IEcoLab1Ptr_t me, /* in */ uint32_t elementIndex, /* in */ uint32_t bucketIndex) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    int16_t result = 0;
    IEcoEnumConnections* pEnum = 0;
    IEcoLab1Events* pIEvents = 0;
    EcoConnectionData cd;

    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    if (pCMe->m_pISinkCP != 0) {
        struct IEcoConnectionPoint* pICP = (struct IEcoConnectionPoint*)pCMe->m_pISinkCP;
        result = pICP->pVTbl->EnumConnections(pICP, (struct IEcoEnumConnections**)&pEnum);
        if ((result == 0) && (pEnum != 0)) {
            while (pEnum->pVTbl->Next((struct IEcoEnumConnections*)pEnum, 1, &cd, 0) == 0) {
                result = cd.pUnk->pVTbl->QueryInterface(cd.pUnk, &IID_IEcoLab1Events, (void**)&pIEvents);
                if ((result == 0) && (pIEvents != 0)) {
                    if (pIEvents->pVTbl->OnElementDistributed != 0) {
                        result = pIEvents->pVTbl->OnElementDistributed((struct IEcoLab1Events*)pIEvents, elementIndex, bucketIndex);
                    }
                    pIEvents->pVTbl->Release((struct IEcoLab1Events*)pIEvents);
                }
                cd.pUnk->pVTbl->Release(cd.pUnk);
            }
            pEnum->pVTbl->Release((struct IEcoEnumConnections*)pEnum);
        }
    }
    return result;
}

/*
 *
 * <сводка>
 *   Функция Fire_OnBucketSortingStarted
 * </сводка>
 *
 * <описание>
 *   Функция вызова события OnBucketSortingStarted
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoLab1_Fire_OnBucketSortingStarted(/* in */ IEcoLab1Ptr_t me, /* in */ uint32_t bucketIndex, /* in */ uint32_t bucketSize) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    int16_t result = 0;
    IEcoEnumConnections* pEnum = 0;
    IEcoLab1Events* pIEvents = 0;
    EcoConnectionData cd;

    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    if (pCMe->m_pISinkCP != 0) {
        struct IEcoConnectionPoint* pICP = (struct IEcoConnectionPoint*)pCMe->m_pISinkCP;
        result = pICP->pVTbl->EnumConnections(pICP, (struct IEcoEnumConnections**)&pEnum);
        if ((result == 0) && (pEnum != 0)) {
            while (pEnum->pVTbl->Next((struct IEcoEnumConnections*)pEnum, 1, &cd, 0) == 0) {
                result = cd.pUnk->pVTbl->QueryInterface(cd.pUnk, &IID_IEcoLab1Events, (void**)&pIEvents);
                if ((result == 0) && (pIEvents != 0)) {
                    if (pIEvents->pVTbl->OnBucketSortingStarted != 0) {
                        result = pIEvents->pVTbl->OnBucketSortingStarted((struct IEcoLab1Events*)pIEvents, bucketIndex, bucketSize);
                    }
                    pIEvents->pVTbl->Release((struct IEcoLab1Events*)pIEvents);
                }
                cd.pUnk->pVTbl->Release(cd.pUnk);
            }
            pEnum->pVTbl->Release((struct IEcoEnumConnections*)pEnum);
        }
    }
    return result;
}

/*
 *
 * <сводка>
 *   Функция Fire_OnBucketSorted
 * </сводка>
 *
 * <описание>
 *   Функция вызова события OnBucketSorted
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoLab1_Fire_OnBucketSorted(/* in */ IEcoLab1Ptr_t me, /* in */ uint32_t bucketIndex) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    int16_t result = 0;
    IEcoEnumConnections* pEnum = 0;
    IEcoLab1Events* pIEvents = 0;
    EcoConnectionData cd;

    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    if (pCMe->m_pISinkCP != 0) {
        struct IEcoConnectionPoint* pICP = (struct IEcoConnectionPoint*)pCMe->m_pISinkCP;
        result = pICP->pVTbl->EnumConnections(pICP, (struct IEcoEnumConnections**)&pEnum);
        if ((result == 0) && (pEnum != 0)) {
            while (pEnum->pVTbl->Next((struct IEcoEnumConnections*)pEnum, 1, &cd, 0) == 0) {
                result = cd.pUnk->pVTbl->QueryInterface(cd.pUnk, &IID_IEcoLab1Events, (void**)&pIEvents);
                if ((result == 0) && (pIEvents != 0)) {
                    if (pIEvents->pVTbl->OnBucketSorted != 0) {
                        result = pIEvents->pVTbl->OnBucketSorted((struct IEcoLab1Events*)pIEvents, bucketIndex);
                    }
                    pIEvents->pVTbl->Release((struct IEcoLab1Events*)pIEvents);
                }
                cd.pUnk->pVTbl->Release(cd.pUnk);
            }
            pEnum->pVTbl->Release((struct IEcoEnumConnections*)pEnum);
        }
    }
    return result;
}

/*
 *
 * <сводка>
 *   Функция Fire_OnBucketSortCompleted
 * </сводка>
 *
 * <описание>
 *   Функция вызова события OnBucketSortCompleted
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoLab1_Fire_OnBucketSortCompleted(/* in */ IEcoLab1Ptr_t me) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    int16_t result = 0;
    IEcoEnumConnections* pEnum = 0;
    IEcoLab1Events* pIEvents = 0;
    EcoConnectionData cd;

    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    if (pCMe->m_pISinkCP != 0) {
        struct IEcoConnectionPoint* pICP = (struct IEcoConnectionPoint*)pCMe->m_pISinkCP;
        result = pICP->pVTbl->EnumConnections(pICP, (struct IEcoEnumConnections**)&pEnum);
        if ((result == 0) && (pEnum != 0)) {
            while (pEnum->pVTbl->Next((struct IEcoEnumConnections*)pEnum, 1, &cd, 0) == 0) {
                result = cd.pUnk->pVTbl->QueryInterface(cd.pUnk, &IID_IEcoLab1Events, (void**)&pIEvents);
                if ((result == 0) && (pIEvents != 0)) {
                    if (pIEvents->pVTbl->OnBucketSortCompleted != 0) {
                        result = pIEvents->pVTbl->OnBucketSortCompleted((struct IEcoLab1Events*)pIEvents);
                    }
                    pIEvents->pVTbl->Release((struct IEcoLab1Events*)pIEvents);
                }
                cd.pUnk->pVTbl->Release(cd.pUnk);
            }
            pEnum->pVTbl->Release((struct IEcoEnumConnections*)pEnum);
        }
    }
    return result;
}

/* ========================= Bucket Sort Helpers ========================= */
static uint32_t CEcoLab1_clampBucketIndex(double normalized, uint32_t bucketCount) {
    uint32_t index = (uint32_t)(normalized * (double)bucketCount);
    if (index >= bucketCount) {
        index = bucketCount - 1;
    }
    return index;
}

#define DEFINE_BUCKET_SORT_FOR_TYPE(TYPE, FUNC_NAME) \
static void CEcoLab1_insertionSort_##FUNC_NAME(IEcoLab1Ptr_t me, TYPE* array, uint32_t start, uint32_t endExclusive, uint32_t bucketIndex) { \
    for (uint32_t i = start + 1; i < endExclusive; ++i) { \
        TYPE key = array[i]; \
        int64_t j = (int64_t)i - 1; \
        while (j >= (int64_t)start && array[j] > key) { \
            array[j + 1] = array[j]; \
            CEcoLab1_Fire_OnSortSwap(me, (int16_t)(j + 1), (int16_t)j); \
            --j; \
        } \
        if (j + 1 != (int64_t)i) { \
            array[j + 1] = key; \
            CEcoLab1_Fire_OnSortSwap(me, (int16_t)i, (int16_t)(j + 1)); \
        } \
    } \
} \
static int16_t ECOCALLMETHOD CEcoLab1_##FUNC_NAME(IEcoLab1Ptr_t me, TYPE* array, uint32_t length) { \
    CEcoLab1* pCMe = (CEcoLab1*)me; \
    if (me == 0 || array == 0) { \
        return ERR_ECO_POINTER; \
    } \
    if (length <= 1) { \
        return ERR_ECO_SUCCESES; \
    } \
    /* Fire OnBucketSortStart - начало сортировки */ \
    CEcoLab1_Fire_OnBucketSortStart(me, length); \
    TYPE minValue = array[0]; \
    TYPE maxValue = array[0]; \
    for (uint32_t i = 1; i < length; ++i) { \
        if (array[i] < minValue) minValue = array[i]; \
        if (array[i] > maxValue) maxValue = array[i]; \
    } \
    /* Fire OnMinMaxFound - найдены минимальное и максимальное значения */ \
    CEcoLab1_Fire_OnMinMaxFound(me, (double)minValue, (double)maxValue); \
    double range = (double)maxValue - (double)minValue; \
    if (range == 0.0) { \
        return ERR_ECO_SUCCESES; \
    } \
    uint32_t bucketCount = length; \
    /* Fire OnBucketsCreated - созданы корзины */ \
    CEcoLab1_Fire_OnBucketsCreated(me, bucketCount); \
    IEcoMemoryAllocator1* pIMem = pCMe->m_pIMem; \
    uint32_t* bucketSizes = (uint32_t*)pIMem->pVTbl->Alloc(pIMem, bucketCount * sizeof(uint32_t)); \
    uint32_t* bucketOffsets = (uint32_t*)pIMem->pVTbl->Alloc(pIMem, bucketCount * sizeof(uint32_t)); \
    TYPE* buffer = (TYPE*)pIMem->pVTbl->Alloc(pIMem, length * sizeof(TYPE)); \
    if (bucketSizes == 0 || bucketOffsets == 0 || buffer == 0) { \
        if (bucketSizes) pIMem->pVTbl->Free(pIMem, bucketSizes); \
        if (bucketOffsets) pIMem->pVTbl->Free(pIMem, bucketOffsets); \
        if (buffer) pIMem->pVTbl->Free(pIMem, buffer); \
        return ERR_ECO_NOSYSTEM; \
    } \
    for (uint32_t i = 0; i < bucketCount; ++i) bucketSizes[i] = 0; \
    /* Подсчет размеров корзин */ \
    for (uint32_t i = 0; i < length; ++i) { \
        double normalized = ((double)array[i] - (double)minValue) / range; \
        uint32_t index = CEcoLab1_clampBucketIndex(normalized, bucketCount); \
        bucketSizes[index]++; \
    } \
    uint32_t sum = 0; \
    for (uint32_t i = 0; i < bucketCount; ++i) { \
        bucketOffsets[i] = sum; \
        sum += bucketSizes[i]; \
    } \
    /* Place elements into buffer by bucket */ \
    for (uint32_t i = 0; i < length; ++i) { \
        double normalized = ((double)array[i] - (double)minValue) / range; \
        uint32_t index = CEcoLab1_clampBucketIndex(normalized, bucketCount); \
        buffer[bucketOffsets[index]++] = array[i]; \
        /* Fire OnElementDistributed - элемент распределен в корзину */ \
        CEcoLab1_Fire_OnElementDistributed(me, i, index); \
    } \
    /* Restore offsets to starts to sort each bucket in place in buffer */ \
    sum = 0; \
    for (uint32_t i = 0; i < bucketCount; ++i) { \
        uint32_t start = sum; \
        uint32_t end = sum + bucketSizes[i]; \
        if (end > start + 1) { \
            /* Fire OnBucketSortingStarted - начало сортировки корзины */ \
            CEcoLab1_Fire_OnBucketSortingStarted(me, i, bucketSizes[i]); \
            CEcoLab1_insertionSort_##FUNC_NAME(me, buffer, start, end, i); \
            /* Fire OnBucketSorted - корзина отсортирована */ \
            CEcoLab1_Fire_OnBucketSorted(me, i); \
        } \
        sum = end; \
    } \
    /* Copy back */ \
    for (uint32_t i = 0; i < length; ++i) { \
        array[i] = buffer[i]; \
    } \
    /* Fire OnBucketSortCompleted - завершение сортировки */ \
    CEcoLab1_Fire_OnBucketSortCompleted(me); \
    pIMem->pVTbl->Free(pIMem, bucketSizes); \
    pIMem->pVTbl->Free(pIMem, bucketOffsets); \
    pIMem->pVTbl->Free(pIMem, buffer); \
    return ERR_ECO_SUCCESES; \
}

/* Generate implementations */
DEFINE_BUCKET_SORT_FOR_TYPE(int, BucketSortInt)
DEFINE_BUCKET_SORT_FOR_TYPE(long, BucketSortLong)
DEFINE_BUCKET_SORT_FOR_TYPE(float, BucketSortFloat)
DEFINE_BUCKET_SORT_FOR_TYPE(double, BucketSortDouble)
DEFINE_BUCKET_SORT_FOR_TYPE(long double, BucketSortLongDouble)

/*
 *
 * <сводка>
 *   Функция QueryInterface
 * </сводка>
 *
 * <описание>
 *   Функция QueryInterface для интерфейса IEcoConnectionPointContainer
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1_IEcoConnectionPointContainer_QueryInterface(/* in */ struct IEcoConnectionPointContainer* me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint64_t)me - sizeof(struct IEcoUnknown*));

    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    /* Проверка и получение запрошенного интерфейса */
    if ( IsEqualUGUID(riid, &IID_IEcoLab1) ) {
        *ppv = &pCMe->m_pVTblIEcoLab1;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoConnectionPointContainer) ) {
        *ppv = &pCMe->m_pVTblICPC;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblIEcoLab1;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else {
        *ppv = 0;
        return ERR_ECO_NOINTERFACE;
    }

    return ERR_ECO_SUCCESES;
}

/*
 *
 * <сводка>
 *   Функция AddRef
 * </сводка>
 *
 * <описание>
 *   Функция AddRef для интерфейса IEcoConnectionPointContainer
 * </описание>
 *
 */
uint32_t ECOCALLMETHOD CEcoLab1_IEcoConnectionPointContainer_AddRef(/* in */ struct IEcoConnectionPointContainer* me) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint64_t)me - sizeof(struct IEcoUnknown*));

    if (me == 0 ) {
        return (uint32_t)-1;
    }

    return ++pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция Release
 * </сводка>
 *
 * <описание>
 *   Функция Release для интерфейса IEcoConnectionPointContainer
 * </описание>
 *
 */
uint32_t ECOCALLMETHOD CEcoLab1_IEcoConnectionPointContainer_Release(/* in */ struct IEcoConnectionPointContainer* me) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint64_t)me - sizeof(struct IEcoUnknown*));

    if (me == 0 ) {
        return (uint32_t)-1;
    }

    /* Уменьшение счетчика ссылок на компонент */
    --pCMe->m_cRef;

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if ( pCMe->m_cRef == 0 ) {
        deleteCEcoLab1((IEcoLab1*)pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция EnumConnectionPoints
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1_IEcoConnectionPointContainer_EnumConnectionPoints(/* in */ struct IEcoConnectionPointContainer* me, /* out */ struct IEcoEnumConnectionPoints **ppEnum) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint64_t)me - sizeof(struct IEcoUnknown*));
    int16_t result = ERR_ECO_POINTER;

    if (me == 0 || ppEnum == 0 ) {
        return result;
    }

    if (pCMe->m_pISinkCP != 0) {
        result = createCEcoLab1EnumConnectionPoints((IEcoUnknown*)pCMe->m_pISys, (struct IEcoConnectionPoint*)&pCMe->m_pISinkCP->m_pVTblICP, ppEnum);
    }

    return result;
}

/*
 *
 * <сводка>
 *   Функция FindConnectionPoint
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1_IEcoConnectionPointContainer_FindConnectionPoint(/* in */ struct IEcoConnectionPointContainer* me, /* in */ const UGUID* riid, /* out */ struct IEcoConnectionPoint **ppCP) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint64_t)me - sizeof(struct IEcoUnknown*));
    int16_t result = ERR_ECO_POINTER;

    if (me == 0 || ppCP == 0 ) {
        return result;
    }

    if ( !IsEqualUGUID(riid, &IID_IEcoLab1Events ) ) {
        *ppCP = 0;
        /* CONNECT_E_NOCONNECTION */
        return ERR_ECO_NOINTERFACE;
    }

    if (pCMe->m_pISinkCP == 0) {
        /* E_FAIL */
        return ERR_ECO_NOSYSTEM;
    }

    struct IEcoConnectionPoint* pICP = (struct IEcoConnectionPoint*)pCMe->m_pISinkCP;
    pICP->pVTbl->AddRef(pICP);
    *ppCP = (struct IEcoConnectionPoint*)pCMe->m_pISinkCP;

    return ERR_ECO_SUCCESES;
}

/*
 *
 * <сводка>
 *   Функция Init
 * </сводка>
 *
 * <описание>
 *   Функция инициализации экземпляра
 * </описание>
 *
 */
int16_t ECOCALLMETHOD initCEcoLab1(/*in*/ IEcoLab1Ptr_t me, /* in */ struct IEcoUnknown *pIUnkSystem) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    IEcoInterfaceBus1* pIBus = 0;
    int16_t result = -1;

    /* Проверка указателей */
    if (me == 0 ) {
        return result;
    }

    /* Сохранение указателя на системный интерфейс */
    pCMe->m_pISys = (IEcoSystem1*)pIUnkSystem;

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pCMe->m_pISys->pVTbl->QueryInterface(pCMe->m_pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);

    /* Проверка указателей */
    if (me == 0 ) {
        return result;
    }

    /* Создадим/подключим калькулятор B (IEcoCalculatorX) как составной компонент (без агрегирования) */
    if (result == 0 && pIBus != 0) {
        IEcoCalculatorX* pIX = 0;
        int16_t r2 = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorB, 0, &IID_IEcoCalculatorX, (void**)&pIX);
        if (r2 == 0 && pIX != 0) {
            pCMe->m_pICalculatorX = pIX;
        }
        /* Подключим калькулятор D (IEcoCalculatorY) как составной компонент (без агрегирования) */
        IEcoCalculatorY* pIY = 0;
        r2 = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorD, 0, &IID_IEcoCalculatorY, (void**)&pIY);
        if (r2 == 0 && pIY != 0) {
            pCMe->m_pICalculatorY = pIY;
        }
        
        /* Подключим калькулятор C для делегирования методов */
        IEcoCalculatorX* pIC_X = 0;
        r2 = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorC, 0, &IID_IEcoCalculatorX, (void**)&pIC_X);
        if (r2 == 0 && pIC_X != 0) {
            pCMe->m_pICalculatorC_X = pIC_X;
        }
        
        IEcoCalculatorY* pIC_Y = 0;
        r2 = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorC, 0, &IID_IEcoCalculatorY, (void**)&pIC_Y);
        if (r2 == 0 && pIC_Y != 0) {
            pCMe->m_pICalculatorC_Y = pIC_Y;
        }
        
        /* Подключим калькулятор E для делегирования методов умножения и деления */
        IEcoCalculatorY* pIE_Y = 0;
        r2 = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorE, 0, &IID_IEcoCalculatorY, (void**)&pIE_Y);
        if (r2 == 0 && pIE_Y != 0) {
            pCMe->m_pICalculatorE_Y = pIE_Y;
        }
        
        /* Создание точки подключения */
        pCMe->m_pISinkCP = 0;
        result = createCEcoLab1ConnectionPoint((IEcoUnknown*)pCMe->m_pISys, (IEcoConnectionPointContainer*)&pCMe->m_pVTblICPC, &IID_IEcoLab1Events, (IEcoConnectionPoint**)&((pCMe)->m_pISinkCP));
        if (result != 0 || pCMe->m_pISinkCP == 0) {
            /* Если ConnectionPoint не создан, это не критично - события просто не будут работать */
            pCMe->m_pISinkCP = 0;
            result = 0; /* Не считаем это критической ошибкой */
        }
    }

    /* Освобождение */
    if (pIBus != 0) {
        pIBus->pVTbl->Release(pIBus);
    }

    return result;
}





/* Create Virtual Table IEcoLab1 */
IEcoLab1VTbl g_x277FC00C35624096AFCFC125B94EEC90VTbl = {
    CEcoLab1_QueryInterface,
    CEcoLab1_AddRef,
    CEcoLab1_Release,
    CEcoLab1_MyFunction,
    CEcoLab1_BucketSortInt,
    CEcoLab1_BucketSortLong,
    CEcoLab1_BucketSortFloat,
    CEcoLab1_BucketSortDouble,
    CEcoLab1_BucketSortLongDouble
};

/* Create Virtual Table IEcoConnectionPointContainer */
IEcoConnectionPointContainerVTbl g_x0000000500000000C000000000000046VTblCPC = {
    CEcoLab1_IEcoConnectionPointContainer_QueryInterface,
    CEcoLab1_IEcoConnectionPointContainer_AddRef,
    CEcoLab1_IEcoConnectionPointContainer_Release,
    CEcoLab1_IEcoConnectionPointContainer_EnumConnectionPoints,
    CEcoLab1_IEcoConnectionPointContainer_FindConnectionPoint
};

/*
 *
 * <сводка>
 *   Функция Create
 * </сводка>
 *
 * <описание>
 *   Функция создания экземпляра
 * </описание>
 *
 */
int16_t ECOCALLMETHOD createCEcoLab1(/* in */ IEcoUnknown* pIUnkSystem, /* in */ IEcoUnknown* pIUnkOuter, /* out */ IEcoLab1** ppIEcoLab1) {
    int16_t result = -1;
    IEcoSystem1* pISys = 0;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoInterfaceBus1MemExt* pIMemExt = 0;
    IEcoMemoryAllocator1* pIMem = 0;
    CEcoLab1* pCMe = 0;
    UGUID* rcid = (UGUID*)&CID_EcoMemoryManager1;

    /* Проверка указателей */
    if (ppIEcoLab1 == 0 || pIUnkSystem == 0) {
        return result;
    }

    /* Получение системного интерфейса приложения */
    result = pIUnkSystem->pVTbl->QueryInterface(pIUnkSystem, &GID_IEcoSystem, (void **)&pISys);

    /* Проверка */
    if (result != 0 && pISys == 0) {
        return result;
    }

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);

    /* Получение идентификатора компонента для работы с памятью */
    result = pIBus->pVTbl->QueryInterface(pIBus, &IID_IEcoInterfaceBus1MemExt, (void**)&pIMemExt);
    if (result == 0 && pIMemExt != 0) {
        rcid = (UGUID*)pIMemExt->pVTbl->get_Manager(pIMemExt);
        pIMemExt->pVTbl->Release(pIMemExt);
    }

    /* Получение интерфейса распределителя памяти */
    pIBus->pVTbl->QueryComponent(pIBus, rcid, 0, &IID_IEcoMemoryAllocator1, (void**) &pIMem);

    /* Проверка */
    if (result != 0 && pIMem == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
        pISys->pVTbl->Release(pISys);
        return result;
    }

    /* Выделение памяти для данных экземпляра */
    pCMe = (CEcoLab1*)pIMem->pVTbl->Alloc(pIMem, sizeof(CEcoLab1));

    /* Сохранение указателя на системный интерфейс */
    pCMe->m_pISys = pISys;

    /* Сохранение указателя на интерфейс для работы с памятью */
    pCMe->m_pIMem = pIMem;

    /* Установка счетчика ссылок на компонент */
    pCMe->m_cRef = 1;

    /* Создание таблицы функций интерфейса IEcoLab1 */
    pCMe->m_pVTblIEcoLab1 = &g_x277FC00C35624096AFCFC125B94EEC90VTbl;

    /* Создание таблицы функций интерфейса IEcoConnectionPointContainer */
    pCMe->m_pVTblICPC = &g_x0000000500000000C000000000000046VTblCPC;

    /* Инициализация данных */
    pCMe->m_Name = 0;
    pCMe->m_pISinkCP = 0;
    pCMe->m_pICalculatorX = 0;
    pCMe->m_pICalculatorY = 0;
    pCMe->m_pICalculatorC_X = 0;
    pCMe->m_pICalculatorC_Y = 0;
    pCMe->m_pICalculatorE_Y = 0;

    /* Возврат указателя на интерфейс */
    *ppIEcoLab1 = (IEcoLab1*)pCMe;

    /* Инициализация компонента */
    result = initCEcoLab1((IEcoLab1*)pCMe, pIUnkSystem);
    if (result != 0) {
        /* Освобождение в случае ошибки */
        deleteCEcoLab1((IEcoLab1*)pCMe);
        *ppIEcoLab1 = 0;
        return result;
    }

    /* Освобождение */
    pIBus->pVTbl->Release(pIBus);

    return 0;
}

/*
 *
 * <сводка>
 *   Функция Delete
 * </сводка>
 *
 * <описание>
 *   Функция освобождения экземпляра
 * </описание>
 *
 */
void ECOCALLMETHOD deleteCEcoLab1(/* in */ IEcoLab1* pIEcoLab1) {
    CEcoLab1* pCMe = (CEcoLab1*)pIEcoLab1;
    IEcoMemoryAllocator1* pIMem = 0;

    if (pIEcoLab1 != 0 ) {
        pIMem = pCMe->m_pIMem;
        /* Освобождение */
        if ( pCMe->m_Name != 0 ) {
            pIMem->pVTbl->Free(pIMem, pCMe->m_Name);
        }
        if ( pCMe->m_pICalculatorX != 0 ) {
            pCMe->m_pICalculatorX->pVTbl->Release(pCMe->m_pICalculatorX);
        }
        if ( pCMe->m_pICalculatorY != 0 ) {
            pCMe->m_pICalculatorY->pVTbl->Release(pCMe->m_pICalculatorY);
        }
        if ( pCMe->m_pICalculatorC_X != 0 ) {
            pCMe->m_pICalculatorC_X->pVTbl->Release(pCMe->m_pICalculatorC_X);
        }
        if ( pCMe->m_pICalculatorC_Y != 0 ) {
            pCMe->m_pICalculatorC_Y->pVTbl->Release(pCMe->m_pICalculatorC_Y);
        }
        if ( pCMe->m_pICalculatorE_Y != 0 ) {
            pCMe->m_pICalculatorE_Y->pVTbl->Release(pCMe->m_pICalculatorE_Y);
        }
        if ( pCMe->m_pISinkCP != 0 ) {
            deleteCEcoLab1ConnectionPoint((IEcoConnectionPoint*)pCMe->m_pISinkCP);
        }
        if ( pCMe->m_pISys != 0 ) {
            pCMe->m_pISys->pVTbl->Release(pCMe->m_pISys);
        }
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
}

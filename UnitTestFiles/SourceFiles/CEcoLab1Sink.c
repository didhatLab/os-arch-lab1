/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   CEcoLab1Sink
 * </сводка>
 *
 * <описание>
 *   Данный исходный код описывает реализацию интерфейсов CEcoLab1Sink
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2016 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#include "CEcoLab1Sink.h"
#include "IEcoConnectionPointContainer.h"
#include "stdio.h"

/*
 *
 * <сводка>
 *   Функция QueryInterface
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1Sink_QueryInterface(/* in */ struct IEcoLab1Events* me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    if ( IsEqualUGUID(riid, &IID_IEcoLab1Events ) ) {
        *ppv = me;
        me->pVTbl->AddRef(me);
        return 0;
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown ) ) {
        *ppv = me;
        me->pVTbl->AddRef(me);
        return 0;
    }

    *ppv = 0;

    return -1;
}

/*
 *
 * <сводка>
 *   Функция AddRef
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
uint32_t ECOCALLMETHOD CEcoLab1Sink_AddRef(/* in */ struct IEcoLab1Events* me) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*)me;

    if (me == 0 ) {
        return -1;
    }

    pCMe->m_cRef++;
    return pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция Release
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
uint32_t ECOCALLMETHOD CEcoLab1Sink_Release(/* in */ struct IEcoLab1Events* me) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*)me;

    if (me == 0 ) {
        return -1;
    }

    /* Уменьшение счетчика ссылок на компонент */
    --pCMe->m_cRef;

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if ( pCMe->m_cRef == 0 ) {
        deleteCEcoLab1Sink((IEcoLab1Events*)pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция OnMyCallback
 * </сводка>
 *
 * <описание>
 *   Функция обратного вызова
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1Sink_OnMyCallback(/* in */ struct IEcoLab1Events* me, /* in */ char_t* Name) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*)me;

    if (me == 0 ) {
        return -1;
    }


    return 0;
}

/*
 *
 * <сводка>
 *   Функция OnSortSwap
 * </сводка>
 *
 * <описание>
 *   Функция обработки события обмена элементов при сортировке
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1Sink_OnSortSwap(/* in */ struct IEcoLab1Events* me, /* in */ int16_t indexSrc, /* in */ int16_t indexDst) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*)me;

    if (me == 0 ) {
        return -1;
    }

    pCMe->m_iStep++;
    printf("Step %d : swapping places %d <-> %d\n", pCMe->m_iStep, indexSrc, indexDst);
    return 0;
}


/*
 *
 * <сводка>
 *   Функция Advise
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1Sink_Advise(/* in */ struct CEcoLab1Sink* me, /* in */IEcoLab1 *pIEcoLab1) {
    IEcoConnectionPointContainer* pCPC = 0;
    IEcoConnectionPoint* pCP = 0;
    int16_t result = 0;

    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoConnectionPointContainer, (void**)&pCPC);
    if (result != 0 || pCPC == 0) {
        return result;
    }

    result = pCPC->pVTbl->FindConnectionPoint(pCPC, &IID_IEcoLab1Events, &pCP);
    pCPC->pVTbl->Release(pCPC);
    pCPC = 0;
    if (result != 0 || pCP == 0) {
        return result;
    }

    result = pCP->pVTbl->Advise(pCP, (IEcoUnknown*)me, &me->m_cCookie);
    pCP->pVTbl->Release(pCP);
    pCP = 0;

    return result;
}

/*
 *
 * <сводка>
 *   Функция Unadvise
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1Sink_Unadvise(/* in */ struct CEcoLab1Sink* me, /* in */IEcoLab1 *pIEcoLab1) {
    IEcoConnectionPointContainer* pCPC = 0;
    IEcoConnectionPoint * pCP = 0;
    int16_t result = 0;

    if (me->m_cCookie) {
        result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoConnectionPointContainer, (void**)&pCPC);
        if (result == 0) {
            result = pCPC->pVTbl->FindConnectionPoint(pCPC, &IID_IEcoLab1Events, &pCP);
            pCPC->pVTbl->Release(pCPC);
            pCPC = 0;
            if (result == 0) {
                result = pCP->pVTbl->Unadvise(pCP, me->m_cCookie);
                pCP->pVTbl->Release(pCP);
                pCP = 0;
            }
        }
    }
    return result;
}

/*
 *
 * <сводка>
 *   Функция OnBucketSortStart
 * </сводка>
 *
 * <описание>
 *   Функция обработки события начала сортировки
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1Sink_OnBucketSortStart(/* in */ struct IEcoLab1Events* me, /* in */ uint32_t arrayLength) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*)me;

    if (me == 0 ) {
        return -1;
    }

    printf("OnBucketSortStart: Starting bucket sort for array of length %u\n", (unsigned int)arrayLength);
    return 0;
}

/*
 *
 * <сводка>
 *   Функция OnMinMaxFound
 * </сводка>
 *
 * <описание>
 *   Функция обработки события нахождения минимального и максимального значений
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1Sink_OnMinMaxFound(/* in */ struct IEcoLab1Events* me, /* in */ double minValue, /* in */ double maxValue) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*)me;

    if (me == 0 ) {
        return -1;
    }

    printf("OnMinMaxFound: Min=%.2f, Max=%.2f, Range=%.2f\n", minValue, maxValue, maxValue - minValue);
    return 0;
}

/*
 *
 * <сводка>
 *   Функция OnBucketsCreated
 * </сводка>
 *
 * <описание>
 *   Функция обработки события создания корзин
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1Sink_OnBucketsCreated(/* in */ struct IEcoLab1Events* me, /* in */ uint32_t bucketCount) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*)me;

    if (me == 0 ) {
        return -1;
    }

    printf("OnBucketsCreated: Created %u buckets\n", (unsigned int)bucketCount);
    return 0;
}

/*
 *
 * <сводка>
 *   Функция OnElementDistributed
 * </сводка>
 *
 * <описание>
 *   Функция обработки события распределения элемента в корзину
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1Sink_OnElementDistributed(/* in */ struct IEcoLab1Events* me, /* in */ uint32_t elementIndex, /* in */ uint32_t bucketIndex) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*)me;

    if (me == 0 ) {
        return -1;
    }

    printf("OnElementDistributed: Element[%u] -> Bucket[%u]\n", (unsigned int)elementIndex, (unsigned int)bucketIndex);
    return 0;
}

/*
 *
 * <сводка>
 *   Функция OnBucketSortingStarted
 * </сводка>
 *
 * <описание>
 *   Функция обработки события начала сортировки корзины
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1Sink_OnBucketSortingStarted(/* in */ struct IEcoLab1Events* me, /* in */ uint32_t bucketIndex, /* in */ uint32_t bucketSize) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*)me;

    if (me == 0 ) {
        return -1;
    }

    printf("OnBucketSortingStarted: Sorting bucket[%u] with %u elements\n", (unsigned int)bucketIndex, (unsigned int)bucketSize);
    return 0;
}

/*
 *
 * <сводка>
 *   Функция OnBucketSorted
 * </сводка>
 *
 * <описание>
 *   Функция обработки события завершения сортировки корзины
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1Sink_OnBucketSorted(/* in */ struct IEcoLab1Events* me, /* in */ uint32_t bucketIndex) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*)me;

    if (me == 0 ) {
        return -1;
    }

    printf("OnBucketSorted: Bucket[%u] sorted\n", (unsigned int)bucketIndex);
    return 0;
}

/*
 *
 * <сводка>
 *   Функция OnBucketSortCompleted
 * </сводка>
 *
 * <описание>
 *   Функция обработки события завершения сортировки
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1Sink_OnBucketSortCompleted(/* in */ struct IEcoLab1Events* me) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*)me;

    if (me == 0 ) {
        return -1;
    }

    printf("OnBucketSortCompleted: Bucket sort finished successfully\n");
    return 0;
}

/* Create Virtual Table IEcoLab1VTblEvents */
IEcoLab1VTblEvents g_x2D2E3B9214F248A6A09ECB494B59C795VTblEvents = {
    CEcoLab1Sink_QueryInterface,
    CEcoLab1Sink_AddRef,
    CEcoLab1Sink_Release,
    CEcoLab1Sink_OnMyCallback,
    CEcoLab1Sink_OnBucketSortStart,
    CEcoLab1Sink_OnMinMaxFound,
    CEcoLab1Sink_OnBucketsCreated,
    CEcoLab1Sink_OnElementDistributed,
    CEcoLab1Sink_OnBucketSortingStarted,
    CEcoLab1Sink_OnSortSwap,
    CEcoLab1Sink_OnBucketSorted,
    CEcoLab1Sink_OnBucketSortCompleted
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
int16_t ECOCALLMETHOD createCEcoLab1Sink(/* in */ IEcoMemoryAllocator1* pIMem, /* out */ IEcoLab1Events** ppIEcoLab1Events) {
    int16_t result = -1;
    CEcoLab1Sink* pCMe = 0;

    /* Проверка указателей */
    if (ppIEcoLab1Events == 0 || pIMem == 0 ) {
        return result;
    }

    /* Выделение памяти для данных экземпляра */
    pCMe = (CEcoLab1Sink*)pIMem->pVTbl->Alloc(pIMem, sizeof(CEcoLab1Sink));

    /* Сохранение указателя на интерфейс для работы с памятью */
    pCMe->m_pIMem = pIMem;
    pCMe->m_pIMem->pVTbl->AddRef(pCMe->m_pIMem);

    /* Установка счетчика ссылок на компонент */
    pCMe->m_cRef = 1;
    pCMe->m_cCookie = 0;
    pCMe->m_iStep = 0;

    /* Создание таблицы функций интерфейса IEcoP2PEvents */
    pCMe->m_pVTblIEcoLab1Events = &g_x2D2E3B9214F248A6A09ECB494B59C795VTblEvents;

    /* Инициализация вспомогательных функций */
    pCMe->Advise = CEcoLab1Sink_Advise;
    pCMe->Unadvise = CEcoLab1Sink_Unadvise;

    *ppIEcoLab1Events = (IEcoLab1Events*)pCMe;

    return 0;
};

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
void ECOCALLMETHOD deleteCEcoLab1Sink(IEcoLab1Events* pIEcoLab1Events) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*)pIEcoLab1Events;
    IEcoMemoryAllocator1* pIMem = 0;

    if (pIEcoLab1Events != 0 ) {
        pIMem = pCMe->m_pIMem;
        /* Освобождение */
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
};

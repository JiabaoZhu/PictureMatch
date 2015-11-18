#include "LockClass.h"

LockClass::LockClass(std::mutex *pMutex)
{
    m_pMutex = pMutex;
    m_pMutex->lock();
}

LockClass::~LockClass()
{
    m_pMutex->unlock();
    m_pMutex = NULL;
}

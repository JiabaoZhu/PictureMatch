#ifndef LOCKCLASS_H
#define LOCKCLASS_H

#include <mutex>

class LockClass
{
public:
    LockClass(std::mutex *pMutex);
    ~LockClass();

private:
    std::mutex *m_pMutex;
};

#endif // LOCKCLASS_H

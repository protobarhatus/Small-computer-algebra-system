#include "absexmemorychecker.h"
#include "QDebug"
AbsExMemoryChecker &AbsExMemoryChecker::get()
{
    static AbsExMemoryChecker inst;
    return inst;
}

void AbsExMemoryChecker::enableWritingMemory()
{
    get().enabled = true;
}

int AbsExMemoryChecker::addObject()
{
    if (!get().enabled)
        return 0;
    get().objects_in_memory.insert(get().id);
    ++get().id;
    return get().id - 1;
}

void AbsExMemoryChecker::deleteObject(int id)
{
    if (!get().enabled)
        return;
    get().objects_in_memory.erase(id);

}

void AbsExMemoryChecker::debugObjectsInMemory()
{
    qDebug() << "Objects: ";
    auto debug_stream = qDebug();
    for (auto &it : get().objects_in_memory)
        debug_stream << it << ' ';
}

void AbsExMemoryChecker::clear()
{
    get().objects_in_memory.clear();
    get().enabled = false;
    get().id = 0;
}

AbsExMemoryChecker::AbsExMemoryChecker()
{

}

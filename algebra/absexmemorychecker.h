#ifndef ABSEXMEMORYCHECKER_H
#define ABSEXMEMORYCHECKER_H
#include <set>

class AbsExMemoryChecker
{
public:

    static AbsExMemoryChecker& get();
    static void enableWritingMemory();
    static int addObject();
    static void deleteObject(int id);
    static void debugObjectsInMemory();

    static void clear();
private:
    bool enabled = false;
    std::set<int> objects_in_memory;
    int id = 0;
    AbsExMemoryChecker();
    AbsExMemoryChecker(const AbsExMemoryChecker &) = delete;
    AbsExMemoryChecker& operator=(const AbsExMemoryChecker &) = delete;
};

#endif // ABSEXMEMORYCHECKER_H

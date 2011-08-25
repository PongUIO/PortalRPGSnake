#ifndef OBJECTLIST_H
#define OBJECTLIST_H

#include <vector>
#include "baseobject.h"

class ObjectList {
public:

    ObjectList();

    void thinkAll();
    void renderAll();

    void update();

    void add(BaseObject* obj);

protected:
private:

    std::vector<BaseObject*> objects;
    std::vector<BaseObject*> new_objects;

};

#endif // OBJECTLIST_H

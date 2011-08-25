#include "baseobject.h"

static int BaseObject::last_id = 0;

BaseObject::BaseObject() {
    x_pos = 0.0f;
    y_pos = 0.0f;

    object_id = getNextId();
}

void BaseObject::spawn() {

}

void BaseObject::activate() {

}

void BaseObject::preThink() {

}

void BaseObject::think() {

}

void BaseObject::postThink() {

}


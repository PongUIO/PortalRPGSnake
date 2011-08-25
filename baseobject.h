#ifndef BASEOBJECT_H
#define BASEOBJECT_H

class BaseObject {
public:

    BaseObject();

    int objectId() { return object_id; }

    virtual void spawn();
    virtual void activate();

    virtual void preThink();
    virtual void think();
    virtual void postThink();

    // World position
    void setPos(float x, float y) { x_pos = x; y_pos = y; }
    virtual float getX() { return x_pos; }
    virtual float getY() { return y_pos; }

    virtual float getWidth() { return width; }
    virtual float getHeight() { return height; }

    // virtual void preRender();
    // virtual void postRender();

    bool isInPortal() { return inPortal; }

protected:
private:

    static int getNextId() {
        int ret = last_id;
        last_id++;
        return ret;
    }

    static int last_id;

    int object_id;

    bool inPortal;

    float x_pos;
    float y_pos;

    float width;
    float height;

    bool should_kill;
};

#endif // BASEOBJECT_H

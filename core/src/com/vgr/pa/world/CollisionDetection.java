package com.vgr.pa.world;

import com.badlogic.ashley.core.Entity;

public interface CollisionDetection {

    void onCollide(Entity a, short categoryA, Entity b, short categoryB);
}

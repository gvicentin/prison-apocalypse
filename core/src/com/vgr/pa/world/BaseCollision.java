package com.vgr.pa.world;

import com.badlogic.ashley.core.Entity;

public abstract class BaseCollision implements CollisionDetection {

    protected short firstCategoryBits;
    protected short secondCategoryBits;

    public BaseCollision(short aCategoryBits, short bCategoryBits) {
        this.firstCategoryBits = aCategoryBits;
        this.secondCategoryBits = bCategoryBits;
    }

    public int getCategoryBits() {
        return (firstCategoryBits | secondCategoryBits);
    }

    public Entity getFirstEntity(Entity a, short catA, Entity b, short catB) {
        return (firstCategoryBits & catA) != 0 ? a : b;
    }

    public Entity getSecondEntity(Entity a, short catA, Entity b, short catB) {
        return (secondCategoryBits & catB) != 0 ? b : a;
    }
}

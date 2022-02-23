package com.vgr.pa.world;

import com.badlogic.ashley.core.ComponentMapper;
import com.badlogic.ashley.core.Entity;
import com.vgr.pa.Constants;
import com.vgr.pa.weapon.BulletComponent;

public class BulletEnvironmentCollision extends BaseCollision {

    private final ComponentMapper<BulletComponent> bm;

    public BulletEnvironmentCollision() {
        super(Constants.LAYER_BULLETS, Constants.LAYER_ENVIRONMENT);

        bm = ComponentMapper.getFor(BulletComponent.class);
    }

    @Override
    public void onCollide(Entity a, short categoryA, Entity b, short categoryB) {
        Entity bulletEntity = getFirstEntity(a, categoryA, b, categoryB);
        BulletComponent bullet = bm.get(bulletEntity);
        bullet.destroyed = true;
    }
}

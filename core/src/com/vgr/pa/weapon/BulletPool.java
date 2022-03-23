package com.vgr.pa.weapon;

import com.badlogic.ashley.core.Entity;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.physics.box2d.BodyDef;
import com.badlogic.gdx.physics.box2d.FixtureDef;
import com.badlogic.gdx.physics.box2d.PolygonShape;
import com.badlogic.gdx.utils.Pool;
import com.vgr.pa.Constants;
import com.vgr.pa.asset.Assets;
import com.vgr.pa.core.PhysicsComponent;
import com.vgr.pa.core.SpriteComponent;
import com.vgr.pa.core.TransformComponent;
import com.vgr.pa.world.GameWorld;

public class BulletPool extends Pool<Entity> {

    private static final String TAG = BulletPool.class.getSimpleName();

    private static final int POOL_SIZE = 100;

    private OnBulletCreation onBulletCreation;

    public BulletPool(OnBulletCreation onBulletCreation) {
        super(POOL_SIZE);

        this.onBulletCreation = onBulletCreation;

        fill(POOL_SIZE);
        Gdx.app.debug(TAG, "Created Bullets, Pool size: " + POOL_SIZE);
    }

    @Override
    protected Entity newObject() {
        return onBulletCreation.createBullet();
    }

    @Override
    protected void reset(Entity object) { }
}

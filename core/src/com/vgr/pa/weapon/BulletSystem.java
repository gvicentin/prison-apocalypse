package com.vgr.pa.weapon;

import com.badlogic.ashley.core.ComponentMapper;
import com.badlogic.ashley.core.Entity;
import com.badlogic.ashley.core.Family;
import com.badlogic.ashley.systems.IteratingSystem;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.math.MathUtils;
import com.badlogic.gdx.math.Vector2;
import com.vgr.pa.Constants;
import com.vgr.pa.character.CharacterFactory;
import com.vgr.pa.core.PhysicsComponent;
import com.vgr.pa.core.SpriteComponent;
import com.vgr.pa.core.TransformComponent;

public class BulletSystem extends IteratingSystem {

    private BulletPool[] bulletPools;
    private BulletPool currentPool;

    // mappers
    private final ComponentMapper<TransformComponent> tm;
    private final ComponentMapper<PhysicsComponent> pm;
    private final ComponentMapper<SpriteComponent> sm;
    private final ComponentMapper<BulletComponent> bm;

    public BulletSystem(GunFactory gunFactory) {
        super(Family.all(BulletComponent.class).get(), Constants.PRIORITY_BULLET);

        this.bulletPools = new BulletPool[] {
                new BulletPool(gunFactory.onCreatePistolBullet),
                new BulletPool(gunFactory.onCreateRifleBullet)
        };

        tm = ComponentMapper.getFor(TransformComponent.class);
        pm = ComponentMapper.getFor(PhysicsComponent.class);
        sm = ComponentMapper.getFor(SpriteComponent.class);
        bm = ComponentMapper.getFor(BulletComponent.class);
    }

    public void spawn(int poolIndex, Vector2 position, float rotation) {
        currentPool = bulletPools[poolIndex];
        Entity bullet = currentPool.obtain();

        BulletComponent bulletComp = bm.get(bullet);
        bulletComp.isActive = true;
        bulletComp.bulletIndex = poolIndex;

        SpriteComponent sprite = sm.get(bullet);
        sprite.hide = false;

        TransformComponent transform = tm.get(bullet);
        transform.position.set(position);
        transform.rotation = MathUtils.radiansToDegrees * rotation;

        PhysicsComponent physics = pm.get(bullet);
        physics.body.setActive(true);
        physics.body.setTransform(position, rotation);
    }

    @Override
    protected void processEntity(Entity entity, float deltaTime) {
        BulletComponent bullet = bm.get(entity);
        PhysicsComponent physics = pm.get(entity);
        SpriteComponent sprite = sm.get(entity);

        if (!bullet.isActive)
            return;

        bullet.liveTime += deltaTime;
        if (bullet.liveTime > bullet.timeToLive || bullet.destroyed) {
            sprite.hide = true;
            physics.body.setActive(false);
            bullet.reset();
            bulletPools[bullet.bulletIndex].free(entity);
        }

        float angle = physics.body.getAngle();
        Vector2 vel = new Vector2(MathUtils.cos(angle), MathUtils.sin(angle));
        vel.scl(bullet.speed);
        physics.body.setLinearVelocity(vel);
    }
}

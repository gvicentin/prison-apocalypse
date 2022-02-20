package com.vgr.pa.weapon;

import com.badlogic.ashley.core.ComponentMapper;
import com.badlogic.ashley.core.Entity;
import com.badlogic.ashley.core.Family;
import com.badlogic.ashley.systems.IteratingSystem;
import com.badlogic.gdx.math.MathUtils;
import com.badlogic.gdx.math.Vector2;
import com.vgr.pa.Constants;
import com.vgr.pa.core.PhysicsComponent;
import com.vgr.pa.core.SpriteComponent;
import com.vgr.pa.core.TransformComponent;

public class BulletSystem extends IteratingSystem {

    private BulletPool pool;

    // mappers
    private ComponentMapper<PhysicsComponent> pm;
    private ComponentMapper<SpriteComponent> sm;
    private ComponentMapper<BulletComponent> bm;

    public BulletSystem(BulletPool pool) {
        super(Family.all(BulletComponent.class).get(), Constants.PRIORITY_BULLET);

        this.pool = pool;

        pm = ComponentMapper.getFor(PhysicsComponent.class);
        sm = ComponentMapper.getFor(SpriteComponent.class);
        bm = ComponentMapper.getFor(BulletComponent.class);
    }

    public void spawn(Vector2 position, float rotation) {
        Entity bullet = pool.obtain();

        BulletComponent bulletComp = bm.get(bullet);
        bulletComp.isActive = true;

        SpriteComponent sprite = sm.get(bullet);
        sprite.hide = false;

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
        if (bullet.liveTime > bullet.timeToLive) {
            sprite.hide = true;
            physics.body.setActive(false);
            bullet.reset();
            pool.free(entity);
        }

        float angle = physics.body.getAngle();
        Vector2 vel = new Vector2(MathUtils.cos(angle), MathUtils.sin(angle));
        vel.scl(bullet.speed);
        physics.body.setLinearVelocity(vel);
    }
}

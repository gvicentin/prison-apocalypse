package com.vgr.pa.core;

import com.badlogic.ashley.core.ComponentMapper;
import com.badlogic.ashley.core.Entity;
import com.badlogic.ashley.core.Family;
import com.badlogic.ashley.systems.IntervalIteratingSystem;
import com.badlogic.gdx.math.MathUtils;
import com.badlogic.gdx.physics.box2d.World;
import com.vgr.pa.Constants;
import com.vgr.pa.world.GameWorld;

public class PhysicsSystem extends IntervalIteratingSystem {

    private World world;

    private ComponentMapper<PhysicsComponent> pm;
    private ComponentMapper<TransformComponent> tm;

    public PhysicsSystem(GameWorld gameWorld) {
        super(Family.all(PhysicsComponent.class, TransformComponent.class).get(),
                Constants.PHYSICS_STEP_DELTA, Constants.PRIORITY_PHYSICS);

        this.world = gameWorld.physicsWorld;

        this.pm = ComponentMapper.getFor(PhysicsComponent.class);
        this.tm = ComponentMapper.getFor(TransformComponent.class);
    }

    @Override
    protected void updateInterval() {
        world.step(Constants.PHYSICS_STEP_DELTA, Constants.VELOCITY_ITERATIONS, Constants.POSITION_ITERATIONS);
        super.updateInterval();
    }

    @Override
    protected void processEntity(Entity entity) {
        PhysicsComponent physics = pm.get(entity);
        TransformComponent transform = tm.get(entity);
        transform.position.set(physics.body.getPosition()).add(physics.bodyOffset);
        transform.rotation = physics.body.getAngle() * MathUtils.radiansToDegrees;

        // update hit box
        if (physics.hitBox != null) {
            physics.hitBox.setTransform(transform.position, 0f);
        }
    }
}

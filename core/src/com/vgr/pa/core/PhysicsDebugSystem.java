package com.vgr.pa.core;

import com.badlogic.ashley.core.EntitySystem;
import com.badlogic.gdx.graphics.OrthographicCamera;
import com.badlogic.gdx.physics.box2d.Box2DDebugRenderer;
import com.badlogic.gdx.physics.box2d.World;
import com.vgr.pa.Constants;
import com.vgr.pa.scene.GameWorld;

public class PhysicsDebugSystem extends EntitySystem {

    private World world;
    private OrthographicCamera camera;
    private Box2DDebugRenderer debugRenderer;

    public PhysicsDebugSystem(GameWorld gameWorld) {
        super(Constants.PRIORITY_PHYSICS_DEBUG);
        this.world = gameWorld.physicsWorld;
        this.camera = gameWorld.getMainCamera();
        this.debugRenderer = new Box2DDebugRenderer();
    }

    @Override
    public void update(float deltaTime) {
        debugRenderer.render(world, camera.combined);
    }
}

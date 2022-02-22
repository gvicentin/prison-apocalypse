package com.vgr.pa.core;

import com.badlogic.ashley.core.Component;
import com.badlogic.gdx.math.Vector2;
import com.badlogic.gdx.physics.box2d.Body;
import com.badlogic.gdx.utils.Pool;

public class PhysicsComponent implements Component, Pool.Poolable {

    public Body body;
    public Vector2 bodyOffset;

    public Body hitBox;
    public Vector2 hitBoxOffset;

    public PhysicsComponent() {
        bodyOffset = new Vector2();
    }

    @Override
    public void reset() {
    }
}

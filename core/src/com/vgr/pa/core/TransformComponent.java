package com.vgr.pa.core;

import com.badlogic.ashley.core.Component;
import com.badlogic.gdx.math.Vector2;
import com.badlogic.gdx.utils.Pool;

public class TransformComponent implements Component, Pool.Poolable {

    public Vector2 position;
    public Vector2 scale;

    // rotation unit is degrees
    public float rotation;

    public TransformComponent() {
        this.position = new Vector2();
        this.scale = new Vector2();
        reset();
    }

    @Override
    public void reset() {
        position.set(0f, 0f);
        scale.set(1f, 1f);
        rotation = 0f;
    }
}

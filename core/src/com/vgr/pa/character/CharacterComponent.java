package com.vgr.pa.character;

import com.badlogic.ashley.core.Component;
import com.badlogic.gdx.math.Vector2;
import com.badlogic.gdx.utils.Pool;

public class CharacterComponent implements Component, Pool.Poolable {

    public static final int STATE_IDLE = 0;
    public static final int STATE_RUN = 1;
    public static final int STATE_HIT = 2;
    public static final int STATE_DIE = 3;

    public boolean facingLeft;
    public boolean isDamage;

    public float health = 100.0f;

    public float speed;
    public Vector2 velocity;

    public CharacterComponent() {
        velocity = new Vector2();
    }

    @Override
    public void reset() {
        velocity.set(0f, 0f);
    }
}

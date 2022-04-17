package com.vgr.pa.weapon;

import com.badlogic.ashley.core.Component;
import com.badlogic.gdx.math.Vector2;
import com.badlogic.gdx.utils.Pool;

public class WeaponComponent implements Component, Pool.Poolable {

    public Vector2 offset;
    public Vector2 nuzzle;

    public float coolDown = 0.25f;
    public float coolDownTimer;

    // rumble effect
    public float power = 0.1f;
    public float duration = 0.05f;

    public WeaponComponent() {
        offset = new Vector2();
        nuzzle = new Vector2();
    }

    @Override
    public void reset() {
        coolDownTimer = coolDown;
    }
}

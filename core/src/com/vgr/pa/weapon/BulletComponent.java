package com.vgr.pa.weapon;

import com.badlogic.ashley.core.Component;
import com.badlogic.gdx.utils.Pool;

public class BulletComponent implements Component, Pool.Poolable {

    public float speed = 10f;
    public float damage = 5f;

    public float timeToLive = 5f;
    public float liveTime = 0f;

    public boolean destroyed = false;
    public boolean isActive = false;

    @Override
    public void reset() {
        liveTime = 0f;
        destroyed = false;
        isActive = false;
    }
}

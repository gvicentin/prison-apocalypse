package com.vgr.pa.player;

import com.badlogic.ashley.core.Component;
import com.badlogic.gdx.utils.Pool;

public class PlayerComponent implements Component, Pool.Poolable {

    public float speed = 5.0f;

    @Override
    public void reset() {
    }
}

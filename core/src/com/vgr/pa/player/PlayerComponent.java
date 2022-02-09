package com.vgr.pa.player;

import com.badlogic.ashley.core.Component;
import com.badlogic.gdx.utils.Pool;

public class PlayerComponent implements Component, Pool.Poolable {

    public static final int ANIM_IDLE = 0;
    public static final int ANIM_RUN = 1;
    public static final int ANIM_HIT = 2;
    public static final int ANIM_DIE = 3;

    public float speed = 5.0f;

    @Override
    public void reset() {
    }
}

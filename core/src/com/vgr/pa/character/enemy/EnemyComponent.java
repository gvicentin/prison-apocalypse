package com.vgr.pa.character.enemy;

import com.badlogic.ashley.core.Component;
import com.badlogic.gdx.utils.Pool;

public class EnemyComponent implements Component, Pool.Poolable {

    public float detectRadius = 20f;

    public float attackCoolDown = 0.6f;
    public float attackTimer = 0.0f;

    @Override
    public void reset() {
        attackTimer = 0.0f;
    }
}

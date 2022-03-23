package com.vgr.pa.character.player;

import com.badlogic.ashley.core.Component;
import com.badlogic.ashley.core.Entity;
import com.badlogic.gdx.utils.Pool;

public class PlayerComponent implements Component, Pool.Poolable {

    public Entity currentGun;
    public int gunSelectionIndex;

    @Override
    public void reset() {
        currentGun = null;
    }
}

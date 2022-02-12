package com.vgr.pa.player;

import com.badlogic.ashley.core.Component;
import com.badlogic.gdx.graphics.g2d.TextureRegion;
import com.badlogic.gdx.utils.Pool;

public class AimComponent implements Component, Pool.Poolable {

    public TextureRegion texture;

    @Override
    public void reset() {
        texture = null;
    }
}

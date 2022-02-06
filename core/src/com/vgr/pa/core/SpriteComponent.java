package com.vgr.pa.core;

import com.badlogic.ashley.core.Component;
import com.badlogic.gdx.graphics.g2d.TextureRegion;
import com.badlogic.gdx.math.Vector2;
import com.badlogic.gdx.utils.Pool;

public class SpriteComponent implements Component, Pool.Poolable {

    // texture
    public TextureRegion region;
    public boolean flipX;
    public boolean flipY;

    // sizing
    public Vector2 size;
    public Vector2 origin;

    // render order
    public int zIndex;

    public SpriteComponent() {
        this.size = new Vector2();
        this.origin = new Vector2();
        reset();
    }

    @Override
    public void reset() {
        region = null;
        flipX = false;
        flipY = false;
        size.set(1f, 1f);
        origin.set(0.5f, 0.5f);
        zIndex = 0;
    }
}

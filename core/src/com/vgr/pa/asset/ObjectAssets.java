package com.vgr.pa.asset;

import com.badlogic.gdx.graphics.g2d.TextureAtlas;
import com.badlogic.gdx.graphics.g2d.TextureRegion;

public class ObjectAssets {

    public TextureRegion barrel;
    public TextureRegion barrelExploded;
    public TextureRegion locker;
    public TextureRegion lockerOpened;

    public ObjectAssets(TextureAtlas atlas) {
        barrel = atlas.findRegion("barrel");
        barrelExploded = atlas.findRegion("barrel_exploded");
        locker = atlas.findRegion("locker");
        lockerOpened = atlas.findRegion("locker_opened");
    }
}

package com.vgr.pa.asset;

import com.badlogic.gdx.graphics.g2d.TextureAtlas;
import com.badlogic.gdx.graphics.g2d.TextureRegion;

public class UI {

    public TextureRegion aimOpen;

    public UI(TextureAtlas atlas) {
        aimOpen = atlas.findRegion("aim_open");
    }
}

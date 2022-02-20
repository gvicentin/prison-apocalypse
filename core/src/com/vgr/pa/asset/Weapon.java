package com.vgr.pa.asset;

import com.badlogic.gdx.graphics.g2d.TextureAtlas;
import com.badlogic.gdx.graphics.g2d.TextureRegion;

public class Weapon {

    // guns
    public TextureRegion pistol;
    public TextureRegion rifle;
    public TextureRegion laser;

    // bullet
    public TextureRegion pistolBullet;
    public TextureRegion rifleBullet;
    public TextureRegion laserBeam;

    public TextureRegion gunFlash;

    public Weapon(TextureAtlas atlas) {
        pistol = atlas.findRegion("pistol");
        rifle = atlas.findRegion("rifle");
        laser = atlas.findRegion("laser");
        pistolBullet = atlas.findRegion("pistol_bullet");
        rifleBullet = atlas.findRegion("rifle_bullet");
        laserBeam = atlas.findRegion("laser_beam");
        gunFlash = atlas.findRegion("flash");
    }
}

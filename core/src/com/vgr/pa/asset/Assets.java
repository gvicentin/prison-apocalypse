package com.vgr.pa.asset;

import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.assets.AssetManager;
import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.graphics.g2d.Animation;
import com.badlogic.gdx.graphics.g2d.TextureRegion;
import com.badlogic.gdx.utils.Array;
import com.badlogic.gdx.utils.Disposable;
import com.badlogic.gdx.utils.TimeUtils;

public class Assets implements Disposable {

    public static Assets instance = new Assets();

    public Character policemen;
    public Character prisoner;
    public Character zombiePolicemen;
    public Character zombiePrisoner;

    private static final String TAG = Assets.class.getSimpleName();

    private AssetManager manager;

    private Assets() {
        manager = new AssetManager();
    }

    public void finishLoading() {
        double loadTimeInSecs;
        long startTime = TimeUtils.millis();

        // load resources
        Gdx.app.log(TAG, "Loading resources...");
        load();
        manager.finishLoading();

        loadTimeInSecs = (TimeUtils.millis() - startTime) / 1000.0;
        Gdx.app.log(TAG, String.format("Loaded resources in %.2f seconds", loadTimeInSecs));

        // create assets instances
        Gdx.app.log(TAG, "Creating assets");
        setupInstances();
    }

    private void load() {
        // characters
        manager.load(FilePaths.POLICEMEN_TEXTURE, Texture.class);
        manager.load(FilePaths.PRISONER_TEXTURE, Texture.class);
        manager.load(FilePaths.ZOMBIE_POLICEMEN_TEXTURE, Texture.class);
        manager.load(FilePaths.ZOMBIE_PRISONER_TEXTURE, Texture.class);
    }

    private Array<TextureRegion> createCharacterRegions(Texture tex, int frameStart, int frameEnd) {
        Array<TextureRegion> regions = new Array<>();
        for (int i = frameStart; i <= frameEnd; i++) {
            TextureRegion region = new TextureRegion(
                    tex,
                    i * Character.FRAME_WIDTH,
                    0,
                    Character.FRAME_WIDTH,
                    Character.FRAME_HEIGHT
            );
            regions.add(region);
        }
        return regions;
    }

    private void createPolicemen() {
        // get assets
        Texture policemenTex = manager.get(FilePaths.POLICEMEN_TEXTURE);

        this.policemen = new Character();

        // create animations
        this.policemen.animationsMap.put("idle", new Animation<TextureRegion>(
                Character.POLICEMEN_IDLE_DURATION,
                createCharacterRegions(policemenTex, Character.POLICEMEN_IDLE_START, Character.POLICEMEN_IDLE_END),
                Animation.PlayMode.LOOP
        ));
        this.policemen.animationsMap.put("run", new Animation<TextureRegion>(
                Character.POLICEMEN_RUN_DURATION,
                createCharacterRegions(policemenTex, Character.POLICEMEN_RUN_START, Character.POLICEMEN_RUN_END),
                Animation.PlayMode.LOOP
        ));
        this.policemen.animationsMap.put("hit", new Animation<TextureRegion>(
                Character.POLICEMEN_HIT_DURATION,
                createCharacterRegions(policemenTex, Character.POLICEMEN_HIT_START, Character.POLICEMEN_HIT_END),
                Animation.PlayMode.LOOP
        ));
        this.policemen.animationsMap.put("die", new Animation<TextureRegion>(
                Character.POLICEMEN_DIE_DURATION,
                createCharacterRegions(policemenTex, Character.POLICEMEN_DIE_START, Character.POLICEMEN_DIE_END),
                Animation.PlayMode.LOOP
        ));
    }

    private void createPrisoner() {
        // get assets
        Texture prisonerTex = manager.get(FilePaths.PRISONER_TEXTURE);

        this.prisoner = new Character();

        // create animations
        this.prisoner.animationsMap.put("idle", new Animation<TextureRegion>(
                Character.PRISONER_IDLE_DURATION,
                createCharacterRegions(prisonerTex, Character.PRISONER_IDLE_START, Character.PRISONER_IDLE_END),
                Animation.PlayMode.LOOP
        ));
        this.prisoner.animationsMap.put("run", new Animation<TextureRegion>(
                Character.PRISONER_RUN_DURATION,
                createCharacterRegions(prisonerTex, Character.PRISONER_RUN_START, Character.PRISONER_RUN_END),
                Animation.PlayMode.LOOP
        ));
        this.prisoner.animationsMap.put("hit", new Animation<TextureRegion>(
                Character.PRISONER_HIT_DURATION,
                createCharacterRegions(prisonerTex, Character.PRISONER_HIT_START, Character.PRISONER_HIT_END),
                Animation.PlayMode.LOOP
        ));
        this.prisoner.animationsMap.put("die", new Animation<TextureRegion>(
                Character.PRISONER_DIE_DURATION,
                createCharacterRegions(prisonerTex, Character.PRISONER_DIE_START, Character.PRISONER_DIE_END),
                Animation.PlayMode.LOOP
        ));
    }

    private void setupInstances() {
        // characters
        createPolicemen();
        createPrisoner();
        // TODO: load all characters
    }

    @Override
    public void dispose() {
        Gdx.app.log(TAG, "Destroying all resources");
        manager.dispose();
    }
}

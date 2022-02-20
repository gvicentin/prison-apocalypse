package com.vgr.pa.asset;

import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.assets.AssetManager;
import com.badlogic.gdx.assets.loaders.resolvers.InternalFileHandleResolver;
import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.graphics.g2d.Animation;
import com.badlogic.gdx.graphics.g2d.TextureAtlas;
import com.badlogic.gdx.graphics.g2d.TextureRegion;
import com.badlogic.gdx.maps.tiled.TiledMap;
import com.badlogic.gdx.maps.tiled.TmxMapLoader;
import com.badlogic.gdx.utils.Array;
import com.badlogic.gdx.utils.Disposable;
import com.badlogic.gdx.utils.TimeUtils;

public class Assets implements Disposable {

    public static Assets instance = new Assets();

    // characters
    public Character policemen;
    public Character prisoner;
    public Character zombiePolicemen;
    public Character zombiePrisoner;

    public TiledMap sandboxMap;

    // objects
    public Weapon weapon;

    public UI ui;

    private static final String TAG = Assets.class.getSimpleName();

    private final AssetManager manager;

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
        manager.setLoader(TiledMap.class, new TmxMapLoader(new InternalFileHandleResolver()));

        // characters
        manager.load(FilePaths.TEXTURE_POLICEMEN, Texture.class);
        manager.load(FilePaths.TEXTURE_PRISONER, Texture.class);
        manager.load(FilePaths.TEXTURE_ZOMBIE_POLICEMEN, Texture.class);
        manager.load(FilePaths.TEXTURE_ZOMBIE_PRISONER, Texture.class);

        // map
        manager.load(FilePaths.MAP_SANDBOX, TiledMap.class);

        // objects
        manager.load(FilePaths.ATLAS_WEAPONS, TextureAtlas.class);

        // ui
        manager.load(FilePaths.ATLAS_UI, TextureAtlas.class);
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
        Texture policemenTex = manager.get(FilePaths.TEXTURE_POLICEMEN);

        this.policemen = new Character();

        // create animations
        this.policemen.idleAnimation = new Animation<>(
                Character.POLICEMEN_IDLE_DURATION,
                createCharacterRegions(policemenTex, Character.POLICEMEN_IDLE_START, Character.POLICEMEN_IDLE_END),
                Animation.PlayMode.LOOP
        );
        this.policemen.runAnimation = new Animation<>(
                Character.POLICEMEN_RUN_DURATION,
                createCharacterRegions(policemenTex, Character.POLICEMEN_RUN_START, Character.POLICEMEN_RUN_END),
                Animation.PlayMode.LOOP
        );
        this.policemen.hitAnimation = new Animation<>(
                Character.POLICEMEN_HIT_DURATION,
                createCharacterRegions(policemenTex, Character.POLICEMEN_HIT_START, Character.POLICEMEN_HIT_END),
                Animation.PlayMode.LOOP
        );
        this.policemen.dieAnimation = new Animation<>(
                Character.POLICEMEN_DIE_DURATION,
                createCharacterRegions(policemenTex, Character.POLICEMEN_DIE_START, Character.POLICEMEN_DIE_END),
                Animation.PlayMode.LOOP
        );
    }

    private void createPrisoner() {
        // get assets
        Texture prisonerTex = manager.get(FilePaths.TEXTURE_PRISONER);

        this.prisoner = new Character();

        // create animations
        this.prisoner.idleAnimation = new Animation<>(
                Character.PRISONER_IDLE_DURATION,
                createCharacterRegions(prisonerTex, Character.PRISONER_IDLE_START, Character.PRISONER_IDLE_END),
                Animation.PlayMode.LOOP
        );
        this.prisoner.runAnimation = new Animation<>(
                Character.PRISONER_RUN_DURATION,
                createCharacterRegions(prisonerTex, Character.PRISONER_RUN_START, Character.PRISONER_RUN_END),
                Animation.PlayMode.LOOP
        );
        this.prisoner.hitAnimation = new Animation<>(
                Character.PRISONER_HIT_DURATION,
                createCharacterRegions(prisonerTex, Character.PRISONER_HIT_START, Character.PRISONER_HIT_END),
                Animation.PlayMode.LOOP
        );
        this.prisoner.dieAnimation = new Animation<>(
                Character.PRISONER_DIE_DURATION,
                createCharacterRegions(prisonerTex, Character.PRISONER_DIE_START, Character.PRISONER_DIE_END),
                Animation.PlayMode.LOOP
        );
    }

    private void setupInstances() {
        // characters
        createPolicemen();
        createPrisoner();
        // TODO: load all characters

        // map
        this.sandboxMap = manager.get(FilePaths.MAP_SANDBOX);

        // objects
        TextureAtlas weaponAtlas = manager.get(FilePaths.ATLAS_WEAPONS);
        this.weapon = new Weapon(weaponAtlas);

        // ui
        TextureAtlas uiAtlas = manager.get(FilePaths.ATLAS_UI);
        this.ui = new UI(uiAtlas);
    }

    @Override
    public void dispose() {
        Gdx.app.log(TAG, "Destroying all resources");
        manager.dispose();
    }
}

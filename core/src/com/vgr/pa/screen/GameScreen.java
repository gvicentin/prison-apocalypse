package com.vgr.pa.screen;

import com.badlogic.ashley.core.Engine;
import com.badlogic.ashley.core.PooledEngine;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.Screen;
import com.badlogic.gdx.graphics.OrthographicCamera;
import com.badlogic.gdx.graphics.g2d.SpriteBatch;
import com.badlogic.gdx.math.Vector2;
import com.badlogic.gdx.physics.box2d.World;
import com.badlogic.gdx.utils.ScreenUtils;
import com.badlogic.gdx.utils.viewport.FitViewport;
import com.badlogic.gdx.utils.viewport.Viewport;
import com.vgr.pa.Constants;
import com.vgr.pa.PrisonApocalypse;
import com.vgr.pa.Settings;
import com.vgr.pa.asset.Assets;
import com.vgr.pa.character.CharacterSystem;
import com.vgr.pa.character.enemy.EnemySystem;
import com.vgr.pa.character.player.AimSystem;
import com.vgr.pa.character.player.CameraSystem;
import com.vgr.pa.character.player.PlayerSystem;
import com.vgr.pa.core.AnimationSystem;
import com.vgr.pa.core.EntityRenderSystem;
import com.vgr.pa.core.PhysicsDebugSystem;
import com.vgr.pa.core.PhysicsSystem;
import com.vgr.pa.map.MapSystem;
import com.vgr.pa.weapon.BulletSystem;
import com.vgr.pa.weapon.GunFactory;
import com.vgr.pa.weapon.WeaponSystem;
import com.vgr.pa.world.GameWorld;
import com.vgr.pa.world.WorldContact;

public class GameScreen implements Screen {

    private static final String TAG = GameScreen.class.getSimpleName();

    private final PrisonApocalypse game;

    private Engine engine;
    private World physicsWorld;
    private Viewport viewport;

    public GameScreen(PrisonApocalypse game) {
        this.game = game;

        // physics world
        physicsWorld = new World(Vector2.Zero, true);
        WorldContact worldContact = new WorldContact();
        physicsWorld.setContactListener(worldContact);

        // create engine and game scene
        engine = new PooledEngine();
        GameWorld gameWorld = new GameWorld(engine, physicsWorld, game.batch, Assets.instance.sandboxMap);

        // setup main camera
        OrthographicCamera mainCamera = gameWorld.getMainCamera();
        viewport = new FitViewport(Constants.VIEWPORT_WIDTH, Constants.VIEWPORT_HEIGHT, mainCamera);

        // systems
        engine.addSystem(new MapSystem(gameWorld));
        engine.addSystem(new PlayerSystem(gameWorld));
        engine.addSystem(new AimSystem(gameWorld));
        GunFactory gunFactory = new GunFactory(engine, physicsWorld);
        BulletSystem bulletSystem = new BulletSystem(gunFactory);
        engine.addSystem(bulletSystem);
        engine.addSystem(new WeaponSystem(gameWorld, bulletSystem));
        engine.addSystem(new EnemySystem(gameWorld));
        engine.addSystem(new CharacterSystem());
        engine.addSystem(new CameraSystem(gameWorld));
        engine.addSystem(new AnimationSystem());
        engine.addSystem(new EntityRenderSystem(gameWorld));
        engine.addSystem(new PhysicsSystem(gameWorld));
        if (Settings.instance.debugPhysics)
            engine.addSystem(new PhysicsDebugSystem(gameWorld));
    }

    @Override
    public void show() {

    }

    @Override
    public void render(float delta) {
        ScreenUtils.clear(Constants.BACKGROUND_COLOR);
        engine.update(Gdx.graphics.getDeltaTime());
    }

    @Override
    public void resize(int width, int height) {
        Gdx.app.log(TAG, String.format("Resized (%d, %d)", width, height));
        viewport.update(width, height);
    }

    @Override
    public void pause() {

    }

    @Override
    public void resume() {

    }

    @Override
    public void hide() {

    }

    @Override
    public void dispose() {
        physicsWorld.dispose();
    }
}

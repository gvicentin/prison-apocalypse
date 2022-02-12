package com.vgr.pa;

import com.badlogic.ashley.core.Engine;
import com.badlogic.ashley.core.PooledEngine;
import com.badlogic.gdx.ApplicationAdapter;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.OrthographicCamera;
import com.badlogic.gdx.graphics.g2d.SpriteBatch;
import com.badlogic.gdx.math.Vector2;
import com.badlogic.gdx.physics.box2d.World;
import com.badlogic.gdx.utils.ScreenUtils;
import com.badlogic.gdx.utils.viewport.FitViewport;
import com.badlogic.gdx.utils.viewport.Viewport;
import com.vgr.pa.asset.Assets;
import com.vgr.pa.core.AnimationSystem;
import com.vgr.pa.core.EntityRenderSystem;
import com.vgr.pa.core.PhysicsDebugSystem;
import com.vgr.pa.core.PhysicsSystem;
import com.vgr.pa.map.MapSystem;
import com.vgr.pa.player.AimSystem;
import com.vgr.pa.player.CameraSystem;
import com.vgr.pa.player.PlayerSystem;
import com.vgr.pa.scene.GameScene;

public class PrisonApocalypse extends ApplicationAdapter {

	private static final String TAG = PrisonApocalypse.class.getSimpleName();

	private Viewport viewport;
	private SpriteBatch batch;
	private Engine engine;
	private World physicsWorld;

	@Override
	public void create () {
		// create batch
		batch = new SpriteBatch();

		// load assets
		Assets.instance.finishLoading();

		// physics world
		physicsWorld = new World(Vector2.Zero, true);

		// create engine and game scene
		engine = new PooledEngine();
		GameScene game = new GameScene(engine, physicsWorld, Assets.instance.sandboxMap);

		// setup main camera
		OrthographicCamera mainCamera = game.getMainCamera();
		viewport = new FitViewport(Constants.VIEWPORT_WIDTH, Constants.VIEWPORT_HEIGHT, mainCamera);

		// systems
		engine.addSystem(new MapSystem(mainCamera, Assets.instance.sandboxMap));
		engine.addSystem(new PlayerSystem(game));
		engine.addSystem(new AimSystem(game));
		engine.addSystem(new CameraSystem(game));
		engine.addSystem(new AnimationSystem());
		engine.addSystem(new EntityRenderSystem(batch, mainCamera));
		engine.addSystem(new PhysicsSystem(physicsWorld));
		if (Settings.instance.debugPhysics)
			engine.addSystem(new PhysicsDebugSystem(physicsWorld, game));
	}

	@Override
	public void render () {
		ScreenUtils.clear(Constants.BACKGROUND_COLOR);
		engine.update(Gdx.graphics.getDeltaTime());
	}

	@Override
	public void resize(int width, int height) {
		Gdx.app.log(TAG, String.format("Resized (%d, %d)", width, height));
		viewport.update(width, height);
	}

	@Override
	public void dispose () {
		batch.dispose();
		Assets.instance.dispose();
		physicsWorld.dispose();
	}
}

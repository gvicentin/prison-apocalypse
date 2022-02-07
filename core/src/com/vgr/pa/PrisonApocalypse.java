package com.vgr.pa;

import com.badlogic.ashley.core.Engine;
import com.badlogic.ashley.core.Entity;
import com.badlogic.ashley.core.PooledEngine;
import com.badlogic.gdx.ApplicationAdapter;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.Camera;
import com.badlogic.gdx.graphics.OrthographicCamera;
import com.badlogic.gdx.graphics.g2d.SpriteBatch;
import com.badlogic.gdx.utils.ScreenUtils;
import com.badlogic.gdx.utils.viewport.FitViewport;
import com.badlogic.gdx.utils.viewport.Viewport;
import com.vgr.pa.asset.Assets;
import com.vgr.pa.core.*;
import com.vgr.pa.player.Player;

public class PrisonApocalypse extends ApplicationAdapter {

	private static final String TAG = PrisonApocalypse.class.getSimpleName();

	private Camera mainCamera;
	private Viewport viewport;

	private SpriteBatch batch;

	private Engine engine;

	@Override
	public void create () {
		mainCamera = new OrthographicCamera();
		viewport = new FitViewport(Constants.VIEWPORT_WIDTH, Constants.VIEWPORT_HEIGHT, mainCamera);

		batch = new SpriteBatch();

		Assets.instance.finishLoading();

		// engine
		engine = new PooledEngine();
		engine.addSystem(new AnimationSystem());
		engine.addSystem(new EntityRenderSystem(batch, mainCamera));

		// entities
		engine.addEntity(new Player());
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
	}
}

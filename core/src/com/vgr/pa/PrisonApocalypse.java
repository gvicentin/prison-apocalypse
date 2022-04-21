package com.vgr.pa;

import com.badlogic.gdx.Game;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.g2d.SpriteBatch;
import com.vgr.pa.asset.Assets;
import com.vgr.pa.screen.MainMenuScreen;

public class PrisonApocalypse extends Game {

	public SpriteBatch batch;

	@Override
	public void create () {
		// set log level
		Gdx.app.setLogLevel(Settings.instance.logLevel);

		// load assets
		Assets.instance.finishLoading();

		// create batch
		batch = new SpriteBatch();

		// start with menu screen
		setScreen(new MainMenuScreen(this));
	}

	@Override
	public void render () {
		super.render();
	}

	@Override
	public void dispose () {
		Assets.instance.dispose();
		batch.dispose();
	}
}

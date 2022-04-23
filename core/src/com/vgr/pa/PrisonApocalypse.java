package com.vgr.pa;

import com.badlogic.gdx.Game;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.Screen;
import com.badlogic.gdx.graphics.g2d.SpriteBatch;
import com.vgr.pa.asset.Assets;
import com.vgr.pa.screen.GameOverScreen;
import com.vgr.pa.screen.GameScreen;
import com.vgr.pa.screen.MainMenuScreen;
import com.vgr.pa.screen.SettingsScreen;

public class PrisonApocalypse extends Game {

	public static final int SCREEN_MAIN_MENU = 0;
	public static final int SCREEN_SETTINGS = 1;
	public static final int SCREEN_GAME = 2;
	public static final int SCREEN_GAME_OVER = 3;

	public SpriteBatch batch;

	private Screen[] screens;

	@Override
	public void create () {
		// set log level
		Gdx.app.setLogLevel(Settings.instance.logLevel);

		// load assets
		Assets.instance.finishLoading();

		// create batch
		batch = new SpriteBatch();

		// create screens
		screens = new Screen[] {
				new MainMenuScreen(this),
				new SettingsScreen(this),
				new GameScreen(this),
				new GameOverScreen(this)
		};

		// start with menu screen
		setScreen(new MainMenuScreen(this));
	}

	public void setScreen(int screen) {
		super.setScreen(screens[screen]);
	}

	public void close() {
		Gdx.app.exit();
	}

	@Override
	public void render () {
		super.render();
	}

	@Override
	public void dispose () {
		for (Screen s : screens) {
			s.dispose();
		}

		Assets.instance.dispose();
		batch.dispose();
	}
}

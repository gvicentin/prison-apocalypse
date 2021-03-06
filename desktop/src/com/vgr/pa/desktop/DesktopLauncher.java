package com.vgr.pa.desktop;

import com.badlogic.gdx.Application;
import com.badlogic.gdx.backends.lwjgl.LwjglApplication;
import com.badlogic.gdx.backends.lwjgl.LwjglApplicationConfiguration;
import com.vgr.pa.Constants;
import com.vgr.pa.PrisonApocalypse;
import com.vgr.pa.Settings;

public class DesktopLauncher {
	public static void main (String[] arg) {
		LwjglApplicationConfiguration config = new LwjglApplicationConfiguration();
		config.title = Constants.WINDOW_TITLE;
		config.width = Constants.WINDOW_WIDTH;
		config.height = Constants.WINDOW_HEIGHT;

		// settings
		Settings.instance.debugPhysics = true;
		Settings.instance.logLevel = Application.LOG_DEBUG;

		new LwjglApplication(new PrisonApocalypse(), config);
	}
}

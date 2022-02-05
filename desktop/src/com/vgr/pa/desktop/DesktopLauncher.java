package com.vgr.pa.desktop;

import com.badlogic.gdx.backends.lwjgl.LwjglApplication;
import com.badlogic.gdx.backends.lwjgl.LwjglApplicationConfiguration;
import com.vgr.pa.PrisonApocalypse;

public class DesktopLauncher {
	public static void main (String[] arg) {
		LwjglApplicationConfiguration config = new LwjglApplicationConfiguration();
		config.title = "Prison Apocalypse";
		config.width = 1280;
		config.height = 720;
		new LwjglApplication(new PrisonApocalypse(), config);
	}
}

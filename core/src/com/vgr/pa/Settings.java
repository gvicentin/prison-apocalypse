package com.vgr.pa;

import com.badlogic.gdx.Application;

public class Settings {

    public static final Settings instance = new Settings();

    public int logLevel = Application.LOG_ERROR;
    public int resolution = 0;
    public boolean fullscreen = false;
    public boolean debugPhysics = false;

    private Settings() {}
}

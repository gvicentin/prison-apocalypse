package com.vgr.pa;

import com.badlogic.gdx.Application;

public class Settings {

    public static final Settings instance = new Settings();

    public boolean debugPhysics = false;
    public int logLevel = Application.LOG_ERROR;
    
    private Settings() {}
}

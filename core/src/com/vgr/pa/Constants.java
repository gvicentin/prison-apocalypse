package com.vgr.pa;

import com.badlogic.gdx.graphics.Color;

public class Constants {

    /* --------------- Window --------------- */
    public static final String WINDOW_TITLE = "Prison Apocalypse";

    public static final int WINDOW_WIDTH = 1024;
    public static final int WINDOW_HEIGHT = 720;

    public static final float VIEWPORT_WIDTH = 8.5f;
    public static final float VIEWPORT_HEIGHT = 6.0f;

    public static final float PIXELS_PER_UNIT = 32.0f;

    /* ------------- Background ------------- */
    public static final Color BACKGROUND_COLOR = Color.BLACK;

    /* --------------- Physics -------------- */
    public static final float PHYSICS_STEP_DELTA = 1 / 60f;
    public static final int VELOCITY_ITERATIONS = 6;
    public static final int POSITION_ITERATIONS = 2;

    public static final short LAYER_ENVIRONMENT = 0x0001;
    public static final short LAYER_PLAYER = 0x0002;
    public static final short LAYER_PLAYER_HIT = 0x0004;
    public static final short LAYER_ENEMY = 0x0008;
    public static final short LAYER_ENEMY_HIT = 0x0010;
    public static final short LAYER_BULLETS = 0x0020;

    /* --------- Systems Priorities --------- */
    public static final int PRIORITY_MAP = 0;
    public static final int PRIORITY_AIM = 100;
    public static final int PRIORITY_PLAYER = 101;
    public static final int PRIORITY_WEAPON = 102;
    public static final int PRIORITY_ENEMY = 103;
    public static final int PRIORITY_CHARACTER = 104;
    public static final int PRIORITY_BULLET = 200;
    public static final int PRIORITY_ANIMATION = 1000;
    public static final int PRIORITY_ENTITY_RENDER = 1001;
    public static final int PRIORITY_PHYSICS_DEBUG = 2000;
    public static final int PRIORITY_PHYSICS = 2001;
    public static final int PRIORITY_CAMERA = 2002;
}

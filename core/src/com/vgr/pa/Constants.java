package com.vgr.pa;

import com.badlogic.gdx.graphics.Color;

public class Constants {

    /* --------------- Window --------------- */
    public static final float VIEWPORT_WIDTH = 16.0f;
    public static final float VIEWPORT_HEIGHT = 9.0f;

    public static final float PIXELS_PER_UNIT = 32.0f;

    /* ------------- Background ------------- */
    public static final Color BACKGROUND_COLOR = Color.BLACK;

    /* ---------- System Priorities ---------- */
    public static final int PRIORITY_MAP = 0;
    public static final int PRIORITY_PLAYER = 100;
    public static final int PRIORITY_CAMERA = 500;
    public static final int PRIORITY_ANIMATION = 1000;
    public static final int PRIORITY_ENTITY_RENDER = 10001;
}

package com.vgr.pa.asset;

import com.badlogic.gdx.graphics.g2d.Animation;
import com.badlogic.gdx.graphics.g2d.TextureRegion;

import java.util.HashMap;

// TODO: fix the values of the animations, now most of them are placeholders
public class Character {

    // Common
    public static final int FRAME_WIDTH = 32;
    public static final int FRAME_HEIGHT = 32;

    /**
     * Policemen
     */

    // idle
    public static final float POLICEMEN_IDLE_DURATION = 0.1f;
    public static final int POLICEMEN_IDLE_START = 0;
    public static final int POLICEMEN_IDLE_END = 0;

    // run
    public static final float POLICEMEN_RUN_DURATION = 0.1f;
    public static final int POLICEMEN_RUN_START = 1;
    public static final int POLICEMEN_RUN_END = 5;

    // hit
    public static final float POLICEMEN_HIT_DURATION = 0.1f;
    public static final int POLICEMEN_HIT_START = 0;
    public static final int POLICEMEN_HIT_END = 0;

    // die
    public static final float POLICEMEN_DIE_DURATION = 0.1f;
    public static final int POLICEMEN_DIE_START = 0;
    public static final int POLICEMEN_DIE_END = 0;

    /** End of Policemen */

    /**
     * Prisoner
     */

    // idle
    public static final float PRISONER_IDLE_DURATION = 0.1f;
    public static final int PRISONER_IDLE_START = 0;
    public static final int PRISONER_IDLE_END = 0;

    // run
    public static final float PRISONER_RUN_DURATION = 0.1f;
    public static final int PRISONER_RUN_START = 1;
    public static final int PRISONER_RUN_END = 4;

    // hit
    public static final float PRISONER_HIT_DURATION = 0.1f;
    public static final int PRISONER_HIT_START = 0;
    public static final int PRISONER_HIT_END = 0;

    // die
    public static final float PRISONER_DIE_DURATION = 0.1f;
    public static final int PRISONER_DIE_START = 0;
    public static final int PRISONER_DIE_END = 0;

    /** End of Prisoner */

    public Animation<TextureRegion> idleAnimation;
    public Animation<TextureRegion> runAnimation;
    public Animation<TextureRegion> hitAnimation;
    public Animation<TextureRegion> dieAnimation;

}

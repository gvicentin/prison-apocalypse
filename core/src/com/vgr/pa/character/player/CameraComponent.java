package com.vgr.pa.character.player;

import com.badlogic.ashley.core.Component;
import com.badlogic.gdx.graphics.OrthographicCamera;
import com.badlogic.gdx.utils.Pool;

public class CameraComponent implements Component, Pool.Poolable {

    public OrthographicCamera camera;

    public float followSpeed = 5.0f;

    // rumble
    public float power;
    public float duration;
    public float timer;

    @Override
    public void reset() {
        camera = null;
        timer = 0f;
        duration = 0f;
        power = 0;
    }

    public void rumble(float duration, float power) {
        this.timer = 0.0f;
        this.duration = duration;
        this.power = power;
    }
}

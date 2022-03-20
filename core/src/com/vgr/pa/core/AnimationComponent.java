package com.vgr.pa.core;

import com.badlogic.ashley.core.Component;
import com.badlogic.gdx.graphics.g2d.Animation;
import com.badlogic.gdx.graphics.g2d.TextureRegion;
import com.badlogic.gdx.utils.IntMap;
import com.badlogic.gdx.utils.Pool;

public class AnimationComponent implements Component, Pool.Poolable {

    public IntMap<Animation<TextureRegion>> animationMap;
    public int currentAnimation;
    public float timer;

    public AnimationComponent() {
        this.animationMap = new IntMap<>();
        reset();
    }

    @Override
    public void reset() {
        animationMap.clear();
        currentAnimation = 0;
        timer = 0;
    }

    public void transition(int nextAnimation) {
        if (nextAnimation != currentAnimation) {
            timer = 0f;
            currentAnimation = nextAnimation;
        }
    }

    public TextureRegion getCurrentFrame() {
        return animationMap.get(currentAnimation).getKeyFrame(timer);
    }

    public boolean isCurrentAnimationFinished() {
        return animationMap.get(currentAnimation).isAnimationFinished(timer);
    }
}

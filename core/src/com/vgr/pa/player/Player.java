package com.vgr.pa.player;

import com.badlogic.ashley.core.Entity;
import com.vgr.pa.asset.Assets;
import com.vgr.pa.core.AnimationComponent;
import com.vgr.pa.core.SpriteComponent;
import com.vgr.pa.core.TransformComponent;

public class Player extends Entity {

    public static final int ANIM_IDLE = 0;
    public static final int ANIM_RUN = 1;
    public static final int ANIM_HIT = 2;
    public static final int ANIM_DIE = 3;

    public Player() {
        createBaseComponents();
        createAnimationComponent();
    }

    private void createBaseComponents() {
        add(new TransformComponent());
        add(new SpriteComponent());
    }

    private void createAnimationComponent() {
        AnimationComponent animationComp = (AnimationComponent) addAndReturn(new AnimationComponent());
        animationComp.animationMap.put(ANIM_IDLE, Assets.instance.prisoner.idleAnimation);
        animationComp.animationMap.put(ANIM_RUN, Assets.instance.prisoner.runAnimation);
        animationComp.animationMap.put(ANIM_HIT, Assets.instance.prisoner.hitAnimation);
        animationComp.animationMap.put(ANIM_DIE, Assets.instance.prisoner.dieAnimation);
        animationComp.transition(ANIM_RUN);
    }
}

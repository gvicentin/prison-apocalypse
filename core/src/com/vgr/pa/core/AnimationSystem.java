package com.vgr.pa.core;

import com.badlogic.ashley.core.ComponentMapper;
import com.badlogic.ashley.core.Entity;
import com.badlogic.ashley.core.Family;
import com.badlogic.ashley.systems.IteratingSystem;

public class AnimationSystem extends IteratingSystem {

    private ComponentMapper<SpriteComponent> spriteMapper;
    private ComponentMapper<AnimationComponent> animationMapper;

    private SpriteComponent spriteComp;
    private AnimationComponent animationComp;

    public AnimationSystem() {
        super(Family.all(SpriteComponent.class, AnimationComponent.class).get());

        this.spriteMapper = ComponentMapper.getFor(SpriteComponent.class);
        this.animationMapper = ComponentMapper.getFor(AnimationComponent.class);
    }

    @Override
    protected void processEntity(Entity entity, float deltaTime) {
        spriteComp = spriteMapper.get(entity);
        animationComp = animationMapper.get(entity);

        // get next frame
        animationComp.timer += deltaTime;
        spriteComp.region = animationComp.getCurrentFrame();
    }
}

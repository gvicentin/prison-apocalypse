package com.vgr.pa.character;

import com.badlogic.ashley.core.ComponentMapper;
import com.badlogic.ashley.core.Entity;
import com.badlogic.ashley.core.Family;
import com.badlogic.ashley.systems.IteratingSystem;
import com.badlogic.gdx.math.MathUtils;
import com.vgr.pa.Constants;
import com.vgr.pa.core.AnimationComponent;
import com.vgr.pa.core.PhysicsComponent;
import com.vgr.pa.core.SpriteComponent;

public class CharacterSystem extends IteratingSystem {

    private ComponentMapper<SpriteComponent> sm;
    private ComponentMapper<AnimationComponent> am;
    private ComponentMapper<PhysicsComponent> pm;
    private ComponentMapper<CharacterComponent> cm;

    public CharacterSystem() {
        super(Family.all(CharacterComponent.class).get(), Constants.PRIORITY_CHARACTER);

        sm = ComponentMapper.getFor(SpriteComponent.class);
        am = ComponentMapper.getFor(AnimationComponent.class);
        pm = ComponentMapper.getFor(PhysicsComponent.class);
        cm = ComponentMapper.getFor(CharacterComponent.class);
    }

    @Override
    protected void processEntity(Entity entity, float deltaTime) {
        SpriteComponent sprite = sm.get(entity);
        AnimationComponent animation = am.get(entity);
        PhysicsComponent physics = pm.get(entity);
        CharacterComponent character = cm.get(entity);

        // update physics
        character.velocity.scl(character.speed);
        physics.body.setLinearVelocity(character.velocity);

        // change running/idle animation
        if (MathUtils.isZero(physics.body.getLinearVelocity().len2())) {
            animation.transition(CharacterComponent.STATE_IDLE);
        } else {
            animation.transition(CharacterComponent.STATE_RUN);
        }

        // flip
        sprite.flipX = character.facingLeft;
    }
}

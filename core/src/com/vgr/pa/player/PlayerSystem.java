package com.vgr.pa.player;

import com.badlogic.ashley.core.ComponentMapper;
import com.badlogic.ashley.core.Entity;
import com.badlogic.ashley.core.EntitySystem;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.Input;
import com.badlogic.gdx.math.MathUtils;
import com.badlogic.gdx.math.Vector2;
import com.vgr.pa.Constants;
import com.vgr.pa.core.AnimationComponent;
import com.vgr.pa.core.PhysicsComponent;
import com.vgr.pa.core.SpriteComponent;
import com.vgr.pa.core.TransformComponent;
import com.vgr.pa.scene.GameScene;

public class PlayerSystem extends EntitySystem {

    // input
    private final Vector2 movementInput;

    // components
    TransformComponent transform;
    SpriteComponent sprite;
    AnimationComponent animation;
    PhysicsComponent physics;
    PlayerComponent player;

    public PlayerSystem(GameScene gameScene) {
        super(Constants.PRIORITY_PLAYER);

        // input
        movementInput = new Vector2();

        // mappers
        Entity player = gameScene.player;
        this.transform = ComponentMapper.getFor(TransformComponent.class).get(player);
        this.sprite = ComponentMapper.getFor(SpriteComponent.class).get(player);
        this.animation = ComponentMapper.getFor(AnimationComponent.class).get(player);
        this.physics = ComponentMapper.getFor(PhysicsComponent.class).get(player);
        this.player = ComponentMapper.getFor(PlayerComponent.class).get(player);
    }

    @Override
    public void update(float deltaTime) {
        Vector2 velocity = getMovementInput();
        velocity.scl(player.speed);
        if (MathUtils.isZero(physics.body.getLinearVelocity().len2())) {
            animation.transition(PlayerComponent.ANIM_IDLE);
        } else {
            if (physics.body.getLinearVelocity().x < 0)
                sprite.flipX = true;
            else
                sprite.flipX = false;

            animation.transition(PlayerComponent.ANIM_RUN);
        }
        physics.body.setLinearVelocity(velocity);
    }

    private Vector2 getMovementInput() {
        movementInput.set(0f, 0f);
        if (Gdx.input.isKeyPressed(Input.Keys.A)) {
            movementInput.x -= 1.0f;
        }
        if (Gdx.input.isKeyPressed(Input.Keys.D)) {
            movementInput.x += 1.0f;
        }
        if (Gdx.input.isKeyPressed(Input.Keys.W)) {
            movementInput.y += 1.0f;
        }
        if (Gdx.input.isKeyPressed(Input.Keys.S)) {
            movementInput.y -= 1.0f;
        }
        return movementInput.nor();
    }
}

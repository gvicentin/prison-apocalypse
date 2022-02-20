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
import com.vgr.pa.scene.GameWorld;

public class PlayerSystem extends EntitySystem {

    // components
    private TransformComponent transform;
    private SpriteComponent sprite;
    private AnimationComponent animation;
    private PhysicsComponent physics;
    private PlayerComponent player;

    // aim
    private TransformComponent aimTransform;

    public PlayerSystem(GameWorld gameScene) {
        super(Constants.PRIORITY_PLAYER);

        // mappers
        Entity player = gameScene.player;
        this.transform = ComponentMapper.getFor(TransformComponent.class).get(player);
        this.sprite = ComponentMapper.getFor(SpriteComponent.class).get(player);
        this.animation = ComponentMapper.getFor(AnimationComponent.class).get(player);
        this.physics = ComponentMapper.getFor(PhysicsComponent.class).get(player);
        this.player = ComponentMapper.getFor(PlayerComponent.class).get(player);
        this.aimTransform = ComponentMapper.getFor(TransformComponent.class).get(gameScene.aim);
    }

    @Override
    public void update(float deltaTime) {
        Vector2 velocity = getMovementInput();
        velocity.scl(player.speed);
        if (MathUtils.isZero(physics.body.getLinearVelocity().len2())) {
            animation.transition(PlayerComponent.ANIM_IDLE);
        } else {
            animation.transition(PlayerComponent.ANIM_RUN);
        }
        physics.body.setLinearVelocity(velocity);

        // flip
        sprite.flipX = aimTransform.position.x - transform.position.x < 0f;
    }

    private Vector2 getMovementInput() {
        Vector2 movementInput = new Vector2();

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

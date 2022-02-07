package com.vgr.pa.player;

import com.badlogic.ashley.core.ComponentMapper;
import com.badlogic.ashley.core.Entity;
import com.badlogic.ashley.core.EntitySystem;
import com.badlogic.ashley.core.Family;
import com.badlogic.ashley.systems.IteratingSystem;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.Input;
import com.badlogic.gdx.math.Vector2;
import com.vgr.pa.Constants;
import com.vgr.pa.core.AnimationComponent;
import com.vgr.pa.core.SpriteComponent;
import com.vgr.pa.core.TransformComponent;

public class PlayerController extends IteratingSystem {

    // input
    private final Vector2 movementInput;

    // Mappers
    private final ComponentMapper<TransformComponent> transformMapper;
    private final ComponentMapper<SpriteComponent> spriteMapper;
    private final ComponentMapper<AnimationComponent> animationMapper;
    private final ComponentMapper<PlayerComponent> playerMapper;

    public PlayerController() {
        super(Family.all(PlayerComponent.class).get(), Constants.PRIORITY_PLAYER);

        // input
        movementInput = new Vector2();

        // mappers
        transformMapper = ComponentMapper.getFor(TransformComponent.class);
        spriteMapper = ComponentMapper.getFor(SpriteComponent.class);
        animationMapper = ComponentMapper.getFor(AnimationComponent.class);
        playerMapper = ComponentMapper.getFor(PlayerComponent.class);
    }

    @Override
    protected void processEntity(Entity entity, float deltaTime) {
        TransformComponent transform = transformMapper.get(entity);
        SpriteComponent sprite = spriteMapper.get(entity);
        AnimationComponent animation = animationMapper.get(entity);
        PlayerComponent player = playerMapper.get(entity);

        Vector2 velocity = getMovementInput();
        velocity.scl(player.speed * deltaTime);

        animation.transition(velocity.len2() > 0 ? Player.ANIM_RUN : Player.ANIM_IDLE);
        sprite.flipX = velocity.x < 0f;

        transform.position.add(velocity);
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

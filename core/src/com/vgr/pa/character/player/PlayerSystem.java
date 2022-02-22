package com.vgr.pa.character.player;

import com.badlogic.ashley.core.ComponentMapper;
import com.badlogic.ashley.core.Entity;
import com.badlogic.ashley.core.EntitySystem;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.Input;
import com.badlogic.gdx.math.Vector2;
import com.vgr.pa.Constants;
import com.vgr.pa.character.CharacterComponent;
import com.vgr.pa.core.TransformComponent;
import com.vgr.pa.world.GameWorld;

public class PlayerSystem extends EntitySystem {

    private Entity player;
    private Entity aim;

    private ComponentMapper<TransformComponent> tm;
    private ComponentMapper<CharacterComponent> cm;

    // aim
    private TransformComponent aimTransform;

    public PlayerSystem(GameWorld gameWorld) {
        super(Constants.PRIORITY_PLAYER);

        // entities
        player = gameWorld.player;
        aim = gameWorld.aim;

        // mappers
        tm = ComponentMapper.getFor(TransformComponent.class);
        cm = ComponentMapper.getFor(CharacterComponent.class);
    }

    @Override
    public void update(float deltaTime) {
        TransformComponent aimTransform = tm.get(aim);
        TransformComponent playerTransform = tm.get(player);
        CharacterComponent character = cm.get(player);
        Vector2 movementInput = new Vector2();

        // get input
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

        // update char info
        character.velocity.set(movementInput.nor());
        character.facingLeft = aimTransform.position.x - playerTransform.position.x < 0f;
    }
}

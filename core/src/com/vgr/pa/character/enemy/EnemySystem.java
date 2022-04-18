package com.vgr.pa.character.enemy;

import com.badlogic.ashley.core.ComponentMapper;
import com.badlogic.ashley.core.Entity;
import com.badlogic.ashley.core.Family;
import com.badlogic.ashley.systems.IteratingSystem;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.math.Vector2;
import com.vgr.pa.Constants;
import com.vgr.pa.character.CharacterComponent;
import com.vgr.pa.core.TransformComponent;
import com.vgr.pa.world.GameWorld;

public class EnemySystem extends IteratingSystem {

    private static final String TAG = EnemySystem.class.getSimpleName();

    private Entity player;

    private ComponentMapper<TransformComponent> tm;
    private ComponentMapper<CharacterComponent> cm;
    private ComponentMapper<EnemyComponent> em;

    public EnemySystem(GameWorld gameWorld) {
        super(Family.all(EnemyComponent.class).get(), Constants.PRIORITY_ENEMY);

        player = gameWorld.player;

        tm = ComponentMapper.getFor(TransformComponent.class);
        cm = ComponentMapper.getFor(CharacterComponent.class);
        em = ComponentMapper.getFor(EnemyComponent.class);
    }

    @Override
    protected void processEntity(Entity enemy, float deltaTime) {
        TransformComponent playerTransform = tm.get(player);
        TransformComponent enemyTransform = tm.get(enemy);
        CharacterComponent character = cm.get(enemy);
        CharacterComponent playerCharacter = cm.get(player);
        EnemyComponent enemyComp = em.get(enemy);

        Vector2 playerToEnemy = new Vector2(playerTransform.position);
        playerToEnemy.sub(enemyTransform.position);

        float thresholdLimit = .4f;
        enemyComp.attackTimer += deltaTime;
        if (playerToEnemy.len2() < thresholdLimit * thresholdLimit) {
            if (enemyComp.attackTimer > enemyComp.attackCoolDown && playerCharacter.health > 0f) {
                playerCharacter.isDamage = true;
                playerCharacter.health -= 25.0f;
                enemyComp.attackTimer = 0.0f;
                Gdx.app.log(TAG, "player life: " + playerCharacter.health);
            }
            character.velocity.set(Vector2.Zero);
        }
        else if (playerToEnemy.len2() < enemyComp.detectRadius * enemyComp.detectRadius) {
            // chase player
            character.velocity.set(playerToEnemy.nor());
        }
        else {
            character.velocity.set(Vector2.Zero);
        }

        // face movement direction
        character.facingLeft = character.velocity.x < 0f;
    }
}

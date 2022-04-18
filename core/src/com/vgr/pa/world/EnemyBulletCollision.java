package com.vgr.pa.world;

import com.badlogic.ashley.core.ComponentMapper;
import com.badlogic.ashley.core.Entity;
import com.badlogic.gdx.Gdx;
import com.vgr.pa.Constants;
import com.vgr.pa.character.CharacterComponent;
import com.vgr.pa.weapon.BulletComponent;

public class EnemyBulletCollision extends BaseCollision {

    private final ComponentMapper<CharacterComponent> cm;
    private final ComponentMapper<BulletComponent> bm;

    public EnemyBulletCollision() {
        super(Constants.LAYER_ENEMY_HIT, Constants.LAYER_BULLETS);

        cm = ComponentMapper.getFor(CharacterComponent.class);
        bm = ComponentMapper.getFor(BulletComponent.class);
    }

    @Override
    public void onCollide(Entity a, short categoryA, Entity b, short categoryB) {
        Entity enemyEntity = getFirstEntity(a, categoryA, b, categoryB);
        Entity bulletEntity = getSecondEntity(a, categoryA, b, categoryB);

        CharacterComponent enemyChar = cm.get(enemyEntity);
        BulletComponent bullet = bm.get(bulletEntity);

        enemyChar.health -= bullet.damage;
        enemyChar.isDamage = true;
        bullet.destroyed = true;
    }
}

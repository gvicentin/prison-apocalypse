package com.vgr.pa.world;

import com.badlogic.ashley.core.ComponentMapper;
import com.badlogic.ashley.core.Entity;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.physics.box2d.*;
import com.badlogic.gdx.utils.IntMap;
import com.vgr.pa.Constants;
import com.vgr.pa.character.CharacterComponent;

public class WorldContact implements ContactListener {

    private static final String TAG = WorldContact.class.getSimpleName();

    private class EnemyBulletCollision implements CollisionDetection {

        @Override
        public void onCollide(Entity a, short categoryA, Entity b, short categoryB) {
            Entity enemy = (categoryA & Constants.LAYER_ENEMY_HIT) != 0 ? a : b;
            Entity bullet = (categoryA & Constants.LAYER_ENEMY_HIT) != 0 ? b : a;

            ComponentMapper<CharacterComponent> cm = ComponentMapper.getFor(CharacterComponent.class);
            CharacterComponent character = cm.get(enemy);
            character.health -= 10f;

            Gdx.app.debug(TAG, "" + character.health);
        }
    }

    private IntMap<CollisionDetection> collisions;

    public WorldContact() {
        collisions = new IntMap<>();
        collisions.put(Constants.LAYER_BULLETS | Constants.LAYER_ENEMY_HIT, new EnemyBulletCollision());
    }

    @Override
    public void beginContact(Contact contact) {
        Fixture a = contact.getFixtureA();
        Fixture b = contact.getFixtureB();
        short catA = a.getFilterData().categoryBits;
        short catB = b.getFilterData().categoryBits;

        CollisionDetection col = collisions.get(catA | catB);
        if (col != null) {
            col.onCollide((Entity) a.getBody().getUserData(), catA, (Entity) b.getBody().getUserData(), catB);
        }
    }

    @Override
    public void endContact(Contact contact) {}

    @Override
    public void preSolve(Contact contact, Manifold oldManifold) {}

    @Override
    public void postSolve(Contact contact, ContactImpulse impulse) {}
}

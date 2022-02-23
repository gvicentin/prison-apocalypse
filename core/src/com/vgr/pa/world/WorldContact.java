package com.vgr.pa.world;

import com.badlogic.ashley.core.Entity;
import com.badlogic.gdx.physics.box2d.*;
import com.badlogic.gdx.utils.IntMap;

public class WorldContact implements ContactListener {

    private static final String TAG = WorldContact.class.getSimpleName();

    private IntMap<CollisionDetection> collisions;

    public WorldContact() {
        EnemyBulletCollision enemyBulletCollision = new EnemyBulletCollision();
        BulletEnvironmentCollision bulletEnvironmentCollision = new BulletEnvironmentCollision();

        collisions = new IntMap<>();
        collisions.put(enemyBulletCollision.getCategoryBits(), enemyBulletCollision);
        collisions.put(bulletEnvironmentCollision.getCategoryBits(), bulletEnvironmentCollision);
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

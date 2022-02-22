package com.vgr.pa.weapon;

import com.badlogic.ashley.core.Entity;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.physics.box2d.BodyDef;
import com.badlogic.gdx.physics.box2d.FixtureDef;
import com.badlogic.gdx.physics.box2d.PolygonShape;
import com.badlogic.gdx.utils.Pool;
import com.vgr.pa.Constants;
import com.vgr.pa.asset.Assets;
import com.vgr.pa.core.PhysicsComponent;
import com.vgr.pa.core.SpriteComponent;
import com.vgr.pa.core.TransformComponent;
import com.vgr.pa.world.GameWorld;

public class BulletPool extends Pool<Entity> {

    private static final String TAG = BulletPool.class.getSimpleName();

    private static final int POOL_SIZE = 100;

    private GameWorld gameWorld;

    public BulletPool(GameWorld gameWorld) {
        super(POOL_SIZE);

        this.gameWorld = gameWorld;

        fill(POOL_SIZE);
        Gdx.app.debug(TAG, "Created Bullets, Pool size: " + POOL_SIZE);
    }

    @Override
    protected Entity newObject() {
        Gdx.app.debug(TAG, "Create bullet");
        Entity bullet = gameWorld.entitiesEngine.createEntity();

        TransformComponent transform = gameWorld.entitiesEngine.createComponent(TransformComponent.class);

        SpriteComponent sprite = gameWorld.entitiesEngine.createComponent(SpriteComponent.class);
        sprite.region = Assets.instance.weapon.pistolBullet;
        sprite.size.set(0.1f, 0.05f);
        sprite.origin.set(0.05f, 0.025f);

        PhysicsComponent physicsComponent = gameWorld.entitiesEngine.createComponent(PhysicsComponent.class);

        // body definition
        BodyDef playerBodyDef = new BodyDef();
        playerBodyDef.type = BodyDef.BodyType.DynamicBody;
        playerBodyDef.bullet = true;

        // collider shape
        PolygonShape boxShape = new PolygonShape();
        boxShape.setAsBox(0.05f, 0.025f);

        // define fixture
        FixtureDef fixtureDef = new FixtureDef();
        fixtureDef.shape = boxShape;
        fixtureDef.isSensor = true;
        fixtureDef.filter.categoryBits = Constants.LAYER_BULLETS;
        fixtureDef.filter.maskBits = Constants.LAYER_ENVIRONMENT |
                Constants.LAYER_PLAYER_HIT | Constants.LAYER_ENEMY_HIT;

        // create body
        physicsComponent.body = gameWorld.physicsWorld.createBody(playerBodyDef);
        physicsComponent.body.createFixture(fixtureDef);
        physicsComponent.body.setUserData(bullet);

        boxShape.dispose();

        BulletComponent bulletComp = gameWorld.entitiesEngine.createComponent(BulletComponent.class);
        bulletComp.isActive = false;

        bullet.add(transform);
        bullet.add(sprite);
        bullet.add(physicsComponent);
        bullet.add(bulletComp);
        gameWorld.entitiesEngine.addEntity(bullet);

        return bullet;
    }

    @Override
    protected void reset(Entity object) { }
}

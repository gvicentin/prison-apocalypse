package com.vgr.pa.weapon;

import com.badlogic.ashley.core.ComponentMapper;
import com.badlogic.ashley.core.Engine;
import com.badlogic.ashley.core.Entity;
import com.badlogic.gdx.graphics.g2d.TextureRegion;
import com.badlogic.gdx.math.Vector2;
import com.badlogic.gdx.physics.box2d.BodyDef;
import com.badlogic.gdx.physics.box2d.FixtureDef;
import com.badlogic.gdx.physics.box2d.PolygonShape;
import com.badlogic.gdx.physics.box2d.World;
import com.vgr.pa.Constants;
import com.vgr.pa.asset.Assets;
import com.vgr.pa.core.PhysicsComponent;
import com.vgr.pa.core.SpriteComponent;
import com.vgr.pa.core.TransformComponent;

public class GunFactory {

    private final Engine engine;
    private final World physicsWorld;

    public OnBulletCreation onCreatePistolBullet;
    private class OnCreatePistolBullet implements OnBulletCreation {

        @Override
        public Entity createBullet() {
            return createPistolBullet();
        }
    }

    public OnBulletCreation onCreateRifleBullet;
    private class OnCreateRifleBullet implements OnBulletCreation {

        @Override
        public Entity createBullet() {
            return createRifleBullet();
        }
    }

    public GunFactory(Engine engine, World physicsWorld) {
        this.engine = engine;
        this.physicsWorld = physicsWorld;
        onCreatePistolBullet = new OnCreatePistolBullet();
        onCreateRifleBullet = new OnCreateRifleBullet();
    }

    public Entity createPistol() {
        Entity pistol = engine.createEntity();

        TransformComponent transform = engine.createComponent(TransformComponent.class);

        SpriteComponent sprite = engine.createComponent(SpriteComponent.class);
        sprite.region = Assets.instance.weapon.pistol;
        sprite.size.set(0.25f, 0.15f);
        sprite.origin.set(0.125f, 0.075f);

        WeaponComponent weapon = engine.createComponent(WeaponComponent.class);
        weapon.offset.set(0f, -0.1f);
        weapon.nuzzle.set(0.2f, 0.02f);
        // rumble
        weapon.duration = 0.08f;
        weapon.power = 0.05f;

        pistol.add(transform);
        pistol.add(sprite);
        pistol.add(weapon);

        return pistol;
    }

    public Entity createRifle() {
        Entity rifle = engine.createEntity();

        TransformComponent transform = engine.createComponent(TransformComponent.class);

        SpriteComponent sprite = engine.createComponent(SpriteComponent.class);
        sprite.region = Assets.instance.weapon.rifle;
        sprite.size.set(0.48f, 0.28f);
        sprite.origin.set(0.15f, 0.14f);

        WeaponComponent weapon = engine.createComponent(WeaponComponent.class);
        weapon.offset.set(0f, -0.05f);
        weapon.nuzzle.set(0.3f, 0f);
        // rumble
        weapon.duration = 0.1f;
        weapon.power = 0.065f;

        rifle.add(transform);
        rifle.add(sprite);
        rifle.add(weapon);

        return rifle;
    }

    public Entity createPistolBullet() {
        Entity bullet = createBaseBullet(Assets.instance.weapon.pistolBullet, new Vector2(0.1f, 0.05f));

        ComponentMapper<BulletComponent> bm = ComponentMapper.getFor(BulletComponent.class);
        BulletComponent bulletComp = bm.get(bullet);
        bulletComp.damage = 5f;
        bulletComp.speed = 10f;
        bulletComp.timeToLive = 5f;

        return bullet;
    }

    public Entity createRifleBullet() {
        Entity bullet = createBaseBullet(Assets.instance.weapon.rifleBullet, new Vector2(0.25f, 0.1f));

        ComponentMapper<BulletComponent> bm = ComponentMapper.getFor(BulletComponent.class);
        BulletComponent bulletComp = bm.get(bullet);
        bulletComp.damage = 10f;
        bulletComp.speed = 16f;
        bulletComp.timeToLive = 8f;

        return bullet;
    }

    protected Entity createBaseBullet(TextureRegion textureRegion, Vector2 size) {
        Vector2 halfSize = new Vector2(size).scl(0.5f);
        Entity bullet = engine.createEntity();

        TransformComponent transform = engine.createComponent(TransformComponent.class);

        SpriteComponent sprite = engine.createComponent(SpriteComponent.class);
        sprite.region = textureRegion;
        sprite.size.set(size);
        sprite.origin.set(halfSize);

        PhysicsComponent physicsComponent = engine.createComponent(PhysicsComponent.class);

        // body definition
        BodyDef playerBodyDef = new BodyDef();
        playerBodyDef.type = BodyDef.BodyType.DynamicBody;
        playerBodyDef.bullet = true;

        // collider shape
        PolygonShape boxShape = new PolygonShape();
        boxShape.setAsBox(halfSize.x, halfSize.y);

        // define fixture
        FixtureDef fixtureDef = new FixtureDef();
        fixtureDef.shape = boxShape;
        fixtureDef.isSensor = true;
        fixtureDef.filter.categoryBits = Constants.LAYER_BULLETS;
        fixtureDef.filter.maskBits = Constants.LAYER_ENVIRONMENT |
                Constants.LAYER_PLAYER_HIT | Constants.LAYER_ENEMY_HIT;

        // create body
        physicsComponent.body = physicsWorld.createBody(playerBodyDef);
        physicsComponent.body.createFixture(fixtureDef);
        physicsComponent.body.setUserData(bullet);

        boxShape.dispose();

        BulletComponent bulletComp = engine.createComponent(BulletComponent.class);
        bulletComp.isActive = false;

        bullet.add(transform);
        bullet.add(sprite);
        bullet.add(physicsComponent);
        bullet.add(bulletComp);
        engine.addEntity(bullet);

        return bullet;
    }
}

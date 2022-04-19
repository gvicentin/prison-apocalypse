package com.vgr.pa.map;

import com.badlogic.ashley.core.Engine;
import com.badlogic.ashley.core.Entity;
import com.badlogic.gdx.graphics.g2d.TextureRegion;
import com.badlogic.gdx.maps.tiled.TiledMap;
import com.badlogic.gdx.math.Rectangle;
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

public class MapFactory {

    private final Engine engine;
    private final World physicsWorld;

    public MapFactory(Engine engine, World physicsWorld) {
        this.engine = engine;
        this.physicsWorld = physicsWorld;
    }

    public Entity createMap(TiledMap tiledMap) {
        Entity map = engine.createEntity();
        MapComponent mapComp = engine.createComponent(MapComponent.class);
        mapComp.map = tiledMap;
        map.add(mapComp);
        engine.addEntity(map);
        return map;
    }

    public Entity createWall(Rectangle rect) {
        Entity wall = engine.createEntity();
        PhysicsComponent physicsComponent = engine.createComponent(PhysicsComponent.class);

        Vector2 boxHalfSize = new Vector2(rect.width / 2f, rect.height / 2f);
        Vector2 boxCenter = new Vector2(rect.x + boxHalfSize.x, rect.y + boxHalfSize.y);

        // body definition
        BodyDef playerBodyDef = new BodyDef();
        playerBodyDef.type = BodyDef.BodyType.StaticBody;
        playerBodyDef.position.set(boxCenter);

        // collider shape
        PolygonShape boxShape = new PolygonShape();
        boxShape.setAsBox(boxHalfSize.x, boxHalfSize.y);

        // define fixture
        FixtureDef fixtureDef = new FixtureDef();
        fixtureDef.shape = boxShape;
        fixtureDef.filter.categoryBits = Constants.LAYER_ENVIRONMENT;
        fixtureDef.filter.maskBits = Constants.LAYER_PLAYER | Constants.LAYER_ENEMY | Constants.LAYER_BULLETS;

        // create body
        physicsComponent.body = physicsWorld.createBody(playerBodyDef);
        physicsComponent.body.createFixture(fixtureDef);

        boxShape.dispose();

        // add wall to engine
        engine.addEntity(wall);

        return wall;
    }

    public void createBarrel(Vector2 position) {
        Entity barrelEntity = createBaseObject(
                Assets.instance.object.barrel,
                position,
                new Vector2(0.5f, 0.75f),
                new Vector2(0.2f, 0.1f),
                new Vector2(0.0f, -0.1f)
        );

        // add barrel to engine
        engine.addEntity(barrelEntity);
    }

    public void createLocker(Vector2 position) {
        Entity barrelEntity = createBaseObject(
                Assets.instance.object.locker,
                position,
                new Vector2(0.6f, 1f),
                new Vector2(0.2f, 0.15f),
                new Vector2(-0.05f, 0.25f)
        );

        // add barrel to engine
        engine.addEntity(barrelEntity);
    }

    private Entity createBaseObject(TextureRegion region, Vector2 position, Vector2 size,
                                    Vector2 colliderSize, Vector2 colliderOffset) {
        Entity objectEntity = engine.createEntity();

        TransformComponent transform = engine.createComponent(TransformComponent.class);
        transform.position.set(position);
        transform.position.add(0f, size.y / 2f);
        objectEntity.add(transform);

        SpriteComponent sprite = engine.createComponent(SpriteComponent.class);
        sprite.region = region;
        sprite.size.set(size);
        sprite.origin.set(size).scl(0.5f);
        objectEntity.add(sprite);

        // physics comp
        PhysicsComponent physics = engine.createComponent(PhysicsComponent.class);
        PolygonShape bodyShape = new PolygonShape();
        bodyShape.setAsBox(colliderSize.x, colliderSize.y);
        BodyDef bodyBodyDef = new BodyDef();
        bodyBodyDef.type = BodyDef.BodyType.StaticBody;
        bodyBodyDef.position.set(position.x + colliderOffset.x, position.y + colliderOffset.y);

        FixtureDef bodyFixtureDef = new FixtureDef();
        bodyFixtureDef.shape = bodyShape;
        bodyFixtureDef.filter.categoryBits = Constants.LAYER_ENVIRONMENT;
        bodyFixtureDef.filter.maskBits = Constants.LAYER_PLAYER | Constants.LAYER_ENEMY;

        physics.body = physicsWorld.createBody(bodyBodyDef);
        physics.body.createFixture(bodyFixtureDef);

        return objectEntity;
    }
}

package com.vgr.pa.character;

import com.badlogic.ashley.core.Engine;
import com.badlogic.ashley.core.Entity;
import com.badlogic.gdx.math.Vector2;
import com.badlogic.gdx.physics.box2d.BodyDef;
import com.badlogic.gdx.physics.box2d.FixtureDef;
import com.badlogic.gdx.physics.box2d.PolygonShape;
import com.badlogic.gdx.physics.box2d.World;
import com.vgr.pa.Constants;
import com.vgr.pa.asset.Assets;
import com.vgr.pa.asset.Character;
import com.vgr.pa.character.enemy.EnemyComponent;
import com.vgr.pa.character.player.PlayerComponent;
import com.vgr.pa.core.AnimationComponent;
import com.vgr.pa.core.PhysicsComponent;
import com.vgr.pa.core.SpriteComponent;
import com.vgr.pa.core.TransformComponent;

public class CharacterFactory {

    private static final int PLAYER = 0;
    private static final int ENEMY = 2;
    private static final int PRISONER = 0;
    private static final int POLICEMEN = 1;

    private final Engine engine;
    private final World physicsWorld;

    private final Character[] charAssets;
    private final short[] bodyCategoryBits;
    private final short[] bodyMaskBits;
    private final short[] hitCategoryBits;
    private final short hitMaskBits;
    private final float[] densities;

    public CharacterFactory(Engine engine, World physicsWorld) {
        this.engine = engine;
        this.physicsWorld = physicsWorld;

        // assets by type
        charAssets = new Character[] {
                Assets.instance.prisoner,
                Assets.instance.policemen,
                Assets.instance.zombiePrisoner,
                Assets.instance.zombiePolicemen
        };

        bodyCategoryBits = new short[] {
                Constants.LAYER_PLAYER,
                Constants.LAYER_ENEMY
        };

        bodyMaskBits = new short[] {
                Constants.LAYER_ENVIRONMENT | Constants.LAYER_ENEMY,
                Constants.LAYER_ENVIRONMENT | Constants.LAYER_PLAYER,
        };

        hitCategoryBits = new short[] {
                Constants.LAYER_PLAYER_HIT,
                Constants.LAYER_ENEMY_HIT
        };

        hitMaskBits = Constants.LAYER_BULLETS;

        densities = new float[] {
                1.0f,
                2.0f
        };
    }

    public Entity createPlayer(Vector2 startPos) {
        Entity playerEntity = createBaseCharacter(PLAYER | PRISONER, startPos, new Vector2(1f, 1f));

        // character component
        CharacterComponent character = engine.createComponent(CharacterComponent.class);
        character.speed = 3.0f;
        character.health = 100.0f;

        // player component
        PlayerComponent player = engine.createComponent(PlayerComponent.class);

        playerEntity.add(character);
        playerEntity.add(player);

        engine.addEntity(playerEntity);
        return playerEntity;
    }

    public Entity createZombiePrisoner(Vector2 startPos, Vector2 size) {
        Entity enemyEntity = createBaseCharacter(ENEMY | PRISONER, startPos, new Vector2(size.x, size.y));

        // character component
        CharacterComponent character = engine.createComponent(CharacterComponent.class);
        character.speed = 1.0f;
        character.health = 100.0f;

        // enemy component
        EnemyComponent enemy = engine.createComponent(EnemyComponent.class);
        enemy.detectRadius = 5f;

        enemyEntity.add(character);
        enemyEntity.add(enemy);

        engine.addEntity(enemyEntity);
        return enemyEntity;
    }

    public Entity createZombiePolicemen(Vector2 startPos, Vector2 size) {
        Entity enemyEntity = createBaseCharacter(ENEMY | POLICEMEN, startPos, new Vector2(size.x, size.y));

        // character component
        CharacterComponent character = engine.createComponent(CharacterComponent.class);
        character.speed = 1.0f;
        character.health = 100.0f;

        // enemy component
        EnemyComponent enemy = engine.createComponent(EnemyComponent.class);
        enemy.detectRadius = 5f;

        enemyEntity.add(character);
        enemyEntity.add(enemy);

        engine.addEntity(enemyEntity);
        return enemyEntity;
    }

    private Entity createBaseCharacter(int type, Vector2 startPos, Vector2 scale) {
        Entity characterEntity = engine.createEntity();

        SpriteComponent sprite = engine.createComponent(SpriteComponent.class);
        TransformComponent transform = engine.createComponent(TransformComponent.class);
        transform.position.set(startPos);
        transform.scale.scl(scale);
        AnimationComponent animation = createAnimationComponent(charAssets[type]);
        PhysicsComponent physics = createPhysicsComponent(type, startPos, scale);

        // add components
        characterEntity.add(sprite);
        characterEntity.add(transform);
        characterEntity.add(animation);
        characterEntity.add(physics);

        // update physics user data
        physics.body.setUserData(characterEntity);
        physics.hitBox.setUserData(characterEntity);

        return characterEntity;
    }

    private AnimationComponent createAnimationComponent(Character charAsset) {
        AnimationComponent animation = engine.createComponent(AnimationComponent.class);
        animation.animationMap.put(CharacterComponent.STATE_IDLE, charAsset.idleAnimation);
        animation.animationMap.put(CharacterComponent.STATE_RUN, charAsset.runAnimation);
        animation.animationMap.put(CharacterComponent.STATE_HIT, charAsset.hitAnimation);
        animation.animationMap.put(CharacterComponent.STATE_DIE, charAsset.dieAnimation);
        return animation;
    }

    private PhysicsComponent createPhysicsComponent(int type, Vector2 startPos, Vector2 scale) {
        PhysicsComponent physics = engine.createComponent(PhysicsComponent.class);

        // body box
        Vector2 halfBoxBody = new Vector2(0.15f, 0.1f);
        halfBoxBody.scl(scale);
        PolygonShape bodyShape = new PolygonShape();
        bodyShape.setAsBox(halfBoxBody.x, halfBoxBody.y);

        // hit box
        Vector2 halfBoxHit = new Vector2(0.15f, 0.25f);
        halfBoxHit.scl(scale);
        PolygonShape hitShape = new PolygonShape();
        hitShape.setAsBox(halfBoxHit.x, halfBoxHit.y);

        // create body
        int index = type >> 1;
        BodyDef bodyBodyDef = createBodyDef(startPos);
        FixtureDef bodyFixtureDef = createFixtureDef(bodyShape,
                densities[index], bodyCategoryBits[index], bodyMaskBits[index]);
        physics.body = physicsWorld.createBody(bodyBodyDef);
        physics.body.createFixture(bodyFixtureDef);
        physics.bodyOffset.set(0f, 0.1f).scl(scale);

        // create hit
        BodyDef hitBodyDef = createBodyDef(startPos);
        FixtureDef hitFixtureDef = createFixtureDef(hitShape,
                densities[index], hitCategoryBits[index], hitMaskBits);
        physics.hitBox = physicsWorld.createBody(hitBodyDef);
        physics.hitBox.createFixture(hitFixtureDef);

        // cleanup shapes
        bodyShape.dispose();
        hitShape.dispose();

        return physics;
    }

    private BodyDef createBodyDef(Vector2 startPos) {
        // body definition
        BodyDef bodyDef = new BodyDef();
        bodyDef.type = BodyDef.BodyType.DynamicBody;
        bodyDef.position.set(startPos);
        bodyDef.fixedRotation = true;
        return bodyDef;
    }

    private FixtureDef createFixtureDef(PolygonShape shape, float density, short categoryBits, short maskBits) {
        // define fixture
        FixtureDef fixtureDef = new FixtureDef();
        fixtureDef.shape = shape;
        fixtureDef.density = density;
        fixtureDef.filter.categoryBits = categoryBits;
        fixtureDef.filter.maskBits = maskBits;
        return fixtureDef;
    }
}

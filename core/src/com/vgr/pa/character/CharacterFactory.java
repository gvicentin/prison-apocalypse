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

    private static final int TYPE_PRISONER = 0;
    private static final int TYPE_POLICEMEN = 1;
    private static final int TYPE_ZOMBIE_PRISONER = 2;
    private static final int TYPE_ZOMBIE_POLICEMEN = 3;

    private Engine engine;
    private World physicsWorld;

    private Character[] charAssets;
    private short[] bodyCategoryBits;
    private short[] bodyMaskBits;
    private short[] hitCategoryBits;
    private short[] hitMaskBits;

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
                Constants.LAYER_PLAYER,
                Constants.LAYER_ENEMY,
                Constants.LAYER_ENEMY
        };

        bodyMaskBits = new short[] {
                Constants.LAYER_ENVIRONMENT | Constants.LAYER_ENEMY,
                Constants.LAYER_ENVIRONMENT | Constants.LAYER_ENEMY,
                Constants.LAYER_ENVIRONMENT | Constants.LAYER_PLAYER,
                Constants.LAYER_ENVIRONMENT | Constants.LAYER_PLAYER,
        };

        hitCategoryBits = new short[] {
                Constants.LAYER_PLAYER_HIT,
                Constants.LAYER_PLAYER_HIT,
                Constants.LAYER_ENEMY_HIT,
                Constants.LAYER_ENEMY_HIT
        };

        hitMaskBits = new short[] {
                Constants.LAYER_BULLETS,
                Constants.LAYER_BULLETS,
                Constants.LAYER_BULLETS,
                Constants.LAYER_BULLETS
        };
    }

    public Entity createBaseCharacter(int type, Vector2 startPos) {
        Entity characterEntity = engine.createEntity();

        SpriteComponent sprite = engine.createComponent(SpriteComponent.class);
        TransformComponent transform = engine.createComponent(TransformComponent.class);
        AnimationComponent animation = createAnimationComponent(charAssets[type]);
        PhysicsComponent physics = createPhysicsComponent(type, startPos);

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

    public Entity createPlayer(Vector2 startPos) {
        Entity playerEntity = createBaseCharacter(TYPE_PRISONER, startPos);

        // character component
        CharacterComponent character = engine.createComponent(CharacterComponent.class);
        character.currentState = CharacterComponent.STATE_IDLE;
        character.speed = 5.0f;
        character.health = 100.0f;

        // player component
        PlayerComponent player = engine.createComponent(PlayerComponent.class);

        playerEntity.add(character);
        playerEntity.add(player);

        return playerEntity;
    }

    public Entity createZombiePolicemen(Vector2 startPos) {
        Entity enemyEntity = createBaseCharacter(TYPE_ZOMBIE_POLICEMEN, startPos);

        // character component
        CharacterComponent character = engine.createComponent(CharacterComponent.class);
        character.currentState = CharacterComponent.STATE_IDLE;
        character.speed = 1.0f;
        character.health = 100.0f;

        // enemy component
        EnemyComponent enemy = engine.createComponent(EnemyComponent.class);
        enemy.detectRadius = 5f;

        enemyEntity.add(character);
        enemyEntity.add(enemy);

        return enemyEntity;
    }

    private AnimationComponent createAnimationComponent(Character charAsset) {
        AnimationComponent animation = engine.createComponent(AnimationComponent.class);
        animation.animationMap.put(CharacterComponent.STATE_IDLE, charAsset.idleAnimation);
        animation.animationMap.put(CharacterComponent.STATE_RUN, charAsset.runAnimation);
        animation.animationMap.put(CharacterComponent.STATE_HIT, charAsset.hitAnimation);
        animation.animationMap.put(CharacterComponent.STATE_DIE, charAsset.dieAnimation);
        return animation;
    }

    private PhysicsComponent createPhysicsComponent(int type, Vector2 startPos) {
        PhysicsComponent physics = engine.createComponent(PhysicsComponent.class);

        // body box
        PolygonShape bodyShape = new PolygonShape();
        bodyShape.setAsBox(0.15f, 0.10f);

        // hit box
        PolygonShape hitShape = new PolygonShape();
        hitShape.setAsBox(0.15f, 0.25f);

        // create body
        BodyDef bodyBodyDef = createBodyDef(startPos);
        FixtureDef bodyFixtureDef = createFixtureDef(bodyShape, bodyCategoryBits[type], bodyMaskBits[type]);
        physics.body = physicsWorld.createBody(bodyBodyDef);
        physics.body.createFixture(bodyFixtureDef);
        physics.bodyOffset.set(0f, 0.1f);

        // create hit
        BodyDef hitBodyDef = createBodyDef(startPos);
        FixtureDef hitFixtureDef = createFixtureDef(hitShape, hitCategoryBits[type], hitMaskBits[type]);
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
        return bodyDef;
    }

    private FixtureDef createFixtureDef(PolygonShape shape, short categoryBits, short maskBits) {
        // define fixture
        FixtureDef fixtureDef = new FixtureDef();
        fixtureDef.shape = shape;
        fixtureDef.filter.categoryBits = categoryBits;
        fixtureDef.filter.maskBits = maskBits;
        return fixtureDef;
    }
}

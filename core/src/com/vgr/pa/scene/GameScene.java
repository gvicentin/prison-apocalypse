package com.vgr.pa.scene;

import com.badlogic.ashley.core.ComponentMapper;
import com.badlogic.ashley.core.Engine;
import com.badlogic.ashley.core.Entity;
import com.badlogic.gdx.graphics.OrthographicCamera;
import com.badlogic.gdx.maps.MapLayer;
import com.badlogic.gdx.maps.MapObjects;
import com.badlogic.gdx.maps.MapProperties;
import com.badlogic.gdx.maps.objects.RectangleMapObject;
import com.badlogic.gdx.maps.tiled.TiledMap;
import com.badlogic.gdx.math.Rectangle;
import com.badlogic.gdx.math.Vector2;
import com.badlogic.gdx.physics.box2d.BodyDef;
import com.badlogic.gdx.physics.box2d.FixtureDef;
import com.badlogic.gdx.physics.box2d.PolygonShape;
import com.badlogic.gdx.physics.box2d.World;
import com.vgr.pa.Constants;
import com.vgr.pa.asset.Assets;
import com.vgr.pa.core.AnimationComponent;
import com.vgr.pa.core.PhysicsComponent;
import com.vgr.pa.core.SpriteComponent;
import com.vgr.pa.core.TransformComponent;
import com.vgr.pa.player.AimComponent;
import com.vgr.pa.player.CameraComponent;
import com.vgr.pa.player.PlayerComponent;

public class GameScene {

    public Entity camera;
    public Entity player;
    public Entity aim;

    public GameScene(Engine engine, World world, TiledMap map) {
        Vector2 playerSpawnPoint = new Vector2();
        createMapEntities(engine, world, map, playerSpawnPoint);

        camera = createCamera(engine);
        engine.addEntity(camera);

        player = createPlayer(engine, world, playerSpawnPoint);
        engine.addEntity(player);

        aim = createAim(engine);
        engine.addEntity(aim);
    }

    public OrthographicCamera getMainCamera() {
        return ComponentMapper.getFor(CameraComponent.class).get(camera).camera;
    }
    private void createMapEntities(Engine engine, World world, TiledMap map, Vector2 playerSpawnPoint) {
        MapLayer wallLayer = map.getLayers().get("collider");
        MapObjects objects = wallLayer.getObjects();

        for (RectangleMapObject rectObj : objects.getByType(RectangleMapObject.class)) {
            Rectangle rect = rectObj.getRectangle();
            Rectangle worldRect = new Rectangle(
                    rect.x / Constants.PIXELS_PER_UNIT,
                    rect.y / Constants.PIXELS_PER_UNIT,
                    rect.width / Constants.PIXELS_PER_UNIT,
                    rect.height / Constants.PIXELS_PER_UNIT
            );
            Entity wall = createWall(engine, world, worldRect);
            engine.addEntity(wall);
        }

        MapProperties properties = map.getProperties();
        int spawn_x = (int) properties.get("player_spawn_x");
        int spawn_y = (int) properties.get("player_spawn_y");
        playerSpawnPoint.set(spawn_x, spawn_y);
    }

    private Entity createWall(Engine engine, World world, Rectangle rect) {
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

        // create body
        physicsComponent.body = world.createBody(playerBodyDef);
        physicsComponent.body.createFixture(fixtureDef);

        boxShape.dispose();

        return wall;
    }

    private Entity createPlayer(Engine engine, World world, Vector2 playerPos) {
        Entity player = engine.createEntity();

        // transform
        TransformComponent transformComp =
                (TransformComponent) player.addAndReturn(engine.createComponent(TransformComponent.class));

        // sprite
        player.add(engine.createComponent(SpriteComponent.class));

        // animation component
        AnimationComponent animationComp =
                (AnimationComponent) player.addAndReturn(engine.createComponent(AnimationComponent.class));
        animationComp.animationMap.put(PlayerComponent.ANIM_IDLE, Assets.instance.prisoner.idleAnimation);
        animationComp.animationMap.put(PlayerComponent.ANIM_RUN, Assets.instance.prisoner.runAnimation);
        animationComp.animationMap.put(PlayerComponent.ANIM_HIT, Assets.instance.prisoner.hitAnimation);
        animationComp.animationMap.put(PlayerComponent.ANIM_DIE, Assets.instance.prisoner.dieAnimation);

        // physics component
        PhysicsComponent physicsComponent = engine.createComponent(PhysicsComponent.class);
        setupPhysicsComponent(physicsComponent, world, playerPos);
        player.add(physicsComponent);

        // player
        player.add(engine.createComponent(PlayerComponent.class));

        return player;
    }

    private void setupPhysicsComponent(PhysicsComponent physicsComponent, World world, Vector2 position) {
        // body definition
        BodyDef playerBodyDef = new BodyDef();
        playerBodyDef.type = BodyDef.BodyType.DynamicBody;
        playerBodyDef.position.set(position);

        // collider shape
        PolygonShape boxShape = new PolygonShape();
        boxShape.setAsBox(0.15f, 0.10f);
        physicsComponent.offset.set(0f, 0.10f);

        // define fixture
        FixtureDef fixtureDef = new FixtureDef();
        fixtureDef.shape = boxShape;

        // create body
        physicsComponent.body = world.createBody(playerBodyDef);
        physicsComponent.body.createFixture(fixtureDef);

        boxShape.dispose();
    }

    private Entity createCamera(Engine engine) {
        Entity camera = engine.createEntity();

        // camera
        CameraComponent cameraComp =
                (CameraComponent) camera.addAndReturn(engine.createComponent(CameraComponent.class));
        cameraComp.camera = new OrthographicCamera();

        return camera;
    }

    private Entity createAim(Engine engine) {
        Entity aim = engine.createEntity();

        AimComponent aimComp = engine.createComponent(AimComponent.class);
        aimComp.texture = Assets.instance.ui.aimOpen;

        SpriteComponent spriteComp = engine.createComponent(SpriteComponent.class);
        spriteComp.size.scl(0.5f);
        spriteComp.origin.scl(0.5f);

        aim.add(engine.createComponent(TransformComponent.class));
        aim.add(spriteComp);
        aim.add(aimComp);

        return aim;
    }
}

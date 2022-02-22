package com.vgr.pa.world;

import com.badlogic.ashley.core.ComponentMapper;
import com.badlogic.ashley.core.Engine;
import com.badlogic.ashley.core.Entity;
import com.badlogic.gdx.graphics.OrthographicCamera;
import com.badlogic.gdx.graphics.g2d.SpriteBatch;
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
import com.vgr.pa.character.CharacterFactory;
import com.vgr.pa.core.PhysicsComponent;
import com.vgr.pa.core.SpriteComponent;
import com.vgr.pa.core.TransformComponent;
import com.vgr.pa.map.MapComponent;
import com.vgr.pa.character.player.AimComponent;
import com.vgr.pa.character.player.CameraComponent;
import com.vgr.pa.weapon.WeaponComponent;

public class GameWorld {

    // entities
    public Entity camera;
    public Entity map;
    public Entity player;
    public Entity aim;

    // gun
    public Entity pistol;

    // game components
    public Engine entitiesEngine;
    public World physicsWorld;
    public SpriteBatch mainBatch;

    // mappers
    private ComponentMapper<CameraComponent> cameraMapper;
    private ComponentMapper<MapComponent> mapMapper;

    public GameWorld(Engine engine, World world, SpriteBatch batch, TiledMap tiledMap) {
        this.entitiesEngine = engine;
        this.physicsWorld = world;
        this.mainBatch = batch;

        // character factory
        CharacterFactory charFactory = new CharacterFactory(engine, world);

        // mappers
        cameraMapper = ComponentMapper.getFor(CameraComponent.class);
        mapMapper = ComponentMapper.getFor(MapComponent.class);

        Vector2 playerSpawnPoint = new Vector2();
        createMapEntities(tiledMap, playerSpawnPoint);

        // create entities
        camera = createCamera();
        map = createMap(tiledMap);
        player = charFactory.createPlayer(playerSpawnPoint);
        aim = createAim();
        pistol = createPistol();

        // add entities to the engine
        entitiesEngine.addEntity(camera);
        entitiesEngine.addEntity(map);
        entitiesEngine.addEntity(player);
        entitiesEngine.addEntity(aim);
        entitiesEngine.addEntity(pistol);

        // add Enemies
        Entity enemy = charFactory.createZombiePolicemen(new Vector2(15f, 20f));
        entitiesEngine.addEntity(enemy);
    }

    public OrthographicCamera getMainCamera() {
        return cameraMapper.get(camera).camera;
    }

    public TiledMap getMap() {
        return mapMapper.get(map).map;
    }

    private Entity createMap(TiledMap tiledMap) {
        Entity map = entitiesEngine.createEntity();

        MapComponent mapComp = entitiesEngine.createComponent(MapComponent.class);
        mapComp.map = tiledMap;

        map.add(mapComp);
        return map;
    }

    private void createMapEntities(TiledMap map, Vector2 playerSpawnPoint) {
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
            Entity wall = createWall(worldRect);
            entitiesEngine.addEntity(wall);
        }

        MapProperties properties = map.getProperties();
        int spawn_x = (int) properties.get("player_spawn_x");
        int spawn_y = (int) properties.get("player_spawn_y");
        playerSpawnPoint.set(spawn_x, spawn_y);
    }

    private Entity createWall(Rectangle rect) {
        Entity wall = entitiesEngine.createEntity();
        PhysicsComponent physicsComponent = entitiesEngine.createComponent(PhysicsComponent.class);

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

        return wall;
    }

    private Entity createCamera() {
        Entity camera = entitiesEngine.createEntity();

        // camera
        CameraComponent cameraComp =
                (CameraComponent) camera.addAndReturn(entitiesEngine.createComponent(CameraComponent.class));
        cameraComp.camera = new OrthographicCamera();

        return camera;
    }

    private Entity createAim() {
        Entity aim = entitiesEngine.createEntity();

        AimComponent aimComp = entitiesEngine.createComponent(AimComponent.class);

        SpriteComponent spriteComp = entitiesEngine.createComponent(SpriteComponent.class);
        spriteComp.region = Assets.instance.ui.aimOpen;
        spriteComp.size.scl(0.5f);
        spriteComp.origin.scl(0.5f);

        aim.add(entitiesEngine.createComponent(TransformComponent.class));
        aim.add(spriteComp);
        aim.add(aimComp);

        return aim;
    }

    private Entity createPistol() {
        Entity pistol = entitiesEngine.createEntity();

        TransformComponent transform = entitiesEngine.createComponent(TransformComponent.class);
        transform.position.set(4f, 20f);
        // transform.rotation = 45f;

        SpriteComponent sprite = entitiesEngine.createComponent(SpriteComponent.class);
        sprite.region = Assets.instance.weapon.pistol;
        sprite.size.set(0.25f, 0.15f);
        sprite.origin.set(0.125f, 0.075f);

        WeaponComponent weapon = entitiesEngine.createComponent(WeaponComponent.class);
        weapon.offset.set(0f, -0.1f);
        weapon.nuzzle.set(0.2f, 0.02f);

        pistol.add(transform);
        pistol.add(sprite);
        pistol.add(weapon);

        return pistol;
    }

}

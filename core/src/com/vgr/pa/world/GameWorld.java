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
import com.vgr.pa.map.MapFactory;
import com.vgr.pa.map.MapLoader;
import com.vgr.pa.weapon.GunFactory;
import com.vgr.pa.weapon.WeaponComponent;

public class GameWorld {

    // entities
    public Entity camera;
    public Entity map;
    public Entity player;
    public Entity aim;

    // gun
    public Entity[] guns;

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

        // factories
        GunFactory gunFactory = new GunFactory(engine, world);
        CharacterFactory charFactory = new CharacterFactory(engine, world);

        // mappers
        cameraMapper = ComponentMapper.getFor(CameraComponent.class);
        mapMapper = ComponentMapper.getFor(MapComponent.class);

        // tiled map
        Vector2 playerSpawnPoint = new Vector2();
        MapFactory mapFactory = new MapFactory(engine, world);
        MapLoader mapLoader = new MapLoader(mapFactory);


        // create entities
        camera = createCamera();
        map = mapLoader.loadTiledMap(tiledMap, playerSpawnPoint);
        player = charFactory.createPlayer(playerSpawnPoint);
        aim = createAim();
        guns = new Entity[] {
          gunFactory.createPistol(),
          gunFactory.createRifle()
        };

        // add entities to the engine
        entitiesEngine.addEntity(camera);
        entitiesEngine.addEntity(player);
        entitiesEngine.addEntity(aim);

        for (Entity gun : guns) {
            entitiesEngine.addEntity(gun);
        }

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
}

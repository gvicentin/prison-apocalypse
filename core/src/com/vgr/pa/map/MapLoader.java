package com.vgr.pa.map;

import com.badlogic.ashley.core.Entity;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.maps.MapLayer;
import com.badlogic.gdx.maps.MapObjects;
import com.badlogic.gdx.maps.MapProperties;
import com.badlogic.gdx.maps.objects.RectangleMapObject;
import com.badlogic.gdx.maps.tiled.TiledMap;
import com.badlogic.gdx.math.Rectangle;
import com.badlogic.gdx.math.Vector2;
import com.vgr.pa.Constants;
import com.vgr.pa.character.CharacterFactory;

public class MapLoader {

    private static final String TAG = MapLoader.class.getSimpleName();

    private static final int TYPE_BARREL = 0;
    private static final int TYPE_LOCKER = 1;

    private static final int TYPE_PRISONER = 0;
    private static final int TYPE_POLICEMEN = 1;

    private final MapFactory mapFactory;
    private final CharacterFactory charFactory;

    public MapLoader(MapFactory mapFactory, CharacterFactory charFactory) {
        this.mapFactory = mapFactory;
        this.charFactory = charFactory;
    }

    public Entity loadTiledMap(TiledMap tiledMap, Vector2 playerSpawnPoint) {
        // create map
        Entity map = mapFactory.createMap(tiledMap);

        // create map entities
        createWalls(tiledMap);
        createObjects(tiledMap);
        createEnemies(tiledMap);

        // load player spawn point
        MapProperties properties = tiledMap.getProperties();
        int spawn_x = (int) properties.get("player_spawn_x");
        int spawn_y = (int) properties.get("player_spawn_y");
        playerSpawnPoint.set(spawn_x, spawn_y);

        return map;
    }

    private void createWalls(TiledMap tiledMap) {
        MapLayer wallLayer = tiledMap.getLayers().get("collider");
        MapObjects objects = wallLayer.getObjects();

        for (RectangleMapObject rectObj : objects.getByType(RectangleMapObject.class)) {
            Rectangle rect = rectObj.getRectangle();
            Rectangle worldRect = new Rectangle(
                    rect.x / Constants.PIXELS_PER_UNIT,
                    rect.y / Constants.PIXELS_PER_UNIT,
                    rect.width / Constants.PIXELS_PER_UNIT,
                    rect.height / Constants.PIXELS_PER_UNIT
            );
            mapFactory.createWall(worldRect);
        }
    }

    private void createObjects(TiledMap tiledMap) {
        MapLayer objectLayer = tiledMap.getLayers().get("object");
        MapObjects objects = objectLayer.getObjects();

        for (RectangleMapObject rectObj : objects.getByType(RectangleMapObject.class)) {
            int type = (Integer) rectObj.getProperties().get("type");
            Gdx.app.log(TAG, "Creating map object, type: " + type);
            Rectangle rect = rectObj.getRectangle();
            Vector2 center = new Vector2(rect.x, rect.y);
            center.scl(1f / Constants.PIXELS_PER_UNIT);

            switch (type) {
                case TYPE_BARREL:
                    mapFactory.createBarrel(center);
                    break;
                case TYPE_LOCKER:
                    mapFactory.createLocker(center);
                    break;
            }
        }
    }

    private void createEnemies(TiledMap tiledMap) {
        MapLayer enemyLayer = tiledMap.getLayers().get("enemy");
        MapObjects enemies = enemyLayer.getObjects();

        for (RectangleMapObject rectObj : enemies.getByType(RectangleMapObject.class)) {
            int type = (Integer) rectObj.getProperties().get("type");
            float size = (Float) rectObj.getProperties().get("size");
            Gdx.app.log(TAG, "Creating enemy, type: " + type);
            Rectangle rect = rectObj.getRectangle();
            Vector2 position = new Vector2(rect.x, rect.y);
            position.scl(1f / Constants.PIXELS_PER_UNIT);

            switch (type) {
                case TYPE_PRISONER:
                    charFactory.createZombiePrisoner(position, new Vector2(size, size));
                    break;
                case TYPE_POLICEMEN:
                    charFactory.createZombiePolicemen(position, new Vector2(size, size));
                    break;
            }
        }
    }
}

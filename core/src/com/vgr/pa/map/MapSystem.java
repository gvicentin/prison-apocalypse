package com.vgr.pa.map;

import com.badlogic.ashley.core.EntitySystem;
import com.badlogic.gdx.graphics.OrthographicCamera;
import com.badlogic.gdx.maps.tiled.TiledMap;
import com.badlogic.gdx.maps.tiled.TiledMapRenderer;
import com.badlogic.gdx.maps.tiled.renderers.OrthogonalTiledMapRenderer;
import com.vgr.pa.Constants;
import com.vgr.pa.scene.GameWorld;

public class MapSystem extends EntitySystem {

    private final OrthographicCamera camera;
    private final TiledMap map;
    private final TiledMapRenderer mapRenderer;

    public MapSystem(GameWorld gameWorld) {
        super(Constants.PRIORITY_MAP);

        this.camera = gameWorld.getMainCamera();
        this.map = gameWorld.getMap();
        this.mapRenderer = new OrthogonalTiledMapRenderer(map, 1f / Constants.PIXELS_PER_UNIT);
    }

    @Override
    public void update(float deltaTime) {
        mapRenderer.setView(camera);
        mapRenderer.render();
    }
}

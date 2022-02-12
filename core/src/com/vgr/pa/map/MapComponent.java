package com.vgr.pa.map;

import com.badlogic.ashley.core.Component;
import com.badlogic.gdx.maps.tiled.TiledMap;
import com.badlogic.gdx.utils.Pool;

public class MapComponent implements Component, Pool.Poolable {

    public TiledMap map;

    @Override
    public void reset() {
        map = null;
    }
}

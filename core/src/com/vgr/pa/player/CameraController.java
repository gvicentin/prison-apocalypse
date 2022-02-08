package com.vgr.pa.player;

import com.badlogic.ashley.core.*;
import com.badlogic.gdx.graphics.OrthographicCamera;
import com.vgr.pa.Constants;
import com.vgr.pa.core.TransformComponent;

public class CameraController extends EntitySystem {

    private OrthographicCamera camera;

    private TransformComponent target;

    public CameraController() {
        super(Constants.PRIORITY_CAMERA);
    }

    @Override
    public void addedToEngine(Engine engine) {
        // get player as target
        Family targetFamily = Family.all(PlayerComponent.class).get();
        Entity target = engine.getEntitiesFor(targetFamily).get(0);
        this.target = ComponentMapper.getFor(TransformComponent.class).get(target);

        // get camera
        Family cameraFamily = Family.all(CameraComponent.class).get();
        Entity camera = engine.getEntitiesFor(cameraFamily).get(0);
        this.camera = ComponentMapper.getFor(CameraComponent.class).get(camera).camera;
    }

    @Override
    public void update(float deltaTime) {
        camera.position.set(target.position, 0f);
        camera.update();
    }
}

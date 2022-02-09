package com.vgr.pa.player;

import com.badlogic.ashley.core.*;
import com.badlogic.gdx.graphics.OrthographicCamera;
import com.vgr.pa.Constants;
import com.vgr.pa.core.TransformComponent;
import com.vgr.pa.scene.GameScene;

public class CameraSystem extends EntitySystem {

    private OrthographicCamera camera;

    private TransformComponent target;

    public CameraSystem(GameScene gameScene) {
        super(Constants.PRIORITY_CAMERA);

        this.camera = gameScene.getMainCamera();

        target = ComponentMapper.getFor(TransformComponent.class).get(gameScene.player);
    }

    @Override
    public void update(float deltaTime) {
        camera.position.set(target.position, 0f);
        camera.update();
    }
}

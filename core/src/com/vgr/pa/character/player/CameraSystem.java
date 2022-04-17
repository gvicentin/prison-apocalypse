package com.vgr.pa.character.player;

import com.badlogic.ashley.core.*;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.OrthographicCamera;
import com.badlogic.gdx.math.MathUtils;
import com.badlogic.gdx.math.Vector3;
import com.vgr.pa.Constants;
import com.vgr.pa.core.TransformComponent;
import com.vgr.pa.world.GameWorld;

public class CameraSystem extends EntitySystem {

    private static final String TAG = CameraSystem.class.getSimpleName();

    private CameraComponent camera;

    // player
    private TransformComponent playerTransform;
    private PlayerComponent player;

    private OrthographicCamera mainCamera;

    public CameraSystem(GameWorld gameScene) {
        super(Constants.PRIORITY_CAMERA);

        this.camera = ComponentMapper.getFor(CameraComponent.class).get(gameScene.camera);
        this.playerTransform = ComponentMapper.getFor(TransformComponent.class).get(gameScene.player);
        this.player = ComponentMapper.getFor(PlayerComponent.class).get(gameScene.player);

        // camera initial pos
        Gdx.app.log(TAG, "target: " + this.playerTransform.position);
        this.mainCamera = camera.camera;
        this.mainCamera.position.set(this.playerTransform.position, 0f);
        this.mainCamera.update();
    }

    @Override
    public void update(float deltaTime) {
        if (camera.timer < camera.duration) {
            float power = camera.power * (camera.duration - camera.timer) / camera.duration;
            float x = MathUtils.random(-1f, 1f) * power;
            float y = MathUtils.random(-1f, 1f) * power;
            mainCamera.position.add(x, y, 0f);
            camera.timer += deltaTime;
        }
        mainCamera.position.lerp(new Vector3(playerTransform.position, 0f), camera.followSpeed * deltaTime);
        mainCamera.update();
    }
}

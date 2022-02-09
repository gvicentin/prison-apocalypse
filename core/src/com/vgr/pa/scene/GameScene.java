package com.vgr.pa.scene;

import com.badlogic.ashley.core.ComponentMapper;
import com.badlogic.ashley.core.Engine;
import com.badlogic.ashley.core.Entity;
import com.badlogic.gdx.graphics.OrthographicCamera;
import com.vgr.pa.asset.Assets;
import com.vgr.pa.core.AnimationComponent;
import com.vgr.pa.core.SpriteComponent;
import com.vgr.pa.core.TransformComponent;
import com.vgr.pa.player.CameraComponent;
import com.vgr.pa.player.PlayerComponent;

public class GameScene {

    public Entity player;
    public Entity camera;

    public GameScene(Engine engine) {
        player = createPlayer(engine);
        engine.addEntity(player);

        camera = createCamera(engine);
        engine.addEntity(camera);
    }

    public OrthographicCamera getMainCamera() {
        return ComponentMapper.getFor(CameraComponent.class).get(camera).camera;
    }

    private Entity createPlayer(Engine engine) {
        Entity player = engine.createEntity();

        // transform
        TransformComponent transformComp =
                (TransformComponent) player.addAndReturn(engine.createComponent(TransformComponent.class));
        transformComp.position.set(20f, 20f);

        // sprite
        player.add(engine.createComponent(SpriteComponent.class));

        // animation component
        AnimationComponent animationComp =
                (AnimationComponent) player.addAndReturn(engine.createComponent(AnimationComponent.class));
        animationComp.animationMap.put(PlayerComponent.ANIM_IDLE, Assets.instance.prisoner.idleAnimation);
        animationComp.animationMap.put(PlayerComponent.ANIM_RUN, Assets.instance.prisoner.runAnimation);
        animationComp.animationMap.put(PlayerComponent.ANIM_HIT, Assets.instance.prisoner.hitAnimation);
        animationComp.animationMap.put(PlayerComponent.ANIM_DIE, Assets.instance.prisoner.dieAnimation);

        // player
        player.add(engine.createComponent(PlayerComponent.class));

        return player;
    }

    private Entity createCamera(Engine engine) {
        Entity camera = engine.createEntity();

        // camera
        CameraComponent cameraComp =
                (CameraComponent) camera.addAndReturn(engine.createComponent(CameraComponent.class));
        cameraComp.camera = new OrthographicCamera();

        return camera;
    }
}

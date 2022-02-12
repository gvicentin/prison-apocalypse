package com.vgr.pa.player;

import com.badlogic.ashley.core.ComponentMapper;
import com.badlogic.ashley.core.Entity;
import com.badlogic.ashley.core.EntitySystem;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.OrthographicCamera;
import com.badlogic.gdx.math.Vector3;
import com.vgr.pa.Constants;
import com.vgr.pa.core.SpriteComponent;
import com.vgr.pa.core.TransformComponent;
import com.vgr.pa.scene.GameScene;

public class AimSystem extends EntitySystem {

    // aim
    private AimComponent aim;
    private TransformComponent aimTransform;
    private SpriteComponent aimSprite;

    // player
    private TransformComponent playerTransform;
    private SpriteComponent playerSprite;

    private OrthographicCamera camera;
    private Vector3 mousePosition;

    public AimSystem(GameScene game) {
        super(Constants.PRIORITY_AIM);

        camera = game.getMainCamera();
        mousePosition = new Vector3();

        Entity aimEntity = game.aim;
        Entity playerEntity = game.player;

        ComponentMapper<TransformComponent> tm = ComponentMapper.getFor(TransformComponent.class);
        ComponentMapper<SpriteComponent> sm = ComponentMapper.getFor(SpriteComponent.class);
        ComponentMapper<AimComponent> am = ComponentMapper.getFor(AimComponent.class);

        aim = am.get(aimEntity);
        aimTransform = tm.get(aimEntity);
        aimSprite = sm.get(aimEntity);
        playerTransform = tm.get(playerEntity);
        playerSprite = sm.get(playerEntity);
    }

    @Override
    public void update(float deltaTime) {
        mousePosition.set(Gdx.input.getX(), Gdx.input.getY(), 0f);
        Vector3 aimPosition = camera.unproject(mousePosition);
        aimTransform.position.set(aimPosition.x, aimPosition.y);

        playerSprite.flipX = aimPosition.x - playerTransform.position.x < 0f;

        // update aim texture
        aimSprite.region = aim.texture;
    }
}

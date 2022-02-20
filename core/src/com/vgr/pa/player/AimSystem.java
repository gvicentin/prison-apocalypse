package com.vgr.pa.player;

import com.badlogic.ashley.core.ComponentMapper;
import com.badlogic.ashley.core.Entity;
import com.badlogic.ashley.core.EntitySystem;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.OrthographicCamera;
import com.badlogic.gdx.math.Vector3;
import com.vgr.pa.Constants;
import com.vgr.pa.core.TransformComponent;
import com.vgr.pa.scene.GameWorld;

public class AimSystem extends EntitySystem {

    private TransformComponent aimTransform;

    private OrthographicCamera camera;
    private Vector3 mousePosition;

    public AimSystem(GameWorld game) {
        super(Constants.PRIORITY_AIM);

        camera = game.getMainCamera();
        mousePosition = new Vector3();

        Entity aimEntity = game.aim;

        ComponentMapper<TransformComponent> tm = ComponentMapper.getFor(TransformComponent.class);
        ComponentMapper<AimComponent> am = ComponentMapper.getFor(AimComponent.class);

        aimTransform = tm.get(aimEntity);
    }

    @Override
    public void update(float deltaTime) {
        // un project
        mousePosition.set(Gdx.input.getX(), Gdx.input.getY(), 0f);
        Vector3 aimPosition = camera.unproject(mousePosition);

        // update aim position
        aimTransform.position.set(aimPosition.x, aimPosition.y);
    }
}

package com.vgr.pa.weapon;

import com.badlogic.ashley.core.ComponentMapper;
import com.badlogic.ashley.core.Entity;
import com.badlogic.ashley.core.EntitySystem;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.math.Vector2;
import com.vgr.pa.Constants;
import com.vgr.pa.character.player.CameraComponent;
import com.vgr.pa.character.player.PlayerComponent;
import com.vgr.pa.core.SpriteComponent;
import com.vgr.pa.core.TransformComponent;
import com.vgr.pa.world.GameWorld;

public class WeaponSystem extends EntitySystem {

    private Entity player;
    private Entity aim;
    private Entity camera;

    private BulletSystem bulletSystem;

    private ComponentMapper<TransformComponent> tm;
    private ComponentMapper<WeaponComponent> wm;
    private ComponentMapper<SpriteComponent> sm;
    private ComponentMapper<PlayerComponent> pm;
    private ComponentMapper<CameraComponent> cm;

    public WeaponSystem(GameWorld gameWorld, BulletSystem bulletSystem) {
        super(Constants.PRIORITY_WEAPON);

        this.bulletSystem = bulletSystem;

        player = gameWorld.player;
        aim = gameWorld.aim;
        camera = gameWorld.camera;

        tm = ComponentMapper.getFor(TransformComponent.class);
        wm = ComponentMapper.getFor(WeaponComponent.class);
        sm = ComponentMapper.getFor(SpriteComponent.class);
        pm = ComponentMapper.getFor(PlayerComponent.class);
        cm = ComponentMapper.getFor(CameraComponent.class);
    }

    @Override
    public void update(float deltaTime) {
        TransformComponent playerTransform = tm.get(player);
        TransformComponent aimTransform = tm.get(aim);
        PlayerComponent playerComp = pm.get(player);

        if (playerComp.currentGun == null) {
            return;
        }

        TransformComponent weaponTransform = tm.get(playerComp.currentGun);
        WeaponComponent weaponComp = wm.get(playerComp.currentGun);
        CameraComponent cameraComp = cm.get(camera);

        // calculate aim angle
        Vector2 aimToPlayer = new Vector2(aimTransform.position);
        aimToPlayer.sub(playerTransform.position);
        aimToPlayer.nor();

        // fire weapon
        weaponComp.coolDownTimer += deltaTime;
        if (Gdx.input.isTouched() && weaponComp.coolDownTimer > weaponComp.coolDown) {
            weaponComp.coolDownTimer = 0f;

            Vector2 bulletPos = new Vector2(weaponComp.nuzzle);
            bulletPos.y = aimToPlayer.x < 0f ? -bulletPos.y : bulletPos.y;
            bulletPos.rotateRad(aimToPlayer.angleRad());
            bulletPos.add(playerTransform.position);
            bulletPos.add(weaponComp.offset);
            bulletSystem.spawn(playerComp.gunSelectionIndex, bulletPos, aimToPlayer.angleRad());
            cameraComp.rumble(weaponComp.duration, weaponComp.power);
        }

        // flip weapon sprite
        SpriteComponent sprite = sm.get(playerComp.currentGun);
        sprite.flipY = aimToPlayer.x < 0f;

        // update transform
        weaponTransform.position.set(playerTransform.position);
        weaponTransform.position.add(weaponComp.offset);
        weaponTransform.rotation = aimToPlayer.angleDeg();
    }
}

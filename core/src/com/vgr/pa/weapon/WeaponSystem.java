package com.vgr.pa.weapon;

import com.badlogic.ashley.core.ComponentMapper;
import com.badlogic.ashley.core.Entity;
import com.badlogic.ashley.core.EntitySystem;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.math.MathUtils;
import com.badlogic.gdx.math.Vector2;
import com.vgr.pa.Constants;
import com.vgr.pa.core.SpriteComponent;
import com.vgr.pa.core.TransformComponent;
import com.vgr.pa.scene.GameWorld;

public class WeaponSystem extends EntitySystem {

    private static final String TAG = WeaponSystem.class.getSimpleName();

    private Entity player;
    private Entity weapon;
    private Entity aim;

    private BulletSystem bulletSystem;

    private ComponentMapper<TransformComponent> tm;
    private ComponentMapper<WeaponComponent> wm;
    private ComponentMapper<SpriteComponent> sm;

    public WeaponSystem(GameWorld gameWorld, BulletSystem bulletSystem) {
        super(Constants.PRIORITY_WEAPON);

        this.bulletSystem = bulletSystem;

        player = gameWorld.player;
        weapon = gameWorld.pistol;
        aim = gameWorld.aim;
        tm = ComponentMapper.getFor(TransformComponent.class);
        wm = ComponentMapper.getFor(WeaponComponent.class);
        sm = ComponentMapper.getFor(SpriteComponent.class);
    }

    @Override
    public void update(float deltaTime) {
        TransformComponent playerTransform = tm.get(player);
        TransformComponent weaponTransform = tm.get(weapon);
        TransformComponent aimTransform = tm.get(aim);
        WeaponComponent weaponComp = wm.get(weapon);

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
            bulletSystem.spawn(bulletPos, aimToPlayer.angleRad());
        }

        // flip weapon sprite
        SpriteComponent sprite = sm.get(weapon);
        sprite.flipY = aimToPlayer.x < 0f;

        // update transform
        weaponTransform.position.set(playerTransform.position);
        weaponTransform.position.add(weaponComp.offset);
        weaponTransform.rotation = aimToPlayer.angleDeg();
    }
}

package com.vgr.pa.core;

import com.badlogic.ashley.core.ComponentMapper;
import com.badlogic.ashley.core.Entity;

import java.util.Comparator;

public class ZComparator implements Comparator<Entity> {

    private ComponentMapper<SpriteComponent> spriteMapper;
    private ComponentMapper<TransformComponent> transformMapper;


    public ZComparator() {
        this.spriteMapper = ComponentMapper.getFor(SpriteComponent.class);
        this.transformMapper = ComponentMapper.getFor(TransformComponent.class);
    }

    @Override
    public int compare(Entity e1, Entity e2) {
        SpriteComponent sprite1 = spriteMapper.get(e1);
        SpriteComponent sprite2 = spriteMapper.get(e2);

        return (int) Math.signum(sprite1.zIndex - sprite2.zIndex);
    }

    private int compareYSort(Entity e1, Entity e2) {
        SpriteComponent sprite1 = spriteMapper.get(e1);
        SpriteComponent sprite2 = spriteMapper.get(e2);
        TransformComponent transform1 = transformMapper.get(e1);
        TransformComponent transform2 = transformMapper.get(e2);

        return (int) (-1f * Math.signum((transform1.position.y - sprite1.size.y / 2f) -
                (transform2.position.y - sprite2.size.y / 2f)));
    }
}

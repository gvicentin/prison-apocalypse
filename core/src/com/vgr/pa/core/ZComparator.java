package com.vgr.pa.core;

import com.badlogic.ashley.core.ComponentMapper;
import com.badlogic.ashley.core.Entity;

import java.util.Comparator;

public class ZComparator implements Comparator<Entity> {

    private ComponentMapper<SpriteComponent> spriteMapper;

    // entity components
    private SpriteComponent spriteComp1;
    private SpriteComponent spriteComp2;

    public ZComparator() {
        this.spriteMapper = ComponentMapper.getFor(SpriteComponent.class);
    }

    @Override
    public int compare(Entity e1, Entity e2) {
        spriteComp1 = spriteMapper.get(e1);
        spriteComp2 = spriteMapper.get(e2);
        return (int) Math.signum(spriteComp1.zIndex - spriteComp2.zIndex);
    }
}

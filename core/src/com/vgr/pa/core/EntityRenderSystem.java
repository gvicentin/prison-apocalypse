package com.vgr.pa.core;

import com.badlogic.ashley.core.ComponentMapper;
import com.badlogic.ashley.core.Entity;
import com.badlogic.ashley.core.Family;
import com.badlogic.ashley.systems.SortedIteratingSystem;
import com.badlogic.gdx.graphics.OrthographicCamera;
import com.badlogic.gdx.graphics.g2d.SpriteBatch;
import com.badlogic.gdx.math.Affine2;
import com.badlogic.gdx.math.Vector2;
import com.vgr.pa.Constants;
import com.vgr.pa.scene.GameWorld;

public class EntityRenderSystem extends SortedIteratingSystem {

    private SpriteBatch batch;
    private OrthographicCamera camera;

    // mappers
    private ComponentMapper<SpriteComponent> texMapper;
    private ComponentMapper<TransformComponent> transMapper;

    // process entity variables
    private SpriteComponent spriteComp;
    private TransformComponent transComp;
    private Affine2 transform;
    private Vector2 texScale;
    private Vector2 texCenterTranslation;

    public EntityRenderSystem(GameWorld gameWorld) {
        super(Family.all(SpriteComponent.class, TransformComponent.class).get(),
                new ZComparator(), Constants.PRIORITY_ENTITY_RENDER);

        this.batch = gameWorld.mainBatch;
        this.camera = gameWorld.getMainCamera();
        this.texMapper = ComponentMapper.getFor(SpriteComponent.class);
        this.transMapper = ComponentMapper.getFor(TransformComponent.class);

        this.transform = new Affine2();
        this.texScale = new Vector2();
        this.texCenterTranslation = new Vector2();
    }

    @Override
    public void update(float deltaTime) {
        batch.setProjectionMatrix(camera.combined);
        batch.begin();
        super.update(deltaTime);
        batch.end();
    }

    @Override
    protected void processEntity(Entity entity, float deltaTime) {
        spriteComp = texMapper.get(entity);
        transComp = transMapper.get(entity);

        // transform
        transform.idt();
        transform.translate(transComp.position);

        // scale considering texture flipping
        texScale.set(transComp.scale);
        texScale.x = spriteComp.flipX ? -texScale.x : texScale.x;
        texScale.y = spriteComp.flipY ? -texScale.y : texScale.y;
        transform.scale(texScale);

        transform.rotate(transComp.rotation);

        // place origin in the middle of coordinate system
        texCenterTranslation.set(spriteComp.origin).scl(-1.0f);
        transform.translate(texCenterTranslation);

        batch.draw(spriteComp.region, spriteComp.size.x, spriteComp.size.y, transform);
    }
}

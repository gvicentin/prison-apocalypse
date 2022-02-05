package com.vgr.pa;

import com.badlogic.gdx.ApplicationAdapter;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.Camera;
import com.badlogic.gdx.graphics.OrthographicCamera;
import com.badlogic.gdx.graphics.g2d.Animation;
import com.badlogic.gdx.graphics.g2d.SpriteBatch;
import com.badlogic.gdx.graphics.g2d.TextureRegion;
import com.badlogic.gdx.utils.ScreenUtils;
import com.badlogic.gdx.utils.viewport.FitViewport;
import com.badlogic.gdx.utils.viewport.Viewport;
import com.vgr.pa.asset.Assets;

public class PrisonApocalypse extends ApplicationAdapter {

	private static final String TAG = PrisonApocalypse.class.getSimpleName();

	private Camera mainCamera;
	private Viewport viewport;

	private SpriteBatch batch;

	private Animation<TextureRegion> anim;
	private float elapsed = 0f;

	@Override
	public void create () {
		mainCamera = new OrthographicCamera();
		viewport = new FitViewport(Constants.VIEWPORT_WIDTH, Constants.VIEWPORT_HEIGHT, mainCamera);

		batch = new SpriteBatch();

		Assets.instance.finishLoading();
		anim = Assets.instance.prisoner.animationsMap.get("run");
	}

	@Override
	public void render () {
		ScreenUtils.clear(Constants.BACKGROUND_COLOR);

		batch.setProjectionMatrix(mainCamera.combined);
		batch.setTransformMatrix(mainCamera.view);

		elapsed += Gdx.graphics.getDeltaTime();
		TextureRegion currentFrame = anim.getKeyFrame(elapsed);

		batch.begin();
		batch.draw(currentFrame, 0, 0, 1, 1);
		batch.end();
	}

	@Override
	public void resize(int width, int height) {
		Gdx.app.log(TAG, String.format("Resized (%d, %d)", width, height));
		viewport.update(width, height);
	}

	@Override
	public void dispose () {
		batch.dispose();
		Assets.instance.dispose();
	}
}

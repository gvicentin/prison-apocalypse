package com.vgr.pa.screen;

import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.Input;
import com.badlogic.gdx.Screen;
import com.badlogic.gdx.scenes.scene2d.Actor;
import com.badlogic.gdx.scenes.scene2d.InputEvent;
import com.badlogic.gdx.scenes.scene2d.InputListener;
import com.badlogic.gdx.scenes.scene2d.Stage;
import com.badlogic.gdx.scenes.scene2d.ui.Label;
import com.badlogic.gdx.scenes.scene2d.ui.Skin;
import com.badlogic.gdx.scenes.scene2d.ui.Table;
import com.badlogic.gdx.scenes.scene2d.ui.TextButton;
import com.badlogic.gdx.scenes.scene2d.utils.ChangeListener;
import com.badlogic.gdx.utils.Align;
import com.badlogic.gdx.utils.ScreenUtils;
import com.badlogic.gdx.utils.viewport.Viewport;
import com.vgr.pa.Constants;
import com.vgr.pa.PrisonApocalypse;

public class GameOverScreen implements Screen {

    private final PrisonApocalypse game;

    private Stage stage;
    private Table table;
    private Skin skin;

    private TextButton continueBtn;

    public GameOverScreen(PrisonApocalypse game) {
        this.game = game;

        // load skin
        skin = new Skin(Gdx.files.internal("ui/uiskin.json"));

        stage = new Stage();

        table = new Table();
        table.setFillParent(true);
        stage.addActor(table);

        // table.setDebug(true);

        Label titleLabel = new Label("Game Over", skin);
        table.add(titleLabel).padBottom(50);

        table.row();
        continueBtn = new TextButton("Main Menu", skin);
        table.add(continueBtn).width(100).padBottom(5);

        handleButtonsClick();
    }

    private void handleButtonsClick() {
        // click play
        continueBtn.addListener(new ChangeListener() {
            public void changed(ChangeEvent event, Actor actor) {
                game.setScreen(PrisonApocalypse.SCREEN_MAIN_MENU);
            }
        });
    }

    @Override
    public void show() {
        Gdx.input.setInputProcessor(stage);
    }

    @Override
    public void render(float delta) {
        ScreenUtils.clear(Constants.BACKGROUND_COLOR);
        stage.act(delta);
        stage.draw();
    }

    @Override
    public void resize(int width, int height) {
        stage.getViewport().update(width, height);
    }

    @Override
    public void pause() {

    }

    @Override
    public void resume() {

    }

    @Override
    public void hide() {

    }

    @Override
    public void dispose() {
        stage.dispose();
    }
}
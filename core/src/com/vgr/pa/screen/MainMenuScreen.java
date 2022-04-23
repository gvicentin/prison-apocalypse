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

public class MainMenuScreen implements Screen {

    private static final String TAG = MainMenuScreen.class.getSimpleName();

    private final PrisonApocalypse game;

    private Stage stage;
    private Table table;
    private Skin skin;

    private TextButton playBtn;
    private TextButton settingsBtn;
    private TextButton quitBtn;

    public MainMenuScreen(PrisonApocalypse game) {
        this.game = game;

        // load skin
        skin = new Skin(Gdx.files.internal("ui/uiskin.json"));

        stage = new Stage();

        table = new Table();
        table.setFillParent(true);
        stage.addActor(table);

        // table.setDebug(true);

        Label titleLabel = new Label("Prison Apocalypse", skin);
        table.add(titleLabel).padBottom(50);

        table.row();
        playBtn = new TextButton("Play", skin);
        table.add(playBtn).width(100).padBottom(5);

        table.row();
        settingsBtn = new TextButton("Settings", skin);
        table.add(settingsBtn).width(100).padBottom(5);

        table.row();
        quitBtn = new TextButton("Quit", skin);
        table.add(quitBtn).width(100).padBottom(5);

        handleButtonsClick();
    }

    private void handleButtonsClick() {
        // click play
        playBtn.addListener(new ChangeListener() {
            public void changed(ChangeEvent event, Actor actor) {
                game.setScreen(PrisonApocalypse.SCREEN_GAME);
            }
        });

        settingsBtn.addListener(new ChangeListener() {
            public void changed(ChangeEvent event, Actor actor) {
                Gdx.app.debug(TAG, "settings");
                game.setScreen(PrisonApocalypse.SCREEN_SETTINGS);
            }
        });

        // click quit
        quitBtn.addListener(new ChangeListener() {
            @Override
            public void changed(ChangeEvent event, Actor actor) {
                game.close();
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
        Gdx.app.debug(TAG, "cleanup");
    }
}

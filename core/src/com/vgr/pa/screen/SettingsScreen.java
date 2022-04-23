package com.vgr.pa.screen;

import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.Screen;
import com.badlogic.gdx.scenes.scene2d.Actor;
import com.badlogic.gdx.scenes.scene2d.Stage;
import com.badlogic.gdx.scenes.scene2d.ui.*;
import com.badlogic.gdx.scenes.scene2d.utils.ChangeListener;
import com.badlogic.gdx.utils.ScreenUtils;
import com.vgr.pa.Constants;
import com.vgr.pa.PrisonApocalypse;
import com.vgr.pa.Settings;

public class SettingsScreen implements Screen {

    private static final String TAG = SettingsScreen.class.getSimpleName();
    private static final String[] RESOLUTIONS = {
            "1980x1080",
            "1080x720",
            "640x480"
    };
    private static final int DEFAULT_RESOLUTION = 0;
    private static final boolean DEFAULT_FULLSCREEN = false;
    private static final boolean DEFAULT_DEBUG = false;

    private final PrisonApocalypse game;

    private Stage stage;
    private Table table;
    private Skin skin;

    private SelectBox<String> resolutionSB;
    private CheckBox fullscreenCB;
    private CheckBox debugCB;
    private TextButton saveBtn;
    private TextButton restoreBtn;

    public SettingsScreen(PrisonApocalypse game) {
        this.game = game;

        // load skin
        skin = new Skin(Gdx.files.internal("ui/uiskin.json"));

        stage = new Stage();

        table = new Table();
        table.setFillParent(true);
        stage.addActor(table);

        // table.setDebug(true);

        Label titleLbl = new Label("Settings", skin);
        table.add(titleLbl).colspan(2).padBottom(100);

        table.row();
        Label resolutionLbl = new Label("Resolution", skin);
        table.add(resolutionLbl).width(200).left();

        resolutionSB = new SelectBox<String>(skin);
        resolutionSB.setItems(RESOLUTIONS);
        table.add(resolutionSB).right();

        table.row();
        Label fullscreenLbl = new Label("Fullscreen", skin);
        table.add(fullscreenLbl).left();

        fullscreenCB = new CheckBox("", skin);
        table.add(fullscreenCB).right();

        table.row();
        Label debugLbl = new Label("Debug", skin);
        table.add(debugLbl).left();

        debugCB = new CheckBox("", skin);
        table.add(debugCB).right();

        table.row();
        saveBtn = new TextButton("Save", skin);
        restoreBtn = new TextButton("Restore Defaults", skin);
        table.add(saveBtn).width(150).right().padTop(50).spaceRight(10);
        table.add(restoreBtn).width(150).left().padTop(50);

        handleButtonsClick();
    }

    private void handleButtonsClick() {
        saveBtn.addListener(new ChangeListener() {
            @Override
            public void changed(ChangeEvent event, Actor actor) {
                Gdx.app.log(TAG, "Resolution: " + resolutionSB.getSelected());
                Gdx.app.log(TAG, "Fullscreen: " + fullscreenCB.isChecked());
                Gdx.app.log(TAG, "Debug: " + debugCB.isChecked());
                Settings.instance.resolution = resolutionSB.getSelectedIndex();
                Settings.instance.debugPhysics = debugCB.isChecked();
                Settings.instance.fullscreen = fullscreenCB.isChecked();
                game.setScreen(PrisonApocalypse.SCREEN_MAIN_MENU);
            }
        });
        restoreBtn.addListener(new ChangeListener() {
            @Override
            public void changed(ChangeEvent event, Actor actor) {
                Gdx.app.log(TAG, "Restored default values");
                resolutionSB.setSelected(RESOLUTIONS[DEFAULT_RESOLUTION]);
                fullscreenCB.setChecked(DEFAULT_FULLSCREEN);
                debugCB.setChecked(DEFAULT_DEBUG);
            }
        });
    }

    @Override
    public void show() {
        Gdx.input.setInputProcessor(stage);
        resolutionSB.setSelected(RESOLUTIONS[Settings.instance.resolution]);
        fullscreenCB.setChecked(Settings.instance.fullscreen);
        debugCB.setChecked(Settings.instance.debugPhysics);
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

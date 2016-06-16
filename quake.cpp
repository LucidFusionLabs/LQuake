/*
 * $Id: quake.cpp 1336 2014-12-08 09:29:59Z justin $
 * Copyright (C) 2009 Lucid Fusion Labs

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "core/app/app.h"
#include "core/web/dom.h"
#include "core/web/css.h"
#include "core/app/flow.h"
#include "core/app/gui.h"
#include "q3map.h"

namespace LFL {
AssetMap asset;
SoundAssetMap soundasset;

Scene scene;
MapAsset *quake_map;

// LFL::Application FrameCB
int Frame(LFL::Window *W, unsigned clicks, int flag) {
  W->GetInputController<BindMap>(0)->Repeat(clicks);
  scene.cam.Look(W->gd);
  quake_map->Draw(W->gd, scene.cam);
  // scene.Get("arrow")->YawRight((double)clicks/500);
  // scene.Draw();

  // Press tick for console
  W->gd->DrawMode(DrawMode::_2D);
  W->DrawDialogs();
  return 0;
}

}; // namespace LFL
using namespace LFL;

extern "C" void MyAppCreate(int argc, const char* const* argv) {
  FLAGS_far_plane = 10000;
  FLAGS_ksens = 150;
  FLAGS_target_fps = 50;
  FLAGS_enable_video = FLAGS_enable_input = true;
  app = new Application(argc, argv);
  screen = new Window();
  screen->frame_cb = Frame;
  screen->width = 640;
  screen->height = 480;
  screen->caption = "Quake";
}

extern "C" int MyAppMain() {
  if (app->Create(__FILE__)) return -1;
  if (app->Init()) return -1;
  app->StartNewWindow(screen);

  //  asset.Add(Asset(name, texture,  scale, translate, rotate, geometry, 0, 0));
  asset.Add(Asset("arrow", "", .005, 1, -90, "arrow.obj", 0, 0));
  asset.Load();

  //  soundasset.Add(SoundAsset(name,   filename,   ringbuf, channels, sample_rate, seconds ));
  soundasset.Add(SoundAsset("draw", "Draw.wav", 0, 0, 0, 0));
  soundasset.Load();

  screen->gd->default_draw_mode = DrawMode::_3D;
  screen->shell = make_unique<Shell>(&asset, &soundasset, nullptr);

  BindMap *binds = screen->AddInputController(make_unique<BindMap>());
  //  binds->Add(Bind(key,        callback));
  binds->Add(Bind(Key::Return,    Bind::CB(bind(&Shell::grabmode, screen->shell.get(), vector<string>()))));
  binds->Add(Bind(Key::Escape,    Bind::CB(bind(&Shell::quit,     screen->shell.get(), vector<string>()))));
  binds->Add(Bind(Key::Backquote, Bind::CB(bind(&Shell::console,  screen->shell.get(), vector<string>()))));
  binds->Add(Bind(Key::Quote,     Bind::CB(bind(&Shell::console,  screen->shell.get(), vector<string>()))));
  binds->Add(Bind(Key::LeftShift, Bind::TimeCB(bind(&Entity::RollLeft,   &scene.cam, _1))));
  binds->Add(Bind(Key::Space,     Bind::TimeCB(bind(&Entity::RollRight,  &scene.cam, _1))));
  binds->Add(Bind('w',            Bind::TimeCB(bind(&Entity::MoveFwd,    &scene.cam, _1))));
  binds->Add(Bind('s',            Bind::TimeCB(bind(&Entity::MoveRev,    &scene.cam, _1))));
  binds->Add(Bind('a',            Bind::TimeCB(bind(&Entity::MoveLeft,   &scene.cam, _1))));
  binds->Add(Bind('d',            Bind::TimeCB(bind(&Entity::MoveRight,  &scene.cam, _1))));
  binds->Add(Bind('q',            Bind::TimeCB(bind(&Entity::MoveDown,   &scene.cam, _1))));
  binds->Add(Bind('e',            Bind::TimeCB(bind(&Entity::MoveUp,     &scene.cam, _1))));

  scene.Add(new Entity("axis",  asset("axis")));
  scene.Add(new Entity("grid",  asset("grid")));
  scene.Add(new Entity("room",  asset("room")));
  scene.Add(new Entity("arrow", asset("arrow"), v3(1, .24, 1)));

  quake_map = Q3MapAsset::Load(Asset::FileName("map-20kdm2.pk3"));
  scene.cam.pos = v3(1910.18,443.64,410.21);
  scene.cam.ort = v3(-0.05,0.70,0.03);
  scene.cam.up = v3(0.00,-0.04,0.98);

  // start our engine
  return app->Main();
}

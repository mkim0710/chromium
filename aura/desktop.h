// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef AURA_DESKTOP_H_
#define AURA_DESKTOP_H_
#pragma once

#include "aura/root_window.h"
#include "aura/aura_export.h"
#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/task.h"
#include "ui/gfx/compositor/compositor.h"
#include "ui/gfx/native_widget_types.h"

namespace gfx {
class Size;
}

namespace aura {

class DesktopHost;
class MouseEvent;

// Desktop is responsible for hosting a set of windows.
class AURA_EXPORT Desktop : public ui::CompositorDelegate {
 public:
  Desktop();
  ~Desktop();

  // Shows the desktop host.
  void Show();

  // Sets the size of the desktop.
  void SetSize(const gfx::Size& size);

  // Shows the desktop host and runs an event loop for it.
  void Run();

  // Draws the necessary set of windows.
  void Draw();

  // Handles a mouse event. Returns true if handled.
  bool OnMouseEvent(const MouseEvent& event);

  // Handles a key event. Returns true if handled.
  bool OnKeyEvent(const KeyEvent& event);

  // Compositor we're drawing to.
  ui::Compositor* compositor() { return compositor_.get(); }

  Window* window() { return window_.get(); }

  static Desktop* GetInstance();

 private:
  // Overridden from ui::CompositorDelegate
  virtual void ScheduleCompositorPaint();

  scoped_refptr<ui::Compositor> compositor_;

  scoped_ptr<internal::RootWindow> window_;

  DesktopHost* host_;

  static Desktop* instance_;

  // Used to schedule painting.
  ScopedRunnableMethodFactory<Desktop> schedule_paint_;

  DISALLOW_COPY_AND_ASSIGN(Desktop);
};

}  // namespace aura

#endif  // AURA_DESKTOP_H_

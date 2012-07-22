// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <QtWidgets>

#include "___PROJECTNAMEASIDENTIFIER___Window.h"

int main(int argc, char** argv)
{
  QApplication app(argc, argv);
  
  ___PROJECTNAMEASIDENTIFIER___Window window;
  window.resize(640, 480);
  window.show();
  
  return app.exec();
}


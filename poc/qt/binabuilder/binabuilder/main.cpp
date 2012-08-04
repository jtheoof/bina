// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <QtWidgets>

#include "BuilderWindow.h"

int main(int argc, char** argv)
{
  QApplication app(argc, argv);
  
  BuilderWindow window;
//  window.resize(1024, 768);
  window.show();
  
  return app.exec();
}


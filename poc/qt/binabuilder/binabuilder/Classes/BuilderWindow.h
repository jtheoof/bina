// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BuilderWindow_H
#define BuilderWindow_H

#include <QtWidgets>

class BuilderWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit BuilderWindow(QWidget *parent=0);
  ~BuilderWindow();
protected:
  void closeEvent(QCloseEvent *event);
  
  private slots:
  void load();
  void save();
  void hello();
  
private:
  void createWidgets();
  void createProxyWidgets();
  void createMainView();
  void createConnections();
  void createLayout();
  
private:
  QGraphicsRectItem* drawer_;
  QGraphicsView* view_;
  QGraphicsScene* scene_;
  QHash<QString, QGraphicsProxyWidget*> proxyWidgets_;
  QDialogButtonBox* boxButtons;
//  QPushButton* butHello;
  
};

#endif

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef testQT_TestWindow_h
#define testQT_TestWindow_h

#include <QtWidgets>
#include <QtCore>
#include <QtGui>

class MainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit MainWindow(QWidget *parent=0);
  ~MainWindow();
protected:
  void closeEvent(QCloseEvent *event);
  
private slots:
  void load();
  void save();
  void hello();
  
private:
  void createWidgets();
  void createConnections();
  void createLayout();

private:
  QDialogButtonBox* boxButtons;
  QPushButton* butHello;

};

#endif

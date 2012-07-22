// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ___PROJECTNAMEASIDENTIFIER___WINDOW_H
#define ___PROJECTNAMEASIDENTIFIER___WINDOW_H

#include <QtWidgets>
#include <QtCore>
#include <QtGui>

class ___PROJECTNAMEASIDENTIFIER___Window : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit ___PROJECTNAMEASIDENTIFIER___Window(QWidget *parent=0);
  ~___PROJECTNAMEASIDENTIFIER___Window();
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

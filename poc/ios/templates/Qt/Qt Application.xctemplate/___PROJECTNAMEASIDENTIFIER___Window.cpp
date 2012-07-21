// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "___PROJECTNAMEASIDENTIFIER___Window.h"
#include "moc____PROJECTNAMEASIDENTIFIER___Window.h"

#pragma mark -
#pragma mark Contructors/Desctrutors
#pragma mark -

___PROJECTNAMEASIDENTIFIER___Window::___PROJECTNAMEASIDENTIFIER___Window(QWidget *parent) : QMainWindow(parent)
{
  
  setWindowTitle(tr("Hello World"));
  statusBar()->showMessage(tr(""), 0);
  
  createWidgets();
  createLayout();
  createConnections();  
}

___PROJECTNAMEASIDENTIFIER___Window::~___PROJECTNAMEASIDENTIFIER___Window()
{
  delete butHello;
}

#pragma mark -
#pragma mark Window Setup
#pragma mark -

/**
 * @method createWigets
 * @abstract 
 *  This is where all UI components should be programatically created
 */
void ___PROJECTNAMEASIDENTIFIER___Window::createWidgets()
{
  boxButtons = new QDialogButtonBox;
  
  //  butHello = boxButtons->addButton(tr("Hello World"), 
  //                                   QDialogButtonBox::AcceptRole);
  
  butHello = new QPushButton(tr("Hello World"));
  
}

/**
 * @method createConnections
 * @abstract
 *  This is where signal connections on UI elements should be set
 */
void ___PROJECTNAMEASIDENTIFIER___Window::createConnections()
{
  connect(butHello, SIGNAL(clicked()), this, SLOT(hello()));
}

/**
 * @method createLayout
 * @abstract 
 *  This is where UI elements are added to the window through
 *  customized layout.
 */
void ___PROJECTNAMEASIDENTIFIER___Window::createLayout()
{
  // main layout
  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(butHello);
  
  // main window widget
  QWidget *widget = new QWidget;
  widget->setLayout(layout);
  setCentralWidget(widget);
  
  // focused widget
  butHello->setFocus();
}

#pragma mark -
#pragma mark Events
#pragma mark -

void ___PROJECTNAMEASIDENTIFIER___Window::closeEvent(QCloseEvent *event)
{
  bool okToClearData = true;
  if (okToClearData)
    event->accept();
  else
    event->ignore();
}

#pragma mark -
#pragma mark Actions
#pragma mark -

// basic event handling
void ___PROJECTNAMEASIDENTIFIER___Window::hello()
{
  statusBar()->showMessage(tr("Hello to you !"), 3*1000);
}

void ___PROJECTNAMEASIDENTIFIER___Window::save()
{
  // Saving code goes here
}

void ___PROJECTNAMEASIDENTIFIER___Window::load()
{
  // loodign code goes here
}
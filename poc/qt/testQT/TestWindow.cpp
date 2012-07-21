/*
 Copyright (c) 2011-2012 jjbrothers.net. All rights reserved.
 
 This program or module is free software: you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation, either version 3 of
 the License, or (at your option) any later version. It is provided
 for educational purposes and is distributed in the hope that it will
 be useful, but WITHOUT ANY WARRANTY; without even the implied
 warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
 the GNU General Public License for more details.
 */

#include "TestWindow.h"
#include "moc/moc_TestWindow.h"

#pragma mark -
#pragma mark Contructors/Desctrutors
#pragma mark -

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
  
  setWindowTitle(tr("Hello World"));
  statusBar()->showMessage(tr(""), 0);
  
  createWidgets();
  createLayout();
  createConnections();  
}

MainWindow::~MainWindow()
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
void MainWindow::createWidgets()
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
void MainWindow::createConnections()
{
  connect(butHello, SIGNAL(clicked()), this, SLOT(hello()));
}

/**
 * @method createLayout
 * @abstract 
 *  This is where UI elements are added to the window through
 *  customized layout.
 */
void MainWindow::createLayout()
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

void MainWindow::closeEvent(QCloseEvent *event)
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
void MainWindow::hello()
{
  statusBar()->showMessage(tr("Hello to you !"), 3*1000);
}

void MainWindow::save()
{
  // Saving code goes here
}

void MainWindow::load()
{
  // loodign code goes here
}
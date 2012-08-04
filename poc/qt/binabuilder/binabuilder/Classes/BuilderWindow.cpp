// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "BuilderWindow.h"
#include "moc_BuilderWindow.h"

#pragma mark -
#pragma mark Contructors/Desctrutors
#pragma mark -

BuilderWindow::BuilderWindow(QWidget *parent) : QMainWindow(parent)
{
//  scene_ = new QGraphicsScene(this);
//  scene_->setItemIndexMethod(QGraphicsScene::NoIndex);
//  
//  setWindowTitle(QApplication::applicationName());
//  statusBar()->showMessage(tr(""), 0);
//  
//  createWidgets();
////  createProxyWidgets();
//  createLayout();
//  createMainView();
////  createConnections();
  
  QGraphicsScene* scene = new QGraphicsScene( this );
  scene->setSceneRect( 0, 0, 500, 500 ); // this is what you need
  QGraphicsView* view = new QGraphicsView();
  QListWidget* left = new QListWidget();
  QListWidget* right = new QListWidget();

  view->setScene( scene );
  scene->addEllipse( 50, 50, 40, 40, QPen( Qt::red ), QBrush( Qt::blue ) );
  
  QImage image(tr("/Volumes/Storage/Projects/github/bina/poc/html/img/fordje_0001_background.png"));
  QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
  item->setPos(0, 0);
  scene->addItem(item);
  
  QSplitter* sp = new QSplitter( this );
  sp->addWidget( left );
  sp->addWidget( view );
  sp->addWidget( right );

  sp->setStretchFactor( 0, 1 );
  sp->setStretchFactor( 1, 2 );
  
  this->setCentralWidget( sp );
  
  
  
  
  
}

BuilderWindow::~BuilderWindow()
{
//  delete butHello;
}

#pragma mark -
#pragma mark Window Setup
#pragma mark -

/**
 * @method createWigets
 * @abstract 
 *  This is where all UI components should be programatically created
 */
void BuilderWindow::createWidgets()
{
  boxButtons = new QDialogButtonBox;
  
  // -----------------------------
  // QWidgets
  // -----------------------------
  QPushButton* button1 = new QPushButton(tr("Hello World"));
  QPushButton* button2 = new QPushButton(tr("World Hello"));

  // -----------------------------
  // Proxies
  // -----------------------------
  proxyWidgets_["hello"] = scene_->addWidget(button1);
  proxyWidgets_["world"] = scene_->addWidget(button2);

  // -----------------------------
  // Connections
  // -----------------------------
  connect(button1, SIGNAL(clicked()), this, SLOT(hello()));

  
}

void BuilderWindow::createProxyWidgets()
{
  
}

void BuilderWindow::createMainView()
{
  QSplitter t;
  drawer_ = new QGraphicsRectItem;
  drawer_->setFlags(QGraphicsItem::ItemClipsChildrenToShape);
  drawer_->setPen(QPen(QColor("brown"), 2.5));
  drawer_->setBrush(Qt::white);
  drawer_->setRect(100 + 100, 0, this->width(), this->height());
  scene_->addItem(drawer_);
  
  view_ = new QGraphicsView(scene_);
  view_->setRenderHints(QPainter::Antialiasing|
                       QPainter::TextAntialiasing);
  view_->setBackgroundBrush(QColor("bisque"));
  setCentralWidget(view_);
  
}
/**
 * @method createConnections
 * @abstract
 *  This is where signal connections on UI elements should be set
 */
void BuilderWindow::createConnections()
{
//  connect(butHello, SIGNAL(clicked()), this, SLOT(hello()));
}

/**
 * @method createLayout
 * @abstract 
 *  This is where UI elements are added to the window through
 *  customized layout.
 */
void BuilderWindow::createLayout()
{
  QGraphicsLinearLayout *leftLayout = new QGraphicsLinearLayout(Qt::Vertical);
  leftLayout->addItem(proxyWidgets_["hello"]);
  leftLayout->setMinimumWidth(100);
  leftLayout->setMaximumWidth(100);
  

  QGraphicsLinearLayout *rightLayout = new QGraphicsLinearLayout(Qt::Vertical);
  rightLayout->addItem(proxyWidgets_["world"]);
  
  // main layout
  QGraphicsLinearLayout *layout = new QGraphicsLinearLayout;
  layout->addItem(leftLayout);
  layout->setItemSpacing(0, 20);
  layout->addItem(rightLayout);
  
  // main window widget
  QGraphicsWidget *widget = new QGraphicsWidget;
  widget->setLayout(layout);
  scene_->addItem(widget);
  
  int width = qRound(layout->preferredWidth());
  int height = this->height();
  setMinimumSize(width, height);
  scene_->setSceneRect(0, 0, width, height);
}

#pragma mark -
#pragma mark Events
#pragma mark -

void BuilderWindow::closeEvent(QCloseEvent *event)
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
void BuilderWindow::hello()
{
  statusBar()->showMessage(tr("Hello to you !"), 3*1000);
}

void BuilderWindow::save()
{
  // Saving code goes here
}

void BuilderWindow::load()
{
  // loodign code goes here
}
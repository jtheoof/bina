//
//  main.cpp
//  testQT
//
//  Created by Johan Attali on 7/2/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <QtWidgets>

//#include <QtGui/QPushButton>
//#include <QtWebKit/QWebView>
//#include <QtCore/QCoreApplication>

#include "TestGraphicsView.h"

int main(int argc, char** argv)
{
  QApplication app(argc, argv);
    
//  QPushButton hello("Hello world!");
//  hello.resize(100, 30);
//  
////    hello.show();
//  
//  QString dirPath = QString("test");
  
//  QWebView *view = new QWebView();
//  
//  view->load(QUrl("file:///Volumes/Storage/Projects/github/bina/poc/html/index.html"));
  
//  view->show();
  
  QSplitter *splitter = new QSplitter;
  
  QFileSystemModel *model = new QFileSystemModel;
  model->setRootPath(QDir::currentPath());
  
  QTreeView *tree = new QTreeView(splitter);
  tree->setModel(model);
  tree->setRootIndex(model->index(QDir::currentPath()));
  
  
  QListView *list = new QListView(splitter);
  list->setModel(model);
  list->setRootIndex(model->index(QDir::currentPath()));
  
  splitter->setWindowTitle("Two views onto the same file system model");
  splitter->show();

//  TestGraphicsView* v = new TestGraphicsView();
//  v->show();
  
  return app.exec();
}


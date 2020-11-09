#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QScrollArea>
#include <QGridLayout>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QGraphicsView>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <list>
#include "diagramscene.h"

QT_BEGIN_NAMESPACE
class QAction;
class QWidget;
class DiagramScene;
class DiagramItem;
QT_END_NAMESPACE


class MainWindow : public QMainWindow {
	Q_OBJECT
	public:
		MainWindow(QWidget *parent = 0);
		~MainWindow();
		void WriteDesignInfo(); 
	private: // private data member
		// Drawing data
		QMap<QString, FFsLib> m_flib;
		QMap<QString, FFsLib*> m_ffs;
		QList<Net*> m_net;
		QString m_curFile;
		QWidget *m_mainWidget;

		// ======= ToolBar =================
		// ToolBar: viewToolBar
		// 	* Items
		// 		1. Scaling Up: scaleUpAct <-> scaleUp()
		// 			 * scaleUp() <-> glWidget->ViewScaleUp()
		// 		2. Scaling Down: scaleDownAct <-> scaleDown()
		// 			 * scaleDown() <-> glWidget->ViewScaleDown()
		// =================================
		QToolBar *m_viewToolBar;
			QAction *loadInFileAct;
			QAction *loadOutFileAct;
			QAction *clrSceneAct;
			QAction *exitAppAct;
			QAction *scaleUpAct;
			QAction *scaleDownAct;
			QAction *scaleOrigAct;
			
		// QGraphicsScene *m_scene;
		DiagramScene *m_scene;
		QGraphicsView  *m_view;
		int m_ChipSize[2], 
				m_GridSize[2], 
				m_BinSize[2];
		double m_MaxDensity;
		double m_ScaleValue;
		QListWidget *m_textWin;
		QListWidget *m_infoWin;
	public slots:
		void ItemSelected(const Net *_net, DiagramItem::DiagramType _type); // recieve from m_scene

	private slots:
		void _LoadInFile();
		void _LoadOutFile();
		void scaleUp(); 
		void scaleDown();
		void scaleNormal();
		void _ClearScene(); 
	private:
		/*void _CreateMenus();*/
		void _CreateActions();
		void _CreateToolBars();
		void _CreateWindow();
		void _Painting();
		void _DrawInputScene();
		void _DrawOutputScene();
		void _DrawFrame();
		void _DrawNet();
};

#endif // MAINWINDOW_H

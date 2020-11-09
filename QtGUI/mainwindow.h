#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtOpenGL>

#include "glwidget.h"
#include "rectangle.h"
#include "def.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <string>

QT_BEGIN_NAMESPACE
class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QSlider;
class QComboBox;
class QCheckBox;
QT_END_NAMESPACE
class GLWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private slots:
    void about();
		// add by hobo
		void open();
		void nonDrwa();
		/*void draw();*/
		void scaleUp();
		void scaleDown();
		void moveReset();
		void moveDown();
		void moveUp();
		void moveLeft();
		void moveRight();
		void gridLineFlip();
		// ===== QMenu: MultiFF =====
		void readMulFFSlot();
		void ReadMulFFOutputSlot(); // 顯示最後擺放Flip-flop的位置 
private:
    void createActions();
    void createMenus();
		void createToolBars();
    QSlider *createSlider(const char *changedSignal, const char *setterSlot);
    QSize getSize(); // un-used
		// add by hobo
		void loadFile(const QString &fileName);
		void loadNontree(const QString &fileName);
		// ===== QMenu: MultiFF =====
		void loadFFPlacer(const QString &fileName);
		void _LoadFFPlacerOut(const QString &fileName);

		QList<Point*>   *m_pList;   // I/O pin position
		QMap<QString, Point*> pinMap; 
		QList<Point*>   *m_ffList;  // flip-flop position
		QMap<QString, Point*> ffMap;
		QList<Segment*> *m_sList;   // connections
		int m_ChipSize[2],
			  m_GridSize[2], 
				m_BinSize[2];


		QString curFile;	

		// end
    QWidget *centralWidget;
    QScrollArea *glWidgetArea;
		/*QScrollArea *pixmapLabelArea;*/
    GLWidget *glWidget;
		/*QLabel *pixmapLabel;*/
    QSlider *xSlider;
    QSlider *ySlider;
    QSlider *zSlider;
		
		// Menu
		// ======= MenuBar =================
		// MenuBar: fileMenu
		// 	* Items
		// 		1. Open File: openFileAct <-> open()
		// 		3. NonTree
		// 			+ show graph
		// =================================
    QMenu *fileMenu;
    QMenu *nonTreeMenu;
    QMenu *MultiFFMenu;
    QMenu *helpMenu;
		// ======= ToolBar =================
		// ToolBar: viewToolBar
		// 	* Items
		// 		1. Scaling Up: scaleUpAct <-> scaleUp()
		// 			 * scaleUp() <-> glWidget->ViewScaleUp()
		// 		2. Scaling Down: scaleDownAct <-> scaleDown()
		// 			 * scaleDown() <-> glWidget->ViewScaleDown()
		// =================================
		QToolBar *viewToolBar;
		// ======= ComboBox ================
		// ComBoBox: layerComboxBox
		// =================================
		/*QComboBox *layerComboBox;*/
		// ======= CheckBox ================
		// CheckBox: showGridCheckBox
		// =================================
		/*QCheckBox *showGridcheckBox;*/

		// Action
		/*QAction *grabFrameBufferAct;*/
		/*QAction *renderIntoPixmapAct;*/
		QAction *openFileAct; // hobo define

		// ===== QMenu: MultiFF =====
		QAction *readMulFFInputAct;
		QAction *readMulFFMetaAct;
		// ===== QMenu: nonTreeMenu =====
		QAction *readNonTreeAct;
		/*QAction *drawGraphAct; // hobo define*/
		/*QAction *clearPixmapAct;*/
    QAction *exitAct;            // closeAllWindows 內建函數
    QAction *aboutAct;           // In Help Menu
    QAction *aboutQtAct;         // In Help Menu
		QAction *scaleUpAct;
		QAction *scaleDownAct;
		QAction *moveResetAct;
		QAction *moveDownAct;
		QAction *moveUpAct;
		QAction *moveLeftAct;
		QAction *moveRightAct;
		QAction *gridLineAct;
};

#endif

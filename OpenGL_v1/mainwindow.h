#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


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
		void importResult();
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
private:
    void createActions();
    void createMenus();
		void createToolBars();
    QSlider *createSlider(const char *changedSignal, const char *setterSlot);
    QSize getSize(); // un-used
		// add by hobo
		void loadFile(const QString &fileName);
		void loadRst(const QString &fileName);
		void loadNontree(const QString &fileName);

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
		// 		2. Import Result: readRstAct <-> importResult()
		// 		3. NonTree
		// 			+ show graph
		// =================================
    QMenu *fileMenu;
    QMenu *nonTreeMenu;
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
		QAction *readRstAct;  // hobo define
		QAction *readNonTreeAct;
		/*QAction *drawGraphAct; // hobo define*/
		/*QAction *clearPixmapAct;*/
    QAction *exitAct;            // closeAllWindows ¤º«Ø¨ç¼Æ
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

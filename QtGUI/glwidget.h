#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include "def.h"
#include "rectangle.h"
#include "point.h"
#include "segment.h"
#include "gemStructure.h"

// QT_BEIGN_NAMESAPCE
// class QPaintEvent;
// QT_END_NAMESAPCE

class GLWidget : public QGLWidget
{
    Q_OBJECT
public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);
public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    int xRotation() const { return xRot; }
    int yRotation() const { return yRot; }
    int zRotation() const { return zRot; }

		void setPointSet(QList<Point*> *p) { pList = p; }
		void setFlipFlopSet(QList<Point*> *f) { ffList = f; }
		void setBlockSet(QList<Block*> *b) { bList = b; }
		void setObsSet(QList<Block*> *o) { oList = o; }
		void setSegmentSet(QList<Segment*> *s) { sList = s; }
		void setChipWidth(int w)  { this->Chip_Width = w; }
		void setChipHeight(int h) { this->Chip_Height = h; }
		void drawObject(int W, int H, int laNum, int *pitch);
		void drawObject2();
		void drawObject3(unsigned int *, unsigned int *); // for nonTree
		// ===== QMenu: MultiFF =====
		void Draw2DFFPlacer(int* ChipSize, int* BinSize, int *GridSize); // for CAD 10
		// ===== Toolbar =====
		inline void ViewScaleUp() { scale *= 1.5;}
		inline void ViewScaleDown() { scale *= 0.5;}
		inline void ViewReDraw() { updateGL(); }
		inline void ViewMoveReset() { viewMove[0] = viewMove[1] = 0; }
		inline void ViewMoveDown() { viewMove[1] += 0.2; }
		inline void ViewMoveUp() { viewMove[1] -= 0.2; }
		inline void ViewMoveLeft() { viewMove[0] -= 0.2; }
		inline void ViewMoveRight() { viewMove[0] += 0.2; }
		void flipGridLine();
protected:
		// initial OpenGL window
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
private slots:
    void advanceGears();
private:
    void normalizeAngle(int *angle);
		void drawOuter();
		void drawGrid();
		// Transformation Factor
		const float transW();
		const float transH();
		const float transLayer();
		double TransX(int x);
		double TransY(int y);
		double TransZ(int z);
		// self-define
		GLuint makeObject();
		GLuint object;
		GLuint object2;
		GLuint object3;
		GLuint object4;
	  QColor trolltechGreen;
    QColor trolltechPurple;

		QColor faceColor[4];
    int xRot, yRot, zRot;
    QPoint lastPos;
		int Chip_Width, Chip_Height;
		int Layer_Number;
		int *Pitch;
		float viewMove[2];
		float scale;
		bool gridless;

		QList<Point*> *pList;   // I/O pin position
		QList<Point*> *ffList;  // flip-flop position
		QList<Block*> *bList; // switch box
		QList<Block*> *oList; // obstacle box
		QList<Segment*> *sList;
};

#endif

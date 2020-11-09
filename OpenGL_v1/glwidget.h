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

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    int xRotation() const { return xRot; }
    int yRotation() const { return yRot; }
    int zRotation() const { return zRot; }

		void setPointSet(QList<Point*> *p) { 
			pList = p; }
		void setBlockSet(QList<Block*> *b) { 
			bList = b; }
		void setObsSet(QList<Block*> *o) { 
			oList = o; }
		void setSegmentSet(QList<Segment*> *s) { 
			sList = s; }
		void drawObject(int W, int H, int laNum, 
				            int *pitch);
		void drawObject2();
		void drawObject3(unsigned int *, unsigned int *); // for nonTree
		void ViewScaleUp() { scale *= 1.5;}
		void ViewScaleDown() { scale *= 0.5;}
		void ViewReDraw() { updateGL(); }
		void ViewMoveReset() { viewMove[0] = viewMove[1] = 0; }
		void ViewMoveDown() { viewMove[1] += 0.2; }
		void ViewMoveUp() { viewMove[1] -= 0.2; }
		void ViewMoveLeft() { viewMove[0] -= 0.2; }
		void ViewMoveRight() { viewMove[0] += 0.2; }
		void flipGridLine();
public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);

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
		const float transW() { 
			if (Chip_Width != 0)
				return (right_x - left_x) / Chip_Width; 
			else
				return 0.0;
		}
		const float transH() { 
			if (Chip_Height != 0)
				return (far_z - near_z) / Chip_Height; 
			else
				return 0.0;
		}
		const float transLayer() {
			if (Layer_Number != 0)
				return ((bt_y - top_y) / Layer_Number); 
			else
				return 0.0;
		}
		double TransX(int x) { return (x*transW() + left_x); }
		double TransY(int y) { return (y*transH() + near_z); }
		double TransZ(int z) { return (bt_y - (z-1)*transLayer()); }
		// self-define
		GLuint makeObject();
		GLuint object;
		GLuint object2;
		GLuint object3;
	  QColor trolltechGreen;
    QColor trolltechPurple;

		QColor faceColor[4];
    int xRot; int yRot; int zRot;
    QPoint lastPos;
		int Chip_Width;
		int Chip_Height;
		int Layer_Number;
		int *Pitch;
		float viewMove[2];
		float scale;
		bool gridless;

		QList<Point*> *pList; 
		QList<Block*> *bList; // switch box
		QList<Block*> *oList; // obstacle box
		QList<Segment*> *sList;
};

#endif

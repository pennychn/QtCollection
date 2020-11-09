#include <QtGui>
#include <QtOpenGL>
#include <math.h>
#include "glwidget.h"
GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent) {
	  object = 0;
	  object2 = 0;
	  object3 = 0;
	  object4 = 0; // use for show extra information
    xRot = 0;
    yRot = 0;
    zRot = 0;
		scale = 10.0f;
		viewMove[0] = viewMove[1] = 0;
		faceColor[0] = Qt::red;
		faceColor[1] = Qt::green;
		faceColor[2] = Qt::blue;
		faceColor[3] = Qt::yellow;
		gridless = false; // default draw the grid line

		trolltechGreen = QColor::fromCmykF(0.40, 0.0, 1.0, 0.0);
	  trolltechPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);

		/* Stop auto rotation
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(advanceGears()));
    timer->start(20);
		*/
}
GLWidget::~GLWidget() {
    makeCurrent();
		glDeleteLists(object, 1);
		glDeleteLists(object2, 1);
		glDeleteLists(object3, 1);
		glDeleteLists(object4, 1);
}
// =========================================
// 畫面縮放/旋轉功能
// =========================================
void GLWidget::setXRotation(int angle) {
    normalizeAngle(&angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}
void GLWidget::setYRotation(int angle) {
    normalizeAngle(&angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}
void GLWidget::setZRotation(int angle) {
    normalizeAngle(&angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}
const float GLWidget::transW() { 
	if (Chip_Width != 0)
		return (right_x - left_x) / Chip_Width; 
	else
		return 0.0;
}
const float GLWidget::transH() { 
	if (Chip_Height != 0)
		return (far_z - near_z) / Chip_Height; 
	else
		return 0.0;
}
const float GLWidget::transLayer() {
	if (Layer_Number != 0)
		return ((bt_y - top_y) / Layer_Number); 
	else
		return 0.0;
}
double GLWidget::TransX(int x) { 
	*logger->logFile() << "X = " << (x*transW() + left_x) << std::endl;
	return (x*transW() + left_x); 
}
double GLWidget::TransY(int y) { 
	*logger->logFile() << "Y = " << (y*transH() + near_z) << std::endl;
	return (y*transH() + near_z); 
}
double GLWidget::TransZ(int z) { 
	return (bt_y - (z-1)*transLayer()); 
}
void GLWidget::initializeGL() {
	   qglClearColor(trolltechPurple.dark());
     glShadeModel(GL_FLAT);
     glEnable(GL_DEPTH_TEST);
     glEnable(GL_CULL_FACE);
}
void GLWidget::paintGL() {
	 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	 glLoadIdentity(); // Reset the View
	 // Move Into Screen 10 Units, 
	 // glTranslate 不是以中心點為基準點去移動，而是以當前的位置
	 glTranslated(INIT_TRANS_VIEW_X + viewMove[0], INIT_TRANS_VIEW_Y + viewMove[1], 
			          INIT_TRANS_VIEW_Z); 
	 glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
	 glRotated(yRot / 16.0, 0.0, 1.0, 0.0);
	 glRotated(zRot / 16.0, 0.0, 0.0, 1.0);
	 glScalef(scale, scale, scale);
	 glCallList(object3);
	 glCallList(object2);
	 glCallList(object4);
	 glCallList(object);
}
void GLWidget::resizeGL(int width, int height) {
		int view_BLPointX = 0;
		int view_BLPointY = 0;
		int view_Width = width;
		int view_Height = height;
		// Change Viwport Value to Move
    glViewport(view_BLPointX, 
				       view_BLPointY, 
							 view_Width, view_Height);

		glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // glOrtho(-0.5, +0.5, +0.5, -0.5, 4.0, 15.0);
    glOrtho(-3, +3, +3, -3, 5.0, 15.0);
    glMatrixMode(GL_MODELVIEW);
}
void GLWidget::mousePressEvent(QMouseEvent *event) {
    lastPos = event->pos();
}
void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    }
    lastPos = event->pos();
}
void GLWidget::advanceGears() {
    updateGL();
}
GLuint GLWidget::makeObject() {
	GLuint list = glGenLists(1);
	glNewList(list, GL_COMPILE);
	  drawOuter();
	glEndList();
	return list;
}
void GLWidget::normalizeAngle(int *angle) {
    while (*angle < 0)
        *angle += 360 * 16;
    while (*angle > 360 * 16)
        *angle -= 360 * 16;
}
void GLWidget::drawOuter() {
	 // Draw outer 
	glColor3f(1.0, 1.0, 1.0); // white
	// up-layer
	glBegin(GL_LINE_LOOP);
		glVertex3f(left_x, top_y, near_z);
		glVertex3f(right_x, top_y, near_z);
		glVertex3f(right_x, top_y, far_z);
		glVertex3f(left_x, top_y, far_z);
	glEnd();
	// down-layer
	glBegin(GL_LINE_LOOP);
		glVertex3f(left_x, bt_y, near_z);
		glVertex3f(right_x, bt_y, near_z);
		glVertex3f(right_x, bt_y, far_z);
		glVertex3f(left_x, bt_y, far_z);
	glEnd();
	// draw 柱子
	glBegin(GL_LINES);
		glVertex3f(left_x, top_y, near_z);
		glVertex3f(left_x, bt_y, near_z);
	glEnd();
	glBegin(GL_LINES);
		glVertex3f(right_x, top_y, near_z);
		glVertex3f(right_x, bt_y, near_z);
	glEnd();
	glBegin(GL_LINES);
		glVertex3f(left_x, top_y, far_z);
		glVertex3f(left_x, bt_y, far_z);
	glEnd();
	glBegin(GL_LINES);
		glVertex3f(right_x, top_y, far_z);
		glVertex3f(right_x, bt_y, far_z);
	glEnd();
}
void GLWidget::drawObject(int W, int H, int laNum, int *pitch) {
 // store the value
 Chip_Width = W;
 Chip_Height = H;
 Layer_Number = laNum;
 Pitch = pitch;

		GLuint list = glGenLists(1);
		glNewList(list, GL_COMPILE);

		// Draw pin
		glPointSize(4);
		glColor3f(0.0, 0.0, 0.0); 
		QList<Point*>::iterator i;
		QList<Point*>::iterator so;
		so = pList->begin();
		for (i = pList->begin(); i != pList->end(); ++i)
		{
			glBegin(GL_POINTS);
			glVertex3d(TransX((*i)->getX()),
								 TransZ((*i)->getZ()),
								 TransY((*i)->getY()));
			glEnd();
			if (i != so) {
				glBegin(GL_LINES);
					glVertex3d(TransX((*so)->getX()),
										 TransZ((*so)->getZ()),
										 TransY((*so)->getY()));
					glVertex3d(TransX((*i)->getX()),
										 TransZ((*i)->getZ()),
										 TransY((*i)->getY()));
				glEnd();
			}
		}
		// For each layer, draw the source node mappint to every layer
		glColor3f(1.0, 0.0, 1.0); 
		for (int i = 0; i < Layer_Number; ++i)
		{
				glBegin(GL_POINTS);
					glVertex3d(TransX((*so)->getX()),
										 TransZ(i+1),
										 TransY((*so)->getY()));
				glEnd();

				glBegin(GL_LINES);
					glVertex3d(TransX((*so)->getX()),
										 TransZ((*so)->getZ()),
										 TransY((*so)->getY()));
					glVertex3d(TransX((*so)->getX()),
										 TransZ(i+1),
										 TransY((*so)->getY()));
				glEnd();
		}


		// Draw Rectangle
		glColor3f(1.0, 0.0, 0.0); 
		QList<Block *>::iterator pBlk;
		
		for (pBlk = bList->begin(); pBlk != bList->end(); ++pBlk)
		{
			glBegin(GL_QUAD_STRIP);
				glVertex3d(TransX((*pBlk)->getBLX()),
									 TransZ((*pBlk)->getBLP()->getZ()),
									 TransY((*pBlk)->getTRY()));
				glVertex3d(TransX((*pBlk)->getBLX()),
									 TransZ((*pBlk)->getBLP()->getZ()),
									 TransY((*pBlk)->getBLY()));
				glVertex3d(TransX((*pBlk)->getTRX()),
									 TransZ((*pBlk)->getBLP()->getZ()),
									 TransY((*pBlk)->getTRY()));
				glVertex3d(TransX((*pBlk)->getTRX()),
									 TransZ((*pBlk)->getBLP()->getZ()),
									 TransY((*pBlk)->getBLY()));
			glEnd();
		}

		// Draw Obstacle 
		glColor3f(1.0, 1.0, 0.0); 
		
		for (pBlk = oList->begin(); pBlk != oList->end(); ++pBlk)
		{
			glBegin(GL_QUAD_STRIP);
				glVertex3d(TransX((*pBlk)->getBLX()),
									 TransZ((*pBlk)->getBLP()->getZ()),
									 TransY((*pBlk)->getTRY()));
				glVertex3d(TransX((*pBlk)->getBLX()),
									 TransZ((*pBlk)->getBLP()->getZ()),
									 TransY((*pBlk)->getBLY()));
				glVertex3d(TransX((*pBlk)->getTRX()),
									 TransZ((*pBlk)->getBLP()->getZ()),
									 TransY((*pBlk)->getTRY()));
				glVertex3d(TransX((*pBlk)->getTRX()),
									 TransZ((*pBlk)->getBLP()->getZ()),
									 TransY((*pBlk)->getBLY()));
			glEnd();
		}

	 drawGrid();
 glEndList();

 object2 = list;
 paintGL(); // redraw object1 and object2
}
void GLWidget::flipGridLine() {
	 gridless = gridless?false:true;

	 glDeleteLists(object2, 1);

			GLuint list = glGenLists(1);
			glNewList(list, GL_COMPILE);

			// Draw pin
			glPointSize(4);
			glColor3f(0.0, 0.0, 0.0); 
			QList<Point*>::iterator i;
			QList<Point*>::iterator so;
			so = pList->begin();
			for (i = pList->begin(); i != pList->end(); ++i) {
				glBegin(GL_POINTS);
				glVertex3d(TransX((*i)->getX()),
						       TransZ((*i)->getZ()),
						       TransY((*i)->getY()));
				glEnd();
				if (i != so) {
					glBegin(GL_LINES);
						glVertex3d(TransX((*so)->getX()),
											 TransZ((*so)->getZ()),
											 TransY((*so)->getY()));
						glVertex3d(TransX((*i)->getX()),
											 TransZ((*i)->getZ()),
											 TransY((*i)->getY()));
					glEnd();
				}
			}
			// For each layer, draw the source node mappint to every layer
			glColor3f(1.0, 0.0, 1.0); 
			for (int i = 0; i < Layer_Number; ++i)
			{
					glBegin(GL_POINTS);
						glVertex3d(TransX((*so)->getX()),
											 TransZ(i+1),
											 TransY((*so)->getY()));
					glEnd();

					glBegin(GL_LINES);
						glVertex3d(TransX((*so)->getX()),
											 TransZ((*so)->getZ()),
											 TransY((*so)->getY()));
						glVertex3d(TransX((*so)->getX()),
											 TransZ(i+1),
											 TransY((*so)->getY()));
					glEnd();
			}


			// Draw Rectangle
			glColor3f(1.0, 0.0, 0.0); 
			QList<Block *>::iterator pBlk;
			
			for (pBlk = bList->begin(); pBlk != bList->end(); ++pBlk)
			{
				glBegin(GL_QUAD_STRIP);
					glVertex3d(TransX((*pBlk)->getBLX()),
										 TransZ((*pBlk)->getBLP()->getZ()),
										 TransY((*pBlk)->getTRY()));
					glVertex3d(TransX((*pBlk)->getBLX()),
										 TransZ((*pBlk)->getBLP()->getZ()),
										 TransY((*pBlk)->getBLY()));
					glVertex3d(TransX((*pBlk)->getTRX()),
										 TransZ((*pBlk)->getBLP()->getZ()),
										 TransY((*pBlk)->getTRY()));
					glVertex3d(TransX((*pBlk)->getTRX()),
										 TransZ((*pBlk)->getBLP()->getZ()),
										 TransY((*pBlk)->getBLY()));
				glEnd();
			}

			// Draw Obstacle 
			glColor3f(1.0, 1.0, 0.0); 
			
			for (pBlk = oList->begin(); pBlk != oList->end(); ++pBlk)
			{
				glBegin(GL_QUAD_STRIP);
					glVertex3d(TransX((*pBlk)->getBLX()),
										 TransZ((*pBlk)->getBLP()->getZ()),
										 TransY((*pBlk)->getTRY()));
					glVertex3d(TransX((*pBlk)->getBLX()),
										 TransZ((*pBlk)->getBLP()->getZ()),
										 TransY((*pBlk)->getBLY()));
					glVertex3d(TransX((*pBlk)->getTRX()),
										 TransZ((*pBlk)->getBLP()->getZ()),
										 TransY((*pBlk)->getTRY()));
					glVertex3d(TransX((*pBlk)->getTRX()),
										 TransZ((*pBlk)->getBLP()->getZ()),
										 TransY((*pBlk)->getBLY()));
				glEnd();
			}

		 if (! gridless)
			 drawGrid();
		 else {
			glColor3f(1.0, 1.0, 1.0); 
			 for (int i =0; i < Layer_Number; ++i)
			 {
					// up-layer
					glBegin(GL_LINE_LOOP);
						glVertex3f(left_x, TransZ(i+1), near_z);
						glVertex3f(right_x, TransZ(i+1), near_z);
						glVertex3f(right_x, TransZ(i+1), far_z);
						glVertex3f(left_x, TransZ(i+1), far_z);
					glEnd();
			 }
		 }
	 glEndList();

	 object2 = list;
	 paintGL(); // redraw object1 and object2
 }
void GLWidget::drawGrid() {
	glEnable(GL_LINE_SMOOTH);
	// Draw Grid
		glColor3f(1.0, 1.0, 1.0); // red
		for (int k = 0; k < Layer_Number; ++k) {
			int p = Pitch[k];
			int WGridNum = Chip_Width / p;
			int HGridNum = Chip_Height / p;
			for (int i = 0; i <= WGridNum; ++i) {
				for (int j = 0; j <= HGridNum; ++j) 
				{
					// Vertical Line
					if (j < HGridNum){
						glBegin(GL_LINES);
							glVertex3d(TransX(p*i),
												 TransZ(k+1), 
												 TransY(p*j));
							glVertex3d(TransX(p*i),
												 TransZ(k+1), 
												 TransY(p*(j+1)));
						glEnd();
					}
					// Horizontal Line
					if (i < WGridNum) {
						glBegin(GL_LINES);
							glVertex3d(TransX(p*i),
												 TransZ(k+1), 
												 TransY(p*j));
							glVertex3d(TransX(p*(i+1)),
												 TransZ(k+1), 
												 TransY(p*j));
						glEnd();
					}
				}
			}
		}
	// draw Boundary
}
void GLWidget::drawObject2() {
	GLuint list = glGenLists(1);
	glNewList(list, GL_COMPILE);
		// Draw pin
		glLineWidth(2);
		glColor3f(0.0, 0.0, 1.0); 
		QList<Segment*>::iterator i;
		for (i = sList->begin(); i != sList->end(); ++i)
		{
			glBegin(GL_LINES);
			glVertex3d(TransX((*i)->PointOne()->getX()),
							 TransZ((*i)->PointOne()->getZ()),
							 TransY((*i)->PointOne()->getY()));
			glVertex3d(TransX((*i)->PointTwo()->getX()),
							 TransZ((*i)->PointTwo()->getZ()),
							 TransY((*i)->PointTwo()->getY()));
			glEnd();
		}
		glLineWidth(1);
	glEndList();

	object3 = list;
	paintGL(); 
}
// =========================================
// Non-tree Drawing
// =========================================
void GLWidget::drawObject3(unsigned int *BL, unsigned int *TR) {
	// store the value
	Chip_Width = TR[0] - BL[0];
	Chip_Height = TR[1] - BL[1];
	Layer_Number = 1;
	// ==================================
	// Construct OpenGL object
	// ==================================
	GLuint list = glGenLists(1);
	glNewList(list, GL_COMPILE);

	// ==================================
	// Draw Point
	// ==================================
	glPointSize(4);
	QList<Point*>::iterator i;
	i = pList->begin();
	// source node labed red
	glColor3f(1.0, 0.0, 0.0); 
	glBegin(GL_POINTS);
	glVertex3d(TransX((*i)->getX()),
						 TransZ((*i)->getZ()),
						 TransY((*i)->getY()));
	glEnd();
	glColor3f(0.0, 0.0, 0.0); 
	for (++i; i != pList->end(); ++i)
	{
		glBegin(GL_POINTS);
		glVertex3d(TransX((*i)->getX()),
							 TransZ((*i)->getZ()),
							 TransY((*i)->getY()));
		glEnd();
	}
	// ==================================
	// Draw Segment
	// ==================================
	glLineWidth(2);
	glColor3f(0.0, 0.0, 1.0); 
	QList<Segment*>::iterator sItr;
	for (sItr = sList->begin(); sItr != sList->end(); ++sItr)
	{
		glBegin(GL_LINES);
		glVertex3d(TransX((*sItr)->PointOne()->getX()),
						   TransZ((*sItr)->PointOne()->getZ()),
						   TransY((*sItr)->PointOne()->getY()));
		glVertex3d(TransX((*sItr)->PointTwo()->getX()),
						   TransZ((*sItr)->PointTwo()->getZ()),
						   TransY((*sItr)->PointTwo()->getY()));
		glEnd();
	}

	// ==================================
	// Complete OpenGL object config
	// ==================================
	glEndList();
	object = list;
	paintGL(); 
}
// 使用object2去畫
void GLWidget::Draw2DFFPlacer(int* ChipSize, int* BinSize, int *GridSize) {
	this->setChipHeight(ChipSize[1]);
	this->setChipWidth(ChipSize[0]);
	GLuint list = glGenLists(1);
		glNewList(list, GL_COMPILE);

		// Draw I/O pin && flip-flop
		glPointSize(3);
		QList<Point*>::iterator i;
		for (i = pList->begin(); i != pList->end(); ++i) {
			if ((*i)->getID() == 0)
				glColor3f(1.0, 1.0, 0.0);  // IN(yellow)
			else if ((*i)->getID() == 1)
				glColor3f(0.0, 1.0, 0.0); //  OUT(green)

			glBegin(GL_POINTS);
			glVertex2d(TransX((*i)->getX()), TransY((*i)->getY()));
			glEnd();
		}
		glColor3f(1.0, 0.0, 0.0); // FF(red)
		for (i = ffList->begin(); i != ffList->end(); ++i) {
			glBegin(GL_POINTS);
			glVertex2d(TransX((*i)->getX()), TransY((*i)->getY()));
			glEnd();
		}
		// Draw connection
		glLineWidth(1);
		glColor3f(0.0, 0.0, 1.0); 
		QList<Segment*>::iterator j;
		for (j = sList->begin(); j != sList->end(); ++j)
		{
			glBegin(GL_LINES);
			glVertex2d(TransX((*j)->PointOne()->getX()),
							 TransY((*j)->PointOne()->getY()));
			glVertex2d(TransX((*j)->PointTwo()->getX()),
							 TransY((*j)->PointTwo()->getY()));
			glEnd();
		}
		/*
		glPointSize(4);
		glColor3f(1.0, 0.0, 1.0); 
		for (i = ffVec.begin(); i != ffVec.end(); ++i) {
			glBegin(GL_POINTS);
			glVertex3d(TransX((*i)->getX()), 0,TransY((*i)->getY()));
			glEnd();
		}
			*/

		glEnable(GL_LINE_SMOOTH);
		// Draw Bin
		glColor3f(1.0, 1.0, 1.0); // white
			int WBinNum = ChipSize[0]/ BinSize[0];
			int HBinNum = ChipSize[1]/ BinSize[1];
			for (int i = 0; i <= WBinNum; ++i) {
				for (int j = 0; j <= HBinNum; ++j) {
					// Vertical Line
					if (j < HBinNum){
						glBegin(GL_LINES);
							glVertex2d(TransX(BinSize[0]*i), TransY(BinSize[0]*j));
							glVertex2d(TransX(BinSize[0]*i), TransY(BinSize[0]*(j+1)));
						glEnd();
					}
					// Horizontal Line
					if (i < WBinNum) {
						glBegin(GL_LINES);
							glVertex2d(TransX(BinSize[1]*i), TransY(BinSize[1]*j));
							glVertex2d(TransX(BinSize[1]*(i+1)), TransY(BinSize[1]*j));
						glEnd();
					}
				}
			}
		// Draw Grid Point
		/*
		glColor3f(1.0, 1.0, 0.0); // yellow
			int WGridNum = ChipSize[0]/ GridSize[0];
			int HGridNum = ChipSize[1]/ GridSize[1];
			*logger->logFile() << "WGridNum = " << WGridNum << ", HGridNum = " << HGridNum << std::endl;
			for (int i = 0; i <= WGridNum; ++i) {
				for (int j = 0; j <= HGridNum; ++j) {
					// Vertical Line
					if (j < HGridNum){
						glBegin(GL_LINES);
							glVertex2d(TransX(GridSize[0]*i), TransY(GridSize[0]*j));
							glVertex2d(TransX(GridSize[0]*i), TransY(GridSize[0]*(j+1)));
						glEnd();
					}
					// Horizontal Line
					if (i < WGridNum) {
						glBegin(GL_LINES);
							glVertex2d(TransX(GridSize[1]*i), TransY(GridSize[1]*j));
							glVertex2d(TransX(GridSize[1]*(i+1)), TransY(GridSize[1]*j));
						glEnd();
					}
				}
			}
		*/
 glEndList();

 object2 = list;
 paintGL(); // redraw object1 and object2
}

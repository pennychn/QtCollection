#ifndef DEF_H
#define DEF_H
#include "logger.h"

const float left_x = -0.3; const float right_x = 0.3;
const float bt_y = 0.3; const float top_y = -0.3;
const float near_z = -0.3; const float far_z = 0.3;
const float _unit = 0.1;

const int INIT_WIN_WIDTH = 800;
const int INIT_WIN_HEIGHT = 600;


// void GLWidget::paintGL() 
const float INIT_TRANS_VIEW_X = 0.0;
const float INIT_TRANS_VIEW_Y = 0.1;
const float INIT_TRANS_VIEW_Z = -10.0;

extern Logger *logger;

struct FPoint {
	float x, y;
	FPoint(float _x, float _y) {
		x = _x; y = _y;
	}
};

struct FBlock {
	FPoint *bl, *tr;
	FBlock(FPoint *_bl, FPoint *_tr) {
		bl = _bl; tr = _tr;
	}
};

#endif // DEF_H

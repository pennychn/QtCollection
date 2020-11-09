#include "mainwindow.h"

Logger *logger = new Logger();
MainWindow::MainWindow() {
	m_pList  = new QList<Point*>();
	m_ffList = new QList<Point*>();
	m_sList  = new QList<Segment*>();
	m_ChipSize[0] = m_ChipSize[1] = 0;
	m_GridSize[0] = m_GridSize[1] = 0;
	m_BinSize[0]  = m_BinSize[1]  = 0;

	setWindowIcon(QPixmap(":resource/qt4logo.png"));
	centralWidget = new QWidget;
	setCentralWidget(centralWidget);

	glWidget = new GLWidget;
	//pixmapLabel = new QLabel;

	glWidgetArea = new QScrollArea;
	glWidgetArea->setWidget(glWidget);
	glWidgetArea->setWidgetResizable(true);
	glWidgetArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	glWidgetArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	glWidgetArea->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	glWidgetArea->setMinimumSize(50, 50);

	//pixmapLabelArea = new QScrollArea;
	//pixmapLabelArea->setWidget(pixmapLabel);
	//pixmapLabelArea->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	//pixmapLabelArea->setMinimumSize(50, 50);


	xSlider = createSlider(SIGNAL(xRotationChanged(int)),
												 SLOT(setXRotation(int)));
	ySlider = createSlider(SIGNAL(yRotationChanged(int)),
												 SLOT(setYRotation(int)));
	zSlider = createSlider(SIGNAL(zRotationChanged(int)),
												 SLOT(setZRotation(int)));

	createActions();
	createMenus();
	createToolBars();

	QGridLayout *centralLayout = new QGridLayout;
	centralLayout->addWidget(glWidgetArea, 0, 0, 1, 2);
	// centralLayout->addWidget(pixmapLabelArea, 0, 1);
	centralLayout->addWidget(xSlider, 1, 0, 1, 2);
	centralLayout->addWidget(ySlider, 2, 0, 1, 2);
	centralLayout->addWidget(zSlider, 3, 0, 1, 2);
	centralWidget->setLayout(centralLayout);

	xSlider->setValue(15 * 16);
	ySlider->setValue(345 * 16);
	zSlider->setValue(0 * 16);

	setWindowTitle(tr("QT4 GUI Viewer"));
	resize(INIT_WIN_WIDTH, INIT_WIN_HEIGHT);
}
//void MainWindow::renderIntoPixmap()
//{
    //QSize size = getSize();
    //if (size.isValid()) {
        //QPixmap pixmap = glWidget->renderPixmap(size.width(), size.height());
        //setPixmap(pixmap);
    //}
//}
//void MainWindow::grabFrameBuffer()
//////////{
    //////////QImage image = glWidget->grabFrameBuffer();
		//////////// 以QPixmap格式讀取image
    //////////setPixmap(QPixmap::fromImage(image));
//////////}
//void MainWindow::clearPixmap()
//////{
    //////setPixmap(QPixmap());
//////}
void MainWindow::about()
{
    QMessageBox::about(this, tr("About Multilayer Drawing"),
            tr("<b> Multi-Layer Drawing by OpenGL</b>"
							 "<br> Author: hobo <br> Version: v.1"));
}
void MainWindow::createActions()
{
	// File Menu
	openFileAct = new QAction(tr("&Opne File"), this);
  openFileAct->setShortcut(tr("Ctrl+O"));
	connect(openFileAct, SIGNAL(triggered()), 
			    this, SLOT(open()));

	// NonTreeMenu
	readNonTreeAct = new QAction(tr("Draw"), this);
	connect(readNonTreeAct, SIGNAL(triggered()), 
			    this, SLOT(nonDrwa()));
	// MultiFFMenu
	readMulFFInputAct = new QAction(tr("Draw"), this);
	connect(readMulFFInputAct, SIGNAL(triggered()), 
			    this, SLOT(readMulFFSlot()));
	readMulFFMetaAct = new QAction(tr("MetaDraw"), this);
	connect(readMulFFMetaAct , SIGNAL(triggered()), 
			    this, SLOT(ReadMulFFOutputSlot()));

    //renderIntoPixmapAct = new QAction(tr("&Render into Pixmap..."), this);
    //renderIntoPixmapAct->setShortcut(tr("Ctrl+R"));
    //connect(renderIntoPixmapAct, SIGNAL(triggered()),
            //this, SLOT(renderIntoPixmap()));

    //grabFrameBufferAct = new QAction(tr("&Grab Frame Buffer"), this);
    //grabFrameBufferAct->setShortcut(tr("Ctrl+G"));
    //connect(grabFrameBufferAct, SIGNAL(triggered()),
            //this, SLOT(grabFrameBuffer()));

    //clearPixmapAct = new QAction(tr("&Clear Pixmap"), this);
    //clearPixmapAct->setShortcut(tr("Ctrl+L"));
    //connect(clearPixmapAct, SIGNAL(triggered()), this, SLOT(clearPixmap()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
	
	 scaleUpAct = new QAction(QIcon(":/resource/zoomin.png"), tr("&ScaleUp"), this);
	 scaleUpAct->setStatusTip(tr("Scale Up"));
	 connect(scaleUpAct, SIGNAL(triggered()), this, SLOT(scaleUp()));

	 scaleDownAct = new QAction(QIcon(":/resource/zoomout.png"), tr("&ScaleDown"), this);
	 scaleDownAct->setStatusTip(tr("Scale Down"));
	 connect(scaleDownAct, SIGNAL(triggered()), this, SLOT(scaleDown()));

	 moveResetAct = new QAction(QIcon(":/resource/monkey.png"), tr("&MoveDown"), this);
	 scaleDownAct->setStatusTip(tr("Move Down"));
	 connect(moveResetAct, SIGNAL(triggered()), this, SLOT(moveReset()));

	 moveDownAct = new QAction(QIcon(":/resource/down.bmp"), tr("&MoveDown"), this);
	 scaleDownAct->setStatusTip(tr("Move Down"));
	 connect(moveDownAct, SIGNAL(triggered()), this, SLOT(moveDown()));

	 moveUpAct = new QAction(QIcon(":/resource/up.bmp"), tr("&MoveDown"), this);
	 scaleDownAct->setStatusTip(tr("Move Down"));
	 connect(moveUpAct, SIGNAL(triggered()), this, SLOT(moveUp()));

	 moveLeftAct = new QAction(QIcon(":/resource/left.bmp"), tr("&MoveDown"), this);
	 scaleDownAct->setStatusTip(tr("Move Down"));
	 connect(moveLeftAct, SIGNAL(triggered()), this, SLOT(moveLeft()));

	 moveRightAct = new QAction(QIcon(":/resource/right.bmp"), tr("&MoveDown"), this);
	 scaleDownAct->setStatusTip(tr("Move Down"));
	 connect(moveRightAct, SIGNAL(triggered()), this, SLOT(moveRight()));

	 gridLineAct= new QAction(QIcon(":/resource/gridless.png"), tr("&GridLine"), this);
	 gridLineAct->setStatusTip(tr("Grid Line"));
	 connect(gridLineAct, SIGNAL(triggered()), this, SLOT(gridLineFlip()));
}
void MainWindow::createMenus()
{
		// Menu Level 1
    fileMenu = menuBar()->addMenu(tr("&File"));
		{
			// Menu Level 2
			fileMenu->addAction(openFileAct);
			//fileMenu->addAction(renderIntoPixmapAct);
			//fileMenu->addAction(grabFrameBufferAct);
			//fileMenu->addAction(clearPixmapAct);
			fileMenu->addSeparator();
			fileMenu->addAction(exitAct);
		}
		nonTreeMenu = menuBar()->addMenu(tr("&NonTree"));
		{
			nonTreeMenu->addAction(readNonTreeAct);
		}
		MultiFFMenu = menuBar()->addMenu(tr("&MultiFF"));
		{
			MultiFFMenu->addAction(readMulFFInputAct);
			MultiFFMenu->addAction(readMulFFMetaAct);
		}


    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}
QSlider *MainWindow::createSlider(const char *changedSignal,
                                  const char *setterSlot)
{
    QSlider *slider = new QSlider(Qt::Horizontal);
    slider->setRange(0, 360 * 16);
    slider->setSingleStep(16);
    slider->setPageStep(15 * 16);
    slider->setTickInterval(15 * 16);
    slider->setTickPosition(QSlider::TicksRight);
    connect(slider, SIGNAL(valueChanged(int)), glWidget, setterSlot);
    connect(glWidget, changedSignal, slider, SLOT(setValue(int)));
    return slider;
}
void MainWindow::createToolBars()
{
	 viewToolBar = addToolBar(tr("Scale"));
	 viewToolBar->addAction(scaleUpAct);
	 viewToolBar->addAction(scaleDownAct);
	 viewToolBar = addToolBar(tr("Move"));
	 viewToolBar->addAction(moveResetAct);
	 viewToolBar->addAction(moveDownAct);
	 viewToolBar->addAction(moveUpAct);
	 viewToolBar->addAction(moveLeftAct);
	 viewToolBar->addAction(moveRightAct);
	 viewToolBar = addToolBar(tr("Grid"));
	 viewToolBar->addAction(gridLineAct);
}
//void MainWindow::setPixmap(const QPixmap &pixmap)
//////////////{
    //////////////pixmapLabel->setPixmap(pixmap);
		//////////////QSize size = pixmap.size();
		//////////////if (size - QSize(1, 0) == pixmapLabelArea->maximumViewportSize())
			//////////////size -= QSize(1, 0);
		//////////////pixmapLabel->resize(size);
//////////////}
QSize MainWindow::getSize()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Grabber"),
                                         tr("Enter pixmap size:"),
                                         QLineEdit::Normal,
                                         tr("%1 x %2").arg(glWidget->width())
                                                      .arg(glWidget->height()),
                                         &ok);
    if (!ok)
        return QSize();

    QRegExp regExp(tr("([0-9]+) *x *([0-9]+)"));
    if (regExp.exactMatch(text)) {
        int width = regExp.cap(1).toInt();
        int height = regExp.cap(2).toInt();
        if (width > 0 && width < 2048 && height > 0 && height < 2048)
            return QSize(width, height);
    }

    return glWidget->size();
    //QSize size = getSize();
}
// add by hobo
// ======================================
// File Menu
// ======================================
void MainWindow::open()
{
		QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
			loadFile(fileName);
}
// ======================================
// NonTree Menu
// ======================================
void MainWindow::nonDrwa() {
		QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
        loadNontree(fileName);
}
//void MainWindow::draw() { 
	//// glWidget->paintGL();
//}
void MainWindow::scaleUp() {
	glWidget->ViewScaleUp();
	glWidget->ViewReDraw();
}
void MainWindow::scaleDown() {
	glWidget->ViewScaleDown();
	glWidget->ViewReDraw();

}
void MainWindow::moveReset() {
	glWidget->ViewMoveReset();
	glWidget->ViewReDraw();

}
void MainWindow::moveDown() {
	glWidget->ViewMoveDown();
	glWidget->ViewReDraw();

}
void MainWindow::moveUp() {
	glWidget->ViewMoveUp();
	glWidget->ViewReDraw();

}
void MainWindow::moveLeft() {
	glWidget->ViewMoveLeft();
	glWidget->ViewReDraw();

}
void MainWindow::moveRight() {
	glWidget->ViewMoveRight();
	glWidget->ViewReDraw();

}
void MainWindow::gridLineFlip() {
	glWidget->flipGridLine();
	glWidget->ViewReDraw();

}
// ======================================
// File Menu: slot function
// ======================================
void MainWindow::loadFile(const QString &fileName) {
	QList<Block*> *bList;
	QList<Block*> *oList;
	bList = new QList<Block*>(); // switch
	oList = new QList<Block*>(); // obstacle

	Point *pPoint = NULL, *pPoint2 = NULL;
	Block *pBlock = NULL;
	int BoundBL[2], BoundTR[2], layerNum;
	int* layerPitch;
	const int max_in_size = 1000;
	std::ifstream inFile;
	
	std::string fName;
	fName = fileName.toStdString();

	int x1, y1, x2, y2;

	inFile.open(fName.c_str(), std::ios::in);
	if (!inFile) {
		 QMessageBox::about(this, tr("ERROR!"),
			 tr("file isn't exist..........."));	
	}
	else
	{
		char buffer[max_in_size] = "";
		char x_in;
		while (inFile.getline(buffer, max_in_size, '\n')) {
			sscanf(buffer, ".chip (%d %d) (%d %d)", BoundBL, BoundBL+1, BoundTR, BoundTR+1);

			// read layer information
			inFile.getline(buffer, max_in_size);
			sscanf(buffer, ".layer %d", &(layerNum));
			layerPitch = new int[layerNum];
			for (int i = 0; i < layerNum; ++i) {
				// layer info skip
			  inFile.getline(buffer, max_in_size);
			  sscanf(buffer, "%d %c %d %d %d", &(x1), &x_in, &x2, &y1, &y2);
				layerPitch[i] = x2;
			}
			int swNum =0;
			int swLayNum = 0;
			inFile.getline(buffer, max_in_size);
			sscanf(buffer, ".switch %d", &(swNum)); // x1 = number of switch 
			for (int i = 0; i < swNum; ++i) {
			  inFile.getline(buffer, max_in_size);
			  sscanf(buffer, "(%u %u %d) (%u %u", &(x1), &(y1), &(swLayNum), &(x2), &(y2));

				pPoint = new Point(x1, y1, swLayNum);
				pPoint2 = new Point(x2, y2, swLayNum);
				pBlock = new Block(pPoint, pPoint2);
				bList->push_back(pBlock);
			}
			int viaNum = 0;
			inFile.getline(buffer, max_in_size);
			sscanf(buffer, ".via %d", &(viaNum));
			for (int i = 0; i < viaNum; ++i) {
				// via info skip
			  inFile.getline(buffer, max_in_size);
			}

			// read net information
			int netNum = 0, pinNum = 0, netNameLen = 0;
			bool firstPin = true;
			int netBottomLayer, netTopLayer;
			char netName[12];
			inFile.getline(buffer, max_in_size);
			sscanf(buffer, ".net %d", &(netNum));
			for (int i = 0; i < netNum; ++i) {
			  inFile.getline(buffer, max_in_size);
			  sscanf(buffer, "%s %d", netName, &(pinNum));
			  netNameLen = strlen(netName);

			  // add net
				//ptr_net = new Net(netName, pinNum);

			  char ch;
			  std::string s;
			  netBottomLayer = INT_MAX, netTopLayer = INT_MIN;
				firstPin = true;
			  for (int j = netNameLen + 2; j < max_in_size;++j) {
					ch = buffer[j];
					if (ch == '(')
					{
						// axis_x
						ch = buffer[++j];
						while (isdigit(ch)) {
							s.push_back(ch);
							ch = buffer[++j];
						}
						x1 = atol(s.c_str());
						s.clear();

						// axis_y
						ch = buffer[++j];
						while (isdigit(ch)) {
							s.push_back(ch);
							ch = buffer[++j];
						}
						x2 = atol(s.c_str());
						s.clear();

						// layer
						ch = buffer[++j];
						while (isdigit(ch)) {
							s.push_back(ch);
							ch = buffer[++j];
						}
						y1 = atoi(s.c_str());
						s.clear();

						if (firstPin)
						{
							pPoint = new Point(x1, x2, y1);
							m_pList->push_back(pPoint);
							firstPin = false;
						}
						else
						{
							pPoint = new Point(x1, x2, y1);
							m_pList->push_back(pPoint);
						}
						--pinNum;
					}
					if (pinNum == 0)
						break; // exit loop
			  }
			}
			int obsNum =0;
			int obsLayNum = 0;
			inFile.getline(buffer, max_in_size);
			sscanf(buffer, ".obs %d", &(obsNum)); // x1 = number of switch 
			for (int i = 0; i < obsNum; ++i) {
			  inFile.getline(buffer, max_in_size);
			  sscanf(buffer, "(%d %d %d) (%d %d", &(x1), &(y1), &(obsLayNum), &(x2), &(y2));
				pPoint = new Point(x1, y1, obsLayNum);
				pPoint2 = new Point(x2, y2, obsLayNum);
				pBlock = new Block(pPoint, pPoint2);
				oList->push_back(pBlock);
			}
		}
	}  
	
	glWidget->setPointSet(m_pList);
	glWidget->setBlockSet(bList);
	glWidget->setObsSet(oList);
	glWidget->drawObject(BoundTR[0], BoundTR[1], layerNum, layerPitch);
}
// ====================================================
// >>>>>>>>> MultiFF Menu <<<<<<<<<<<<<<<<
// Load File: CAD10 B1
// Multi-bit Flip Flops: s1.input
// ====================================================
void MainWindow::readMulFFSlot() {
	QString fileName = QFileDialog::getOpenFileName(this);
	if (!fileName.isEmpty())
			loadFFPlacer(fileName);
}
void MainWindow::ReadMulFFOutputSlot() {
	QString fileName = QFileDialog::getOpenFileName(this);
	if (!fileName.isEmpty())
			_LoadFFPlacerOut(fileName);
}
void MainWindow::loadFFPlacer(const QString &fileName) {
	// Prepare for QT
	//QList<Point*> *pinList = new QList<Point*>(); // pin list, includes IN / OUT pins
	//QList<Point*> *m_ffList = new QList<Point*>(); // flip-flop pin list
	//QList<Segment*> *segList = new QList<Segment*>();
	//QMap<QString, Point*> pMap;

	double m_MaxDensity = 0.0f;
	Point *tmpPnt1 = NULL;
	Point *tmpPnt2 = NULL;
	//QString pinName; // store temp pinName for each pin line

	const int max_in_size = 1000;
	std::ifstream inFile;
	std::string fName;
	fName = fileName.toStdString();

	inFile.open(fName.c_str(), std::ios::in);
	if (!inFile) {
		*logger->logFile() << fName << " could not be opened. " << std::endl;
		 QMessageBox::about(this, tr("ERROR!"),
			 tr("file isn't exist..........."));	
	}
	else {
		char buffer[max_in_size] = "";
		char *token = NULL;
		char delims[] = " ,\t\n"; // blank or tab or return
		while (inFile.getline(buffer, max_in_size, '\n')) {
			if (buffer[0] == '#') continue;
			// fetch first token
			token = strtok(buffer, delims);
			// ======= Head Section ==============
			if (strncmp(token, "CHIP", 4) == 0) {
				token = strtok(NULL, delims);
				m_ChipSize[0] = atol(token);
				token = strtok(NULL, delims);
				token = strtok(NULL, delims);
				m_ChipSize[1] = atol(token);
				token = strtok(NULL, delims);
				*logger->logFile() << m_ChipSize[0] << ", " << m_ChipSize[1] << std::endl;
				continue;
			}
			if (strncmp(token, "GRID", 4) == 0) {
				token = strtok(NULL, delims);
				m_GridSize[0] = atol(token);
				token = strtok(NULL, delims);
				token = strtok(NULL, delims);
				m_GridSize[1] = atol(token);
				token = strtok(NULL, delims);
				*logger->logFile() << m_GridSize[0] << ", " << m_GridSize[1] << std::endl;
				continue;
			}
			if (strncmp(token, "BIN", 3) == 0) {
				token = strtok(NULL, delims);
				m_BinSize[0] = atol(token);
				token = strtok(NULL, delims);
				token = strtok(NULL, delims);
				m_BinSize[1] = atol(token);
				token = strtok(NULL, delims);
				*logger->logFile() << m_BinSize[0] << ", " << m_BinSize[1] << std::endl;
				continue;
			}
			if (strncmp(token, "PLACEMENT", 9) == 0) {
				token = strtok(NULL, delims);
				m_MaxDensity = atol(token);
				*logger->logFile() << m_MaxDensity << std::endl;
				continue;
			}
			// ======= Library Section ==============
			if (strncmp(token, "[LIBRARY]", 9) == 0) {
				*logger->logFile() << "Parser Lib: "  << token << std::endl;
				while(strncmp(token, "[END", 4) != 0) { // END LIBRARY
					inFile.getline(buffer, max_in_size, '\n');
					token = strtok(buffer, delims);
					if (strncmp(token, "[FLIP_FLOP_PROPERTY]", 20) == 0) {
						*logger->logFile() << "Parser FF property: "  << token << std::endl;
						while(strncmp(token, "[END", 4) != 0) { // END FILP_FLOP_PROPERTY
							inFile.getline(buffer, max_in_size, '\n');
							token = strtok(buffer, delims);
							// Fetech a FF library: in GUI, we don't need to store this message
							if (strncmp(token, "[FLIP_FLOP", 10) == 0) {
								*logger->logFile() << "Parser FF Type: "  << token << std::endl;
								//FFsLib *pLib = new FFsLib();
								token = strtok(NULL, delims);
								*logger->logFile() << "FF's type: "  << token << std::endl;
								//sscanf(token, "FF%d]", &pLib->fType);

								inFile.getline(buffer, max_in_size, '\n');
								token = strtok(buffer, delims);
								token = strtok(NULL, delims);
								*logger->logFile() << "FF's bit: "  << token << std::endl;
								//pLib->bNum = atoi(token);

								inFile.getline(buffer, max_in_size, '\n');
								token = strtok(buffer, delims);
								token = strtok(NULL, delims);
								*logger->logFile() << "FF's power: "  << token << std::endl;
								//pLib->pConsume = atoi(token);

								inFile.getline(buffer, max_in_size, '\n');
								token = strtok(buffer, delims);
								token = strtok(NULL, delims);
								*logger->logFile() << "FF's area: "  << token << std::endl;
								//pLib->fArea = atoi(token);

								//m_flib->push_back(pLib);
								//*logger->logFile() << pLib->fType << ", " << pLib->bNum << ", " << pLib->pConsume << ", " << pLib->fArea << std::endl;
								inFile.getline(buffer, max_in_size, '\n'); // skip [END FLIP_FLOP]
								/// Here, the token is keep the last value (AREA xxx)
								/// Therefore, the following while loop must not match
							}
						}
					}
				}
			}
			// ======= Design Section ==============
			if (strncmp(token, "[DESIGN]", 8) == 0) {
				*logger->logFile() << "Parser Design: "  << token << std::endl;
				while(strncmp(token, "[END", 4) != 0) { // END DESIGN
					inFile.getline(buffer, max_in_size, '\n');
					token = strtok(buffer, delims);
					// ======= FF LIST Section ==============
					if (strncmp(token, "[FLIP_FLOP_LIST]", 16) == 0) {
						*logger->logFile() << "Parser FF LIST: "  << token << std::endl;
						while(strncmp(token, "[END", 4) != 0) { // END FILP_FLOP_LIST
							inFile.getline(buffer, max_in_size, '\n');
							token = strtok(buffer, delims);
							// Fetech a FF list
							if (strncmp(token, "FF", 2) == 0) {
								int _id = -1, _tid = -1;
								unsigned int _x = 0, _y = 0;
								sscanf(token, "FF%d", &_tid); // type id
								token = strtok(NULL, delims);
								sscanf(token, "FLIP_FLOP%d", &_id); // id
								token = strtok(NULL, delims);
								sscanf(token, "(%u", &_x); // axis of x
								token = strtok(NULL, delims);
								sscanf(token, "%u)", &_y); // axis of x
								tmpPnt1 = new Point(2, _x, _y, 1);
								m_ffList->push_back(tmpPnt1); // FLIP-FLOP , ID = 2
								ffMap.insert("FLIP_FLOP" + QString().setNum(_id), tmpPnt1);
								*logger->logFile() << "TID(" << _tid << "),FID(" << _id << "),X(" << _x << "),Y(" << _y << ")\n";
							}
						}
					}
					/// The following two line is avoid the [END FLIP_FLOP_LIST] to break this loop
					inFile.getline(buffer, max_in_size, '\n');
					token = strtok(buffer, delims);
					// ======= PIN LIST Section ==============
					if (strncmp(token, "[PIN_LIST]", 10) == 0) {
						*logger->logFile() << "Parser PIN LIST: "  << token << std::endl;
						while(strncmp(token, "[END", 4) != 0) { // END PIN_LIST 
							inFile.getline(buffer, max_in_size, '\n');
							token = strtok(buffer, delims);
							// Fetech a FF list
							int _id = -1; unsigned int _x = 0, _y = 0;
							if (strncmp(token, "INPUT", 5) == 0) {
								/// @Add for other parser section
								do {
									token = strtok(NULL, delims);
								} while (0 == isalpha(token[0]));
								sscanf(token, "PIN%d", &_id); // id
								token = strtok(NULL, delims);
								sscanf(token, "(%u", &_x); // axis of x
								token = strtok(NULL, delims);
								sscanf(token, "%u)", &_y); // axis of x
								//m_pin->push_back(new Pin(_id, IN, _x, _y));
								tmpPnt1 = new Point(0, _x, _y, 1); // INPUT ID = 0
								m_pList->push_back(tmpPnt1);
								pinMap.insert("PIN" + QString().setNum(_id), tmpPnt1);
								*logger->logFile() << "PID(" << _id << "),X(" << _x << "),Y(" << _y << ")\n";
							}
							else if (strncmp(token, "OUTPUT", 6) == 0) {
								token = strtok(NULL, delims);
								sscanf(token, "PIN%d", &_id); // id
								token = strtok(NULL, delims);
								sscanf(token, "(%u", &_x); // axis of x
								token = strtok(NULL, delims);
								sscanf(token, "%u)", &_y); // axis of x
								//m_pin->push_back(new Pin(_id, OUT, _x, _y));
								tmpPnt1 = new Point(1, _x, _y, 1); // OUTPUT PIN, ID = 1
								m_pList->push_back(tmpPnt1);
								pinMap.insert("PIN" + QString().setNum(_id), tmpPnt1);
								*logger->logFile() << "PID(" << _id << "),X(" << _x << "),Y(" << _y << ")\n";
							}
						}
					}
					// ======= NET LIST Section ==============
					inFile.getline(buffer, max_in_size, '\n');
					token = strtok(buffer, delims);
					if (strncmp(token, "[NET_LIST]", 10) == 0) {
						*logger->logFile() << "Parser NET LIST: "  << token << std::endl;
						while(strncmp(token, "[END", 4) != 0) { // END NET_LIST
							inFile.getline(buffer, max_in_size, '\n');
							token = strtok(buffer, delims);
							// Fetech a FF list
							int _id1 = -1, _id2 = -1; unsigned int _slack = 0;
							if (strncmp(token, "PIN", 3) == 0) {
								sscanf(token, "PIN%d", &_id1); // pin id
								token = strtok(NULL, delims);
								sscanf(token, "FLIP_FLOP%d", &_id2); // flip-flop id
								token = strtok(NULL, delims);
								_slack = atol(token);

								QMap<QString, Point *>::iterator findItr;
								QMap<QString, Point *>::iterator findItr2;
								QString qEName1 = "PIN" + QString().setNum(_id1);
								QString qEName2 = "FLIP_FLOP" + QString().setNum(_id2);
								findItr  = pinMap.find(qEName1);
								findItr2 = ffMap.find(qEName2);
								if (findItr != pinMap.end() && findItr2 != ffMap.end()) {
									tmpPnt1 = findItr.value();
									tmpPnt2 = findItr2.value();
									m_sList->push_back(new Segment(tmpPnt1, tmpPnt2));
								}
								else {
									*logger->errFile() << qEName1.toStdString() << ", "
										                 << qEName2.toStdString() << std::endl;
								}
								*logger->logFile() << "PID(" << _id1 << "),FID(" << _id2 << "),Slack(" << _slack << ")" << std::endl;
							}
						}
					}
					// ======= BLOCK LIST Section ==============
					inFile.getline(buffer, max_in_size, '\n');
					token = strtok(buffer, delims);
					if (strncmp(token, "[BLOCK_LIST]", 12) == 0) {
						*logger->logFile() << "Parser BLOCK LIST: "  << token << std::endl;
						while(strncmp(token, "[END", 4) != 0) { // END BLOCK_LIST 
							inFile.getline(buffer, max_in_size, '\n');
							token = strtok(buffer, delims);
							// Fetech a block list
							int _id = -1;
							unsigned int _x = 0, _y = 0, _area;
							if (strncmp(token, "BLOCK", 5) == 0) {
								sscanf(token, "BLOCK%d", &_id); // block id
								token = strtok(NULL, delims);
								sscanf(token, "(%u", &_x); // axis of x
								token = strtok(NULL, delims);
								sscanf(token, "%u)", &_y); // axis of y
								token = strtok(NULL, delims);
								_area = atol(token);

								//m_obs->push_back(new OBPin(_id, OBS, _x, _y, _area));
								//pPoint = new Point(x1, y1, 1);
								//pPoint2 = new Point(x2, y2, 1);
								//pBlock = new Block(pPoint, pPoint2);
								//bList->push_back(pBlock);
								*logger->logFile() << "OID(" << _id << "),X(" << _x << "),Y(" << _y << "),AREA(" << _area << ")" << std::endl;
							}
						}
					}
				}
			}
		}
	}
	inFile.close();
	glWidget->setPointSet(m_pList);
	glWidget->setFlipFlopSet(m_ffList);
	glWidget->setSegmentSet(m_sList);
	glWidget->Draw2DFFPlacer(m_ChipSize, m_BinSize, m_GridSize);
}
void MainWindow::_LoadFFPlacerOut(const QString &fileName) {
	ffMap.clear();
	m_ffList->clear(); ///@bug operation w/o release memory
	m_sList->clear();  ///@bug operation w/o release memory

	const int max_in_size = 1000;
	std::ifstream inFile;
	std::string fName;
	fName = fileName.toStdString();
	Point *tmpPnt1 = NULL;
	Point *tmpPnt2 = NULL;

	inFile.open(fName.c_str(), std::ios::in);
	if (!inFile) {
		*logger->logFile() << fName << " could not be opened. " << std::endl;
		 QMessageBox::about(this, tr("ERROR!"),
			 tr("File isn't exist..........."));	
	}
	else {
		char buffer[max_in_size] = "";
		char *token = NULL;
		char delims[] = " ,\t\n"; // blank or tab or return
		while (inFile.getline(buffer, max_in_size, '\n')) {
			if (buffer[0] == '#') continue;
			token = strtok(buffer, delims);
			// ======= FF LIST Section ==============
			if (strncmp(token, "[FLIP_FLOP_LIST]", 16) == 0) {
				*logger->logFile() << "Parser FF LIST: "  << token << std::endl;
				while(strncmp(token, "[END", 4) != 0) { // END FILP_FLOP_LIST
					inFile.getline(buffer, max_in_size, '\n');
					token = strtok(buffer, delims);
					// Fetech a FF list
					if (strncmp(token, "FF", 2) == 0) {
						unsigned int _x = 0, _y = 0;
						char fName[64];
						QString sffName;
						// sscanf(token, "FF%d", &_tid); // skip this parameter
						token = strtok(NULL, delims);
						// sscanf(token, "FLIP_FLOP%d", &_id); // id, skip this parameter
						sscanf(token, "%s", fName); // id, skip this parameter
						sffName = fName;
						token = strtok(NULL, delims);
						sscanf(token, "(%u", &_x); // axis of x
						token = strtok(NULL, delims);
						sscanf(token, "%u)", &_y); // axis of x
						tmpPnt1 = new Point(2, _x, _y, 1); // FLIP-FLOP , ID = 2
						*logger->logFile() << sffName.toStdString() << ": " << _x << ", " << _y << std::endl;
						m_ffList->push_back(tmpPnt1); 
						ffMap.insert(sffName, tmpPnt1);
					}
				}
			}
			// ======= NET LIST Section ==============
			inFile.getline(buffer, max_in_size, '\n');
			token = strtok(buffer, delims);
			if (strncmp(token, "[NET_LIST]", 10) == 0) {
				*logger->logFile() << "Parser NET LIST: "  << token << std::endl;
				while(strncmp(token, "[END", 4) != 0) { // END NET_LIST
					inFile.getline(buffer, max_in_size, '\n');
					token = strtok(buffer, delims);
					// Fetech a FF list
					int _id1 = -1; // _id2 = -1; 
					unsigned int _slack = 0;
					if (strncmp(token, "PIN", 3) == 0) {
						char fName[64];
						QString sffName;
						sscanf(token, "PIN%d", &_id1); // pin id
						token = strtok(NULL, delims);
						// sscanf(token, "FLIP_FLOP%d", &_id2); // flip-flop id
						sscanf(token, "%s", fName); // id, skip this parameter
						sffName = fName;

						token = strtok(NULL, delims);
						_slack = atol(token);

						QMap<QString, Point *>::iterator findItr;
						QMap<QString, Point *>::iterator findItr2;
						QString qEName1 = "PIN" + QString().setNum(_id1);
						QString qEName2 = sffName;
						// QString qEName2 = "FLIP_FLOP" + QString().setNum(_id2);
						findItr  = pinMap.find(qEName1);
						findItr2 = ffMap.find(qEName2);
						if (findItr != pinMap.end() && findItr2 != ffMap.end()) {
							tmpPnt1 = findItr.value();
							tmpPnt2 = findItr2.value();
							m_sList->push_back(new Segment(tmpPnt1, tmpPnt2));
						}
						else {
							*logger->errFile() << qEName1.toStdString() << ", "
								<< qEName2.toStdString() << std::endl;
						}
						*logger->logFile() << qEName1.toStdString() << ", " 
							                 << qEName2.toStdString() << " ==> Slack(" 
															 << _slack << ")" << std::endl;
					}
				}
			}
		}
	}
	inFile.close();
	glWidget->Draw2DFFPlacer(m_ChipSize, m_BinSize, m_GridSize);
}
// ======================================
// NonTree Menu: slot function
// ======================================
void MainWindow::loadNontree(const QString &fileName) {
	QMap<QString, Point *> pMap;

	Point *pPoint = NULL, *pPoint2 = NULL;
	std::ifstream inFile;
	
	std::string fName;
	fName = fileName.toStdString(); // transfer to standard string

	inFile.open(fName.c_str(), std::ios::in);
	if (!inFile) {
		 QMessageBox::about(this, tr("ERROR!"),
			 tr("file isn't exist..........."));	
	}
	else
	{
		char buffer[80], nodeName[30]="", edgeName[10]="", 
				 soName[64], siName[64], nodeType[64]="";
		double pinLoad, driveRes, unitRes, unitCap;
		//double nodeLoad = 0.0f;
		int nodeNum = 0, edgeIdx = 0;
		unsigned int axis_x, axis_y;// , nodeId;
		unsigned int low_point[2];
		unsigned int up_point[2];

		while (inFile.getline(buffer, 80))
		{
			sscanf(buffer, "driver resistance %lf", &driveRes);
			inFile.getline(buffer, 80); sscanf(buffer, "wire resistance %lf", &unitRes);
			inFile.getline(buffer, 80); sscanf(buffer, "wire capacitance %lf", &unitCap);
			inFile.getline(buffer, 80); sscanf(buffer, "pin capacitance %lf", &pinLoad);
			inFile.getline(buffer, 80); sscanf(buffer, "low point ( %u, %u )", low_point, low_point+1); // low point ...
			inFile.getline(buffer, 80); sscanf(buffer, "high point ( %u, %u )", up_point, up_point+1); // high point ...
			inFile.getline(buffer, 80); // tree name
			inFile.getline(buffer, 80); sscanf(buffer, "number_of_nodes %d", &nodeNum);

			for (int i = 0; i < nodeNum; ++i) {
				inFile.getline(buffer, 80);
				//sscanf(buffer, "%s %s %lf %u %u", nodeType, nodeName, &nodeLoad, 
																					//&axis_x, &axis_y);
				sscanf(buffer, "%s %s %u %u", nodeType, nodeName, &axis_x, &axis_y);
				pPoint = new Point(axis_x, axis_y, 0, nodeName);
				QString qName = nodeName;
				pMap.insert(qName, pPoint);
				m_pList->push_back(pPoint);
			}
			while (inFile.getline(buffer, 80)) {
				sscanf(buffer, "%s %s %s", edgeName, soName, siName);
				sprintf(edgeName, "E%d", ++edgeIdx);
				QMap<QString, Point *>::iterator findItr;
				QMap<QString, Point *>::iterator findItr2;
				QString qEName1 = soName;
				QString qEName2 = siName;
				findItr  = pMap.find(qEName1);
				findItr2 = pMap.find(qEName2);
				if (findItr != pMap.end() && findItr2 != pMap.end()) {
					pPoint   = findItr.value();
					pPoint2  = findItr2.value();
					m_sList->push_back(new Segment(pPoint, pPoint2, edgeName));
				}
				else {
					QMessageBox::about(this, tr("ERROR!"), tr("Search point in the Map failed."));	
				}
			}
		}
		glWidget->setPointSet(m_pList);
		glWidget->setSegmentSet(m_sList);
		glWidget->drawObject3(low_point, up_point);
	}
}

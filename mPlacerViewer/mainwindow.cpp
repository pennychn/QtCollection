#include "mainwindow.h"

Logger *pLog = new Logger();
int _max(int val1, int val2) {
	return (val1 > val2)?val1:val2;
}
int _min(int val1, int val2) {
	return (val1 < val2)?val1:val2;
}
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
		setWindowIcon(QPixmap(":resource/run.png"));
    m_mainWidget = new QWidget;
    setCentralWidget(m_mainWidget);
		m_ScaleValue = 1.0f;

		m_scene = new DiagramScene;
		m_view = new QGraphicsView(m_scene);
		m_view->setRenderHint(QPainter::Antialiasing);
    // m_view.setBackgroundBrush(QPixmap(":/images/cheese.jpg"));
    m_view->setCacheMode(QGraphicsView::CacheBackground);
    m_view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    m_view->setDragMode(QGraphicsView::ScrollHandDrag);
    m_view->setWindowTitle(QT_TRANSLATE_NOOP(QGraphicsView, "mPlacer View"));
    m_view->resize(20, 20);
		
		_CreateActions();
		_CreateToolBars();
		_CreateWindow();

		connect(m_scene, SIGNAL(ItemSelected(const Net *, DiagramItem::DiagramType )),
				    this, SLOT(ItemSelected(const Net *, DiagramItem::DiagramType)));

    QGridLayout *centralLayout = new QGridLayout;
		centralLayout->addWidget(m_view, 0, 0, 1, 2);
    m_mainWidget->setLayout(centralLayout);

    setWindowTitle(tr("mPlacer Viewer v.01"));
    resize(800, 600);
    // m_view.show();
}
MainWindow::~MainWindow() { 
}
void MainWindow::_CreateActions() {
	loadInFileAct = new QAction(QIcon(":/resource/fileopen.png"), 
			                        tr("Open Input File"), this);
	connect(loadInFileAct, SIGNAL(triggered()), 
			    this, SLOT(_LoadInFile()));
	loadOutFileAct = new QAction(QIcon(":/resource/fileopen2.png"), 
			                        tr("Open Output File"), this);
	connect(loadOutFileAct, SIGNAL(triggered()), 
			    this, SLOT(_LoadOutFile()));

	clrSceneAct = new QAction(QIcon(":/resource/clear.png"), 
			                     tr("C&lear"), this);
	connect(clrSceneAct, SIGNAL(triggered()), this, SLOT(_ClearScene()));

	exitAppAct = new QAction(QIcon(":/resource/exit.png"), 
			                     tr("E&xit"), this);
	exitAppAct->setShortcut(tr("Ctrl+Q"));
	connect(exitAppAct, SIGNAL(triggered()), this, SLOT(close()));

	scaleUpAct = new QAction(QIcon(":/resource/zoomin.png"), tr("&ScaleUp"), this);
	scaleUpAct->setStatusTip(tr("Scale Up"));
	connect(scaleUpAct, SIGNAL(triggered()), this, SLOT(scaleUp()));

	scaleDownAct = new QAction(QIcon(":/resource/zoomout.png"), tr("&ScaleDown"), this);
	scaleDownAct->setStatusTip(tr("Scale Down"));
	connect(scaleDownAct, SIGNAL(triggered()), this, SLOT(scaleDown()));

	scaleOrigAct = new QAction(QIcon(":/resource/nozoom.png"), tr("&NormalVeiw"), this);
	scaleOrigAct->setStatusTip(tr("NormalView"));
	connect(scaleOrigAct, SIGNAL(triggered()), this, SLOT(scaleNormal()));
}
void MainWindow::_CreateToolBars() {
	m_viewToolBar = addToolBar(tr("Scale"));
	m_viewToolBar->addAction(loadInFileAct);
	m_viewToolBar->addAction(loadOutFileAct);
	m_viewToolBar->addAction(clrSceneAct);
	m_viewToolBar->addAction(scaleOrigAct);
	m_viewToolBar->addAction(scaleUpAct);
	m_viewToolBar->addAction(scaleDownAct);
	m_viewToolBar->addAction(exitAppAct);
}
void MainWindow::_CreateWindow() {
	QDockWidget *dock = new QDockWidget(tr("GraphItem Info"), this);
	dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	m_textWin = new QListWidget(dock);
	dock->setWidget(m_textWin);
	addDockWidget(Qt::RightDockWidgetArea, dock);

	dock = new QDockWidget(tr("Design Info"), this);
	m_infoWin = new QListWidget(dock);
	dock->setWidget(m_infoWin);
	addDockWidget(Qt::RightDockWidgetArea, dock);
}
// PUBLIC SLOT
void MainWindow::ItemSelected(const Net *_net, DiagramItem::DiagramType _type) {
	QString msg;
	if (_type == DiagramItem::INPUT) {
		msg = "INPUT " + _net->inPinName + "(" + QString().setNum(_net->in.x()) + ","
			    + " " + QString().setNum(_net->in.y()) + ")" + " Slack to FF = " + QString().setNum(_net->slack[0]);
	}
	else if (_type == DiagramItem::OUTPUT) {
		msg	= "OUTPUT " + _net->outPinName + "(" + QString().setNum(_net->out.x()) + ","
					+ " " + QString().setNum(_net->out.y()) + ")" + " Slack to FF = " + QString().setNum(_net->slack[1]);;
	}
	else {
		msg	= "Flip-flop " + _net->ff->selfName + "(" + QString().setNum(_net->ff->pnt.x()) + ", "
					+ QString().setNum(_net->ff->pnt.y()) + ")\n"
					+ "  Bit Number = " + QString().setNum(_net->ffInfo->bNum) 
					+ ", Power Consumption = " + QString().setNum(_net->ffInfo->pConsume) 
					+ ", Area = " + QString().setNum(_net->ffInfo->fArea);
	}
	m_textWin->addItems(QStringList() << msg);
	*pLog->logFile() << "Recieve data from scene" << std::endl;
}
// SLOT FUNCTION
void MainWindow::_LoadInFile() {
	m_ffs.clear();
	pLog->Log("Start Loading Input File");
	m_curFile = QFileDialog::getOpenFileName(this, tr("Open Input File"), "./", tr("Input File (*.input)"));
	if (!m_curFile.isEmpty()) {
		QMap<QString, QPoint> mapInPin;
		QMap<QString, QPoint> mapOutPin;
		QMap<QString, FlipFlop *> mapFFPin;

		const int max_in_size = 1000;
		std::ifstream inFile;
		std::string fName;
		fName = m_curFile.toStdString();
		inFile.open(fName.c_str(), std::ios::in);
		if (!inFile) {
			*pLog->logFile() << fName << " could not be opened. " << std::endl;
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
					*pLog->logFile() << m_ChipSize[0] << ", " << m_ChipSize[1] << std::endl;
					continue;
				}
				if (strncmp(token, "GRID", 4) == 0) {
					token = strtok(NULL, delims);
					m_GridSize[0] = atol(token);
					token = strtok(NULL, delims);
					token = strtok(NULL, delims);
					m_GridSize[1] = atol(token);
					token = strtok(NULL, delims);
					*pLog->logFile() << m_GridSize[0] << ", " << m_GridSize[1] << std::endl;
					continue;
				}
				if (strncmp(token, "BIN", 3) == 0) {
					token = strtok(NULL, delims);
					m_BinSize[0] = atol(token);
					token = strtok(NULL, delims);
					token = strtok(NULL, delims);
					m_BinSize[1] = atol(token);
					token = strtok(NULL, delims);
					*pLog->logFile() << m_BinSize[0] << ", " << m_BinSize[1] << std::endl;
					continue;
				}
				if (strncmp(token, "PLACEMENT", 9) == 0) {
					token = strtok(NULL, delims);
					m_MaxDensity = atol(token);
					*pLog->logFile() << m_MaxDensity << std::endl;
					continue;
				}
				// ======= Library Section ==============
				if (strncmp(token, "[LIBRARY]", 9) == 0) {
					*pLog->logFile() << "Parser Lib: "  << token << std::endl;
					while(strncmp(token, "[END", 4) != 0) { // END LIBRARY
						inFile.getline(buffer, max_in_size, '\n');
						token = strtok(buffer, delims);
						if (strncmp(token, "[FLIP_FLOP_PROPERTY]", 20) == 0) {
							*pLog->logFile() << "Parser FF property: "  << token << std::endl;
							while(strncmp(token, "[END", 4) != 0) { // END FILP_FLOP_PROPERTY
								inFile.getline(buffer, max_in_size, '\n');
								token = strtok(buffer, delims);
								// Fetech a FF library: in GUI, we don't need to store this message
								if (strncmp(token, "[FLIP_FLOP", 10) == 0) {
									*pLog->logFile() << "Parser FF Type: "  << token << std::endl;
									FlipID sfID;
									char fName[64];
									int bit_num = 0,
											power_consume = 0,
											ff_area = 0;

									token = strtok(NULL, delims);
									sscanf(token, "%[a-zA-Z0-9_-]", fName);
									sfID = fName;
									*pLog->logFile() << "FF's type: "  << fName << std::endl;

									inFile.getline(buffer, max_in_size, '\n');
									token = strtok(buffer, delims);
									token = strtok(NULL, delims);
									*pLog->logFile() << "FF's bit: "  << token << std::endl;
									bit_num = atoi(token);

									inFile.getline(buffer, max_in_size, '\n');
									token = strtok(buffer, delims);
									token = strtok(NULL, delims);
									*pLog->logFile() << "FF's power: "  << token << std::endl;
									power_consume = atoi(token);

									inFile.getline(buffer, max_in_size, '\n');
									token = strtok(buffer, delims);
									token = strtok(NULL, delims);
									*pLog->logFile() << "FF's area: "  << token << std::endl;
									ff_area = atoi(token);

									m_flib.insert(sfID, FFsLib(sfID, bit_num, power_consume, ff_area)); 
									inFile.getline(buffer, max_in_size, '\n'); // skip [END FLIP_FLOP]
								}
							}
						}
					}
				}
				// ======= Design Section ==============
				if (strncmp(token, "[DESIGN]", 8) == 0) {
					*pLog->logFile() << "Parser Design: "  << token << std::endl;
					while(strncmp(token, "[END", 4) != 0) { // END DESIGN
						inFile.getline(buffer, max_in_size, '\n');
						token = strtok(buffer, delims);
						// ======= FF LIST Section ==============
						if (strncmp(token, "[FLIP_FLOP_LIST]", 16) == 0) {
							*pLog->logFile() << "Parser FF LIST: "  << token << std::endl;
							while(strncmp(token, "[END", 4) != 0) { // END FILP_FLOP_LIST
								inFile.getline(buffer, max_in_size, '\n');
								token = strtok(buffer, delims);
								// Fetech a FF list
								if (strncmp(token, "FF", 2) == 0) {
									char tName[64], sName[64];
									FlipID sfID, tfID;
									// int _id = -1, _tid = -1;
									unsigned int _x = 0, _y = 0;
									sscanf(token, "%[a-zA-Z0-9_-]", tName);
									tfID = tName;
									token = strtok(NULL, delims);
									//sscanf(token, "FLIP_FLOP%d", &_id); // id
									sscanf(token, "%[a-zA-Z0-9_-]", sName);
									sfID = sName;
									token = strtok(NULL, delims);
									sscanf(token, "(%u", &_x); // axis of x
									token = strtok(NULL, delims);
									sscanf(token, "%u)", &_y); // axis of x
									mapFFPin.insert(sfID , new FlipFlop(_x, _y, sfID, tfID));
									*pLog->logFile() << tName << ", " << sName << ",X(" << _x << "),Y(" << _y << ")\n";
								}
							}
						}
						/// The following two line is avoid the [END FLIP_FLOP_LIST] to break this loop
						inFile.getline(buffer, max_in_size, '\n');
						token = strtok(buffer, delims);
						// ======= PIN LIST Section ==============
						if (strncmp(token, "[PIN_LIST]", 10) == 0) {
							*pLog->logFile() << "Parser PIN LIST: "  << token << std::endl;
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
									mapInPin.insert("PIN" + QString().setNum(_id), QPoint(_x, _y));
									*pLog->logFile() << "PID(" << _id << "),X(" << _x << "),Y(" << _y << ")\n";
								}
								else if (strncmp(token, "OUTPUT", 6) == 0) {
									token = strtok(NULL, delims);
									sscanf(token, "PIN%d", &_id); // id
									token = strtok(NULL, delims);
									sscanf(token, "(%u", &_x); // axis of x
									token = strtok(NULL, delims);
									sscanf(token, "%u)", &_y); // axis of x
									mapOutPin.insert("PIN" + QString().setNum(_id), QPoint(_x, _y));
									*pLog->logFile() << "PID(" << _id << "),X(" << _x << "),Y(" << _y << ")\n";
								}
							}
						}
						// ======= NET LIST Section ==============
						inFile.getline(buffer, max_in_size, '\n');
						token = strtok(buffer, delims);
						if (strncmp(token, "[NET_LIST]", 10) == 0) {
							*pLog->logFile() << "Parser NET LIST: "  << token << std::endl;
							while(strncmp(token, "[END", 4) != 0) { // END NET_LIST
								inFile.getline(buffer, max_in_size, '\n');
								token = strtok(buffer, delims);
							  if (strncmp(token, "[END", 4) == 0) break;

								char pinInName[64], 
										 pinOutName[64], 
										 ffName[64];
								FlipID QpinInName, QpinOutName, QffName;
								unsigned int _slack1 = 0, _slack2 = 0;
								// 假設In-FF的下一行就是Out-FF
								// In-to-FF  
								sscanf(token, "%[a-zA-Z0-9_-]", pinInName);
								QpinInName = pinInName;
								token = strtok(NULL, delims);
								sscanf(token, "%[a-zA-Z0-9_-]", ffName);
								QffName = ffName;
								token = strtok(NULL, delims);
								_slack1 = atol(token);
								*pLog->logFile() << pinInName << ", " << ffName << ", Slack = " << _slack1 << std::endl;
								// Out-to-FF  
								inFile.getline(buffer, max_in_size, '\n');
								token = strtok(buffer, delims);
								sscanf(token, "%[a-zA-Z0-9_-]", pinOutName);
								QpinOutName = pinOutName;
								token = strtok(NULL, delims);
								sscanf(token, "%[a-zA-Z0-9_-]", ffName);
								QffName = ffName;
								token = strtok(NULL, delims);
								_slack2 = atol(token);
								*pLog->logFile() << pinOutName << ", " << ffName << ", Slack = " << _slack2 << std::endl;
								Net *net = new Net();
								// connect to Input pin
								QMap<QString, QPoint>::iterator findInPinItr = mapInPin.find(QpinInName);
								if (findInPinItr != mapInPin.end()) {
									net->in = findInPinItr.value();
									net->inPinName = QpinInName;
									net->slack[0] = _slack1;
								}
								else 
									*pLog->errFile() << QpinInName.toStdString() << " not found in mapInPin" << std::endl;
								// connect to Output pin
								QMap<QString, QPoint>::iterator findOutPinItr = mapOutPin.find(QpinOutName);
								if (findOutPinItr != mapOutPin.end()) {
									net->out = findOutPinItr.value();
									net->outPinName = QpinOutName;
									net->slack[1] = _slack2;
								}
								else 
									*pLog->errFile() << QpinOutName.toStdString() << " not found in mapOutPin" << std::endl;
								// connect to FlipFlop
								QMap<QString, FlipFlop*>::iterator findFFItr = mapFFPin.find(QffName);
								if (findFFItr != mapFFPin.end()) {
									net->ff = findFFItr.value();
									// connect to FlipFlop
									QMap<QString, FFsLib>::iterator findFibItr = m_flib.find(net->ff->typeName);
									if (findFibItr != m_flib.end()) {
										net->ffInfo = &(findFibItr.value());
										m_ffs.insert(QffName, net->ffInfo); 
									}
									else 
										*pLog->errFile() << QffName.toStdString() << " not found in m_flib" << std::endl;
								}
								else 
									*pLog->errFile() << QffName.toStdString() << " not found in mapFFPin" << std::endl;
								// store
								m_net.push_back(net);
//								*pLog->logFile() << "PID(" << _id1 << "),FID(" << _id2 << "),Slack(" << _slack << ")" << std::endl;
							}
						}
						// ======= BLOCK LIST Section ==============
						inFile.getline(buffer, max_in_size, '\n');
						token = strtok(buffer, delims);
						if (strncmp(token, "[BLOCK_LIST]", 12) == 0) {
							*pLog->logFile() << "Parser BLOCK LIST: "  << token << std::endl;
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
									
									// m_scene->addRect(QRect(_x, _y, -50, -50));
									//m_scene->addRect(QRect(_x, _y, 5, 5));

									//m_obs->push_back(new OBPin(_id, OBS, _x, _y, _area));
									*pLog->logFile() << "OID(" << _id << "),X(" << _x << "),Y(" << _y << "),AREA(" << _area << ")" << std::endl;
								}
							}
						}
					}
				}
			}
			this->_DrawInputScene();
		}
		inFile.close();
	}
	pLog->Log("End Loading Input File");
}
void MainWindow::_LoadOutFile() {
	m_ffs.clear();
	m_curFile = QFileDialog::getOpenFileName(this, tr("Open Output File"), "./", tr("Output File (*.output)"));
	if (!m_curFile.isEmpty()) {
		QMap<QString, FlipFlop *> mapFFPin;
		const int max_in_size = 1000;
		std::ifstream inFile;
		std::string fName;
		fName = m_curFile.toStdString();
		inFile.open(fName.c_str(), std::ios::in);
		if (!inFile) {
			*pLog->logFile() << fName << " could not be opened. " << std::endl;
			 QMessageBox::about(this, tr("ERROR!"),
				 tr("file isn't exist..........."));	
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
					*pLog->logFile() << "Parser FF LIST: "  << token << std::endl;
					while(strncmp(token, "[END", 4) != 0) { // END FILP_FLOP_LIST
						inFile.getline(buffer, max_in_size, '\n');
						token = strtok(buffer, delims);
						// Fetech a FF list
						if (strncmp(token, "FF", 2) == 0) {
							char tName[64], sName[64];
							FlipID sfID, tfID;
							unsigned int _x = 0, _y = 0;
							sscanf(token, "%[a-zA-Z0-9_-]", tName);
							tfID = tName;
							token = strtok(NULL, delims);
							//sscanf(token, "FLIP_FLOP%d", &_id); // id
							sscanf(token, "%[a-zA-Z0-9_-]", sName);
							sfID = sName;
							token = strtok(NULL, delims);
							sscanf(token, "(%u", &_x); // axis of x
							token = strtok(NULL, delims);
							sscanf(token, "%u)", &_y); // axis of x
							mapFFPin.insert(sfID , new FlipFlop(_x, _y, sfID, tfID));
							*pLog->logFile() << tName << ", " << sName << ",X(" << _x << "),Y(" << _y << ")\n";
						}
					}
				}
				// ======= NET LIST Section ==============
				if (strncmp(token, "[NET_LIST]", 10) == 0) {
					*pLog->logFile() << "Parser NET LIST: "  << token << std::endl;
					while(strncmp(token, "[END", 4) != 0) { // END NET_LIST
						inFile.getline(buffer, max_in_size, '\n');
						token = strtok(buffer, delims);
						if (strncmp(token, "[END", 4) == 0) break;
						char pinInName[64], 
								 pinOutName[64], 
								 ffName[64];
						FlipID QpinInName, QpinOutName, QffName;
						unsigned int _slack1 = 0, _slack2 = 0;
						// 假設In-FF的下一行就是Out-FF
						// In-to-FF  
						sscanf(token, "%[a-zA-Z0-9_-]", pinInName);
						QpinInName = pinInName;
						token = strtok(NULL, delims);
						sscanf(token, "%[a-zA-Z0-9_-]", ffName);
						QffName = ffName;
						token = strtok(NULL, delims);
						_slack1 = atol(token);
						*pLog->logFile() << pinInName << ", " << ffName << ", Slack = " << _slack1 << std::endl;
						// Out-to-FF  
						inFile.getline(buffer, max_in_size, '\n');
						token = strtok(buffer, delims);
						sscanf(token, "%[a-zA-Z0-9_-]", pinOutName);
						QpinOutName = pinOutName;
						token = strtok(NULL, delims);
						sscanf(token, "%[a-zA-Z0-9_-]", ffName);
						QffName = ffName;
						token = strtok(NULL, delims);
						_slack2 = atol(token);
						*pLog->logFile() << pinOutName << ", " << ffName << ", Slack = " << _slack2 << std::endl;
						Net *net = NULL;
						// 由PIN的名稱去找Net
						foreach (Net *_net, m_net) {
							if (_net->inPinName == QpinInName && _net->outPinName == QpinOutName) {
								net = _net;
								net->slack[0] = _slack1;
								net->slack[1] = _slack2;
								break;
							}
						}
						if (net == NULL) {
							*pLog->errFile() << QpinInName.toStdString() << " not found in mapInPin" << std::endl;
							*pLog->errFile() << QpinOutName.toStdString() << " not found in mapOutPin" << std::endl;
						}
						else {
							// connect to FlipFlop
							QMap<QString, FlipFlop*>::iterator findFFItr = mapFFPin.find(QffName);
							if (findFFItr != mapFFPin.end()) {
								delete net->ff;
								net->ff = findFFItr.value();
								// connect to FlipFlop
								QMap<QString, FFsLib>::iterator findFibItr = m_flib.find(net->ff->typeName);
								if (findFibItr != m_flib.end()) {
									net->ffInfo = &(findFibItr.value());
									m_ffs.insert(QffName, net->ffInfo); 
								}
								else 
									*pLog->errFile() << QffName.toStdString() << " not found in m_flib" << std::endl;
							}
							else 
								*pLog->errFile() << QffName.toStdString() << " not found in mapFFPin" << std::endl;
						}
					}
				}
				// ======= MAX DENSITY Section ==============
				if (strncmp(token, "MAX_PLACEMENT_DENSITY", 21) == 0) {
					unsigned int max_density = 0;
					sscanf(token, "MAX_PLACEMENT_DENSITY=%u", &max_density);
					*pLog->logFile() << "DENSIT: "  << max_density << std::endl;
				}
				// ======= CPU Time Section ==============
				if (strncmp(token, "EXEC", 4) == 0) {
					float exetime = 0.0f;
					sscanf(token, "EXECUTION_TIME=%6f", &exetime);
					*pLog->logFile() << "Execute time: "  << exetime << std::endl;
				}
				// ======= CPU Time Section ==============
				if (strncmp(token, "POWER_", 6) == 0) {
					float ratio = 0.0f;
					sscanf(token, "POWER_CONSUMPTION_RATION=%6f", &ratio);
					*pLog->logFile() << "Power ratio: "  << ratio << std::endl;
				}
			}
			this->_DrawOutputScene();
		}
		inFile.close();
	}
}
void MainWindow::scaleUp() {
	 m_ScaleValue = 150.0*m_ScaleValue / 100.0;
	 QMatrix oldMatrix = m_view->matrix();
	 m_view->resetMatrix();
	 m_view->translate(oldMatrix.dx(), oldMatrix.dy());
	 m_view->scale(m_ScaleValue, m_ScaleValue);
}
void MainWindow::scaleDown() {
	 m_ScaleValue = 50.0*m_ScaleValue / 100.0;
	 QMatrix oldMatrix = m_view->matrix();
	 m_view->resetMatrix();
	 m_view->translate(oldMatrix.dx(), oldMatrix.dy());
	 m_view->scale(m_ScaleValue, m_ScaleValue);
}
void MainWindow::scaleNormal() {
	 m_ScaleValue = (0.158203*3000) / _max(m_ChipSize[0], m_ChipSize[1]);
	 QMatrix oldMatrix = m_view->matrix();
	 m_view->resetMatrix();
	 m_view->translate(oldMatrix.dx(), oldMatrix.dy());
	 m_view->scale(m_ScaleValue, m_ScaleValue);
}
void MainWindow::_ClearScene() {
	m_ScaleValue = 1; 
	m_textWin->clear();
	m_infoWin->clear();

	m_scene->clear();
	m_view->resetMatrix();
	foreach(Net *_net, m_net) {
		delete _net;
	}
	m_net.clear();
}
void MainWindow::_DrawInputScene() {
	this->_DrawNet();
	this->_DrawFrame();
	m_scene->update();
	m_view->update();
	this->WriteDesignInfo();
}
void MainWindow::_DrawOutputScene() {
	if (m_net.size() == 0) {
		 QMessageBox::about(this, tr("Warning!"), tr("Please open input file first, 3Q"));	
	}
	else {
		m_textWin->clear();
		m_scene->clear();
		this->_DrawNet();
		this->_DrawFrame();
		m_scene->update();
		m_view->update();
		this->WriteDesignInfo();
	}
}
void MainWindow::_DrawFrame() {
	// outer rectangle
	m_scene->addLine(QLine(0, 0, m_ChipSize[0], 0));
	m_scene->addLine(QLine(0, 0, 0, m_ChipSize[1]));
	m_scene->addLine(QLine(0, m_ChipSize[1], m_ChipSize[0], m_ChipSize[1]));
	m_scene->addLine(QLine(m_ChipSize[0], 0, m_ChipSize[0], m_ChipSize[1]));
	// bin frame
	for (int i = 1; i < (m_ChipSize[0] / m_BinSize[0]); ++i) {
		//myLine = new LineItem(QPoint(i*m_BinSize[0], 0), QPoint(i*m_BinSize[0], m_ChipSize[1]));
		//myLine->setPen(QPen(color));
		//myLine->setEnable(false);
		//m_scene->addItem(myLine);
		m_scene->addLine(QLine(i*m_BinSize[0], 0, 
					i*m_BinSize[0], m_ChipSize[1])); 
	}
	for (int j = 1; j < (m_ChipSize[1] / m_BinSize[1]); ++j) {
		//myLine = new LineItem(QPoint(0, j*m_BinSize[1]), QPoint(m_ChipSize[0], j*m_BinSize[0]));
		//myLine->setPen(QPen(color));
		//myLine->setEnable(false);
		//m_scene->addItem(myLine);
		m_scene->addLine(QLine(0, j*m_BinSize[1], 
					m_ChipSize[0], j*m_BinSize[0])); 
	}
}
QPoint _getCorner(QPoint p1, QPoint p2) {
	return QPoint( _min(p1.x(), p2.x()), _max(p1.y(), p2.y()));
}
void MainWindow::_DrawNet() {
	DiagramItem *myIPinShape = NULL,
							*myOPinShape = NULL,
							*myFPinShape = NULL;
	LineItem *myLine = NULL;
	QList<Net*>::const_iterator netcItr = m_net.begin();
	while (netcItr != m_net.end()) {
		QColor color(static_cast<int>(qrand()) % 256,
			 static_cast<int>(qrand()) % 256, static_cast<int>(qrand()) % 256);
		const Net *_net = *netcItr;
		myLine = new LineItem(_net->in, _net->ff->pnt);
		myLine->setPen(QPen(color));
		//myLine->setEnable(false);
		m_scene->addItem(myLine);
		myLine = new LineItem(_net->ff->pnt, _net->out);
		myLine->setPen(QPen(color));
		m_scene->addItem(myLine);
		// For input pin
		myIPinShape = new DiagramItem(DiagramItem::INPUT, _net);
		myIPinShape->setPos(_net->in);
		myIPinShape->setBrush(QBrush(color));
		myIPinShape->rotate(90);
		m_scene->addItem(myIPinShape);
		// For FF
		myFPinShape = new DiagramItem(DiagramItem::FF, _net);
		myFPinShape->setPos(_net->ff->pnt);
		myFPinShape->setBrush(QBrush(color));
		m_scene->addItem(myFPinShape);
		// For output pin
		myOPinShape = new DiagramItem(DiagramItem::OUTPUT, _net);
		myOPinShape->setPos(_net->out);
		myOPinShape->rotate(-90);
		myOPinShape->setBrush(QBrush(color));
		m_scene->addItem(myOPinShape);
		++netcItr; 
	}
}
void  MainWindow::WriteDesignInfo() {
	QString msg;
	msg = "Load file: " + m_curFile;
	msg += "\n#INPUT PIN = " + QString().setNum(m_net.size());
	msg += "\n#OUTPUT PIN = " + QString().setNum(m_net.size());
	msg += "\n#Flip-flop  = " + QString().setNum(m_ffs.size());
	unsigned int totalPower = 0;
	foreach (QString str, m_ffs.keys()) {
		totalPower += m_ffs.value(str)->pConsume;
	}
	msg += "\nTotal Power = " + QString().setNum(totalPower);
	m_infoWin->addItems(QStringList() << msg);
}

#ifndef DEF_H
#define DEF_H

#include <QGraphicsLineItem>
#include "logger.h"

typedef QString FlipID;
class FFsLib {
	public:
		QString fName;    // type of flip flop
		int bNum;     // bit number
		int pConsume; // power consumption
		int fArea;    // flip-flop area
		FFsLib(QString _name, int _bits, int _consume, int _area) {
			fName = _name;
			bNum = _bits;
			pConsume = _consume;
			fArea = _area;
		}
		bool operator<(const FFsLib &ffs) const { 
			return (bNum < ffs.bNum); }
};
class FlipFlop {
	public:
		QPoint  pnt;
		QString selfName;
		QString typeName;
		FlipFlop(int x, int y,QString _sName, QString _tName) {
			pnt.setX(x);
			pnt.setY(y);
			selfName = _sName;
			typeName = _tName;
		};
		bool operator<(const FlipFlop &fp) const { 
			return (selfName < fp.selfName); }
};
class Net {
	public:
		QPoint in;
		QPoint out;
		FlipFlop *ff;
		FFsLib *ffInfo;
		QString inPinName;
		QString outPinName;
		int slack[2];
};
class LineItem: public QGraphicsLineItem {
	public:
		LineItem(QPoint p1, QPoint p2) {
			m_line.setPoints(p1, p2);
			setLine(m_line);
		}
	private:
		QLineF m_line;
};

extern Logger *pLog;

#endif // DEF_H

#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QGraphicsPolygonItem>
#include "def.h"

class DiagramItem : public QGraphicsPolygonItem
{
	public:
		enum { Type  = UserType + 1 };
		enum DiagramType { INPUT, OUTPUT, FF};
		DiagramItem(DiagramType diagramType, const Net *_net);
		inline const Net* getNet() { return m_net; }
    int type() const { return Type; }
		inline DiagramType getPinType() { return m_type; }

	private:
		DiagramType m_type;
		const Net *m_net;
		QPolygonF m_poly;
};

#endif // DIAGRAMITEM_H

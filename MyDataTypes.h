#include <QVector>


#include "Element.h"

#define QTEXT(a) QString::fromLocal8Bit(a)

class Element;
class Connection;
class XPoint;

using PairInt = QPair<int, int>;
using ConVect = QVector<Connection*>;
using ElemVect = QVector<Element*>;
using HistoryVect = QVector<PairInt>;
using VectOfPairRects = QVector<QPair<int, QRect> >;
using VectOfPoints = QVector<QPointF>;
using VOfVOfPoints = QVector<QVector<QPointF> >;
using BranchVect = QVector<QVector<PairInt> >;
using VectFloat = QVector<float>;
using VectLines = QVector<QLine>;

#include "GraphWidget.h"

#include <QPainter>
#include <QTimer>
#include <QPaintEvent>
#include <math.h>

#include <random>
#include <iostream>

GraphWidget::GraphWidget(QWidget *parent)
    : QWidget(parent), graph(nullptr)
{
    circleBrush = QBrush(Qt::black);
    circlePen = QPen(Qt::black);
    textPen = QPen(Qt::white);
    highlightPen = QPen(Qt::red, 2);
}

void GraphWidget::setGraph(Graph *graph)
{
    this->graph = graph;
}

void GraphWidget::visualize()
{
    update();
}

void GraphWidget::highlightNode(int nodeValue)
{
    highlightedNodes.insert(nodeValue);
    update();
}

void GraphWidget::highlightEdge(int source, int destination)
{
    highlightedEdges.insert(std::make_pair(source, destination));
    update();
}

void GraphWidget::unHighlightNode(int nodeValue)
{
    highlightedNodes.erase(nodeValue);
    update();
}

void GraphWidget::unHighlightEdge(int source, int destination)
{
    highlightedEdges.erase(std::make_pair(source, destination));
    update();
}

void GraphWidget::unHighlightAll() {
    highlightedEdges.clear();
    highlightedNodes.clear();
    update();
}

void GraphWidget::paintEvent(QPaintEvent *event)
{
    if (!graph) return;

    QPainter painter;
    painter.begin(this);
    painter.setPen(circlePen);

    auto nodeCoords = calculateNodeCoordinates(event);

    painter.save();
    for (int i = 0; i < nodeCoords.size(); i++) {
        auto nodeRect = QRect(nodeCoords[i], NODE_RECT_SIZE);

        painter.setBrush(circleBrush);
        painter.setPen(circlePen);
        // Draw node's circle
        painter.drawEllipse(nodeRect);

        Node* destHead = graph->head[i];
        while (destHead != nullptr) {
            auto destCoords = QPoint(nodeCoords[destHead->getValue()]); // center of destination node
            auto destRect = QRect(destCoords, NODE_RECT_SIZE);
            auto sourceDestLine = QLine(nodeRect.center(), destRect.center());

            QLineF borderToBorder;
            // Draw edges and arrow heads
            if (highlightedEdges.find(std::make_pair(i, destHead->getValue())) != highlightedEdges.end()) {
                // Edge should be highlighted
                painter.setPen(highlightPen);
                borderToBorder = drawEdge(&painter, sourceDestLine, nodeRect);
                painter.setPen(circlePen);
            } else {
                // Normal edge
                borderToBorder = drawEdge(&painter, sourceDestLine, nodeRect);
            }

            // Draw weight along the edge
            // Get bounding rect depending on weight and font
            QFontMetrics fontMetrics(painter.font());
            auto weightBoundingRect = fontMetrics.boundingRect(QString::number(destHead->getCost()));
            // Accomodate space for border
            weightBoundingRect.setWidth(weightBoundingRect.width() + 4);
            weightBoundingRect.setHeight(weightBoundingRect.height() + 2);
            weightBoundingRect.translate(-2, -1);

            // Divide the edge into 6 "bands" and place the weight on a band depending on source and dest values
            // In order to avoid overlap of weights when possible
            qreal pointAlongEdget = float(((i + destHead->getValue()) % 6) + 1) / 7;
            QPointF pointAlongEdge = borderToBorder.pointAt(pointAlongEdget);
            QPoint weightRectTopLeft(pointAlongEdge.x() - weightBoundingRect.width() / 2, pointAlongEdge.y() - weightBoundingRect.height() / 2);
            QRect weightRect = QRect(weightRectTopLeft, weightBoundingRect.size());

            painter.save();
            painter.setPen(QPen(circleBrush, 1));
            painter.setBrush(QBrush(Qt::white));
            painter.drawRect(weightRect);
            painter.setPen(QPen(Qt::darkGreen));
            painter.drawText(weightRect, Qt::AlignCenter, QString::number(destHead->getCost()));
            painter.restore(); // restore previous pen and brush

            destHead = destHead->getNextNode();
        }
    }
    painter.restore();

    // Draw highlighted nodes
    for (int node : highlightedNodes) {
        painter.setPen(highlightPen);
        auto nodeRect = QRect(nodeCoordinates[node], NODE_RECT_SIZE);
        painter.drawEllipse(nodeRect);
    }

    // Draw node's numbers at the end so that they are on top
    for (int i = 0; i < nodeCoords.size(); i++) {
        auto nodeRect = QRect(nodeCoords[i], NODE_RECT_SIZE);
        painter.setPen(textPen);
        painter.drawText(nodeRect, Qt::AlignCenter, QString::number(i));
    }

    painter.end();
}

std::vector< QPoint > GraphWidget::calculateNodeCoordinates(QPaintEvent *event) {
    // Calculate each node's center point
    int nodeCount = graph->getCurrentNodeCount();
    int cenX = event->rect().center().x();
    int cenY = event->rect().center().y();
    std::vector< QPoint > nodeCoords;
    int x = 0;
    int y = 12*(nodeCount);

    nodeCoords.push_back(QPoint(x+cenX, y+cenY));
    for (int i = 1; i < nodeCount; i++) {
        int newX = x*cos(2*M_PI/nodeCount) - y*sin(2*M_PI/nodeCount);
        y=(y*cos(2*M_PI/nodeCount) + x*sin(2*M_PI/nodeCount));
        x = newX;
        nodeCoords.push_back(QPoint(x+cenX, y+cenY));
    }
    nodeCoordinates = nodeCoords;
    return nodeCoords;
}

QLineF GraphWidget::drawEdge(QPainter *painter, QLine sourceDestLine, QRect sourceNodeRect) {
    QLineF sourceDestLineF = sourceDestLine.toLineF();
    // The line should begin at border of node, not center
    // Position should be a fraction of the line's length due to using QPointF::pointAt()
    qreal lineBeginningt = (sourceNodeRect.height() / 2) / sourceDestLineF.length();
    QPointF arrowHeadEnd = sourceDestLineF.pointAt(1 - lineBeginningt);
    //distance from center of source node to border of destination node
    QLineF sourceNodeBorderLineF = QLineF(sourceDestLineF.p1(), arrowHeadEnd);

    qreal arrowHeadBaset = 1 - (ARROW_HEAD_LENGTH / sourceNodeBorderLineF.length());
    QPointF arrowHeadBase = sourceNodeBorderLineF.pointAt(arrowHeadBaset);

    // perpendicular to line starting from arrowhead base
    auto arrowNormal = QLineF(arrowHeadBase, sourceNodeBorderLineF.p2()).normalVector();
    painter->drawLine(arrowHeadEnd, arrowNormal.p2());
    arrowNormal.setAngle(arrowNormal.angle() - 180); // rotate 180
    painter->drawLine(arrowHeadEnd, arrowNormal.p2());

    // draw line from border of source node to border of destination node
    QLineF borderToBorder(sourceDestLineF.pointAt(lineBeginningt), arrowHeadEnd);
    painter->drawLine(borderToBorder);
    return borderToBorder;
}

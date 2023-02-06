#include "GraphWidget.h"

#include <QPainter>
#include <QTimer>
#include <QPaintEvent>
#include <math.h>

#include <iostream>

GraphWidget::GraphWidget(QWidget *parent)
    : QWidget(parent), graph(nullptr)
{
    circleBrush = QBrush(Qt::black);
    circlePen = QPen(Qt::black);
    textPen = QPen(Qt::white);
    highlightPen = QPen(Qt::red, 2);
}

void GraphWidget::setGraph(Graph *graph) {
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
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.begin(this);
    painter.setPen(circlePen);
    auto nodeCoords = calculateNodeCoordinates(event);

    painter.save();
    for (int i = 0; i < nodeCoords.size(); i++) {
        auto nodeRect = QRect(nodeCoords[i], NODE_RECT_SIZE);

        painter.setBrush(circleBrush);
        painter.setPen(circlePen);
        painter.drawEllipse(nodeRect);

        Node* destHead = graph->head[i];
        while (destHead != nullptr) {
            auto destCoords = QPoint(nodeCoords[destHead->getValue()]);
            auto destRect = QRect(destCoords, NODE_RECT_SIZE);
            auto sourceDestLine = QLine(nodeRect.center(), destRect.center());
            // Draw edge and weight
            painter.drawText(sourceDestLine.center(), QString::number(destHead->getCost()));

            // Draw edges and arrow heads
            if (highlightedEdges.find(std::make_pair(i, destHead->getValue())) != highlightedEdges.end()) {
                // Edge should be highlighted
                painter.setPen(highlightPen);
                drawEdge(&painter, sourceDestLine, nodeRect);
                painter.setPen(circlePen);
            } else {
                // Normal edge
                drawEdge(&painter, sourceDestLine, nodeRect);
            }

            destHead = destHead->getNextNode();
        }
    }
    painter.restore();

    // Draw highlights
    painter.save();
    for (int node : highlightedNodes) {
        painter.setPen(highlightPen);
        auto nodeRect = QRect(nodeCoordinates[node], NODE_RECT_SIZE);
        painter.drawEllipse(nodeRect);
    }

    painter.restore();

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

void GraphWidget::drawEdge(QPainter *painter, QLine sourceDestLine, QRect sourceNodeRect) {
    QLineF sourceDestLineF = sourceDestLine.toLineF();
    // The line should begin at border of node, not center
    // Position should be a fraction of the line's length due to using QPointF::pointAt()
    qreal lineBeginningt = (sourceNodeRect.height() / 2) / sourceDestLineF.length();
    qreal arrowHeadEndt = 1 - lineBeginningt;
    QPointF arrowHeadEnd = sourceDestLineF.pointAt(arrowHeadEndt);
    QLineF sourceNodeBorderLineF = QLineF(sourceDestLineF.p1(), arrowHeadEnd);

    qreal arrowHeadBaset = 1 - (ARROW_HEAD_LENGTH / sourceNodeBorderLineF.length());
    QPointF arrowHeadBase = sourceNodeBorderLineF.pointAt(arrowHeadBaset);

    auto arrowNormal = QLineF(arrowHeadBase, sourceNodeBorderLineF.p2()).normalVector();
    painter->drawLine(arrowHeadEnd, arrowNormal.p2());
    arrowNormal.setAngle(arrowNormal.angle() - 180); // rotate 180
    painter->drawLine(arrowHeadEnd, arrowNormal.p2());

    painter->drawLine(QLineF(sourceDestLineF.pointAt(lineBeginningt), arrowHeadEnd));
}

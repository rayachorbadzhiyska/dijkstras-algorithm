#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QWidget>
#include <QPen>
#include <vector>
#include <set>
#include "Graph.h"


class GraphWidget : public QWidget
{
    Q_OBJECT

public:
    GraphWidget(QWidget *parent);
    void setGraph(Graph *graph);

public slots:
    // Visualize the graph by repainting it
    void visualize();
    // Redraws the graph and highlights a node
    void highlightNode(int nodeValue);
    // Redraws the graph and highlights an edge
    void highlightEdge(int source, int destination);
    // Redraws the graph and removes the highlight from node
    void unHighlightNode(int nodeValue);
    // Redraws the graph and removes the highlight from edge
    void unHighlightEdge(int source, int destination);
    // Redraws the graph and removes all highlights
    void unHighlightAll();

protected:
    // Called on update()
    void paintEvent(QPaintEvent *event) override;

private:
    // Arranges all graph nodes on a single circle and returns coordinates of each node
    std::vector< QPoint > calculateNodeCoordinates(QPaintEvent *event);
    // Draws a single edge, consisting of a line and arrow head
    // Returns line from source node border to dest node border
    QLineF drawEdge(QPainter *painter, QLine sourceDestLine, QRect sourceNodeRect);

    QBrush circleBrush;
    QPen circlePen;
    QPen textPen;
    QPen highlightPen;

    Graph *graph;
    std::vector< QPoint > nodeCoordinates;
    std::set< int > highlightedNodes;
    std::set< std::pair< int, int > > highlightedEdges;

    const QSize NODE_RECT_SIZE = QSize(20, 20);
    const int ARROW_HEAD_LENGTH = 10;
};


#endif // GRAPHWIDGET_H

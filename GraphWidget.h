#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QWidget>
#include <QPen>
#include <vector>
#include <set>
#include <QTimer>
#include "Graph.h"

class GraphWidget : public QWidget
{
    Q_OBJECT

public:
    /*!
     * GraphWidget constructor
     *
     * \param parent The parent widget
     */
    GraphWidget(QWidget *parent);

    /*!
     *  Set which graph the widget will visualize
     *
     * \param graph The graph
     */
    void setGraph(Graph *graph);

    /*!
     * Function used to schedule a timer to draw a path that had been processed in the algorithm
     *
     * \param source Path's source
     * \param destination Path's destination
     */
    void scheduleTimerForDrawingPath(int source, int destination);

public slots:
    /*!
     *  Visualize the graph by repainting it
     *
     */
    void visualize();

    /*!
     *  Redraws the graph and highlights a node
     *
     * \param nodeValue Node to be highlighted
     */
    void highlightNode(int nodeValue);

    /*!
     *  Redraws the graph and highlights an edge
     *
     * \param source Source node of edge
     * \param destination Destination node of edge
     */
    void highlightEdge(int source, int destination);

    /*!
     *  Redraws the graph and removes the highlight from node
     *
     * \param nodeValue Node to be unhighlighted
     */
    void unHighlightNode(int nodeValue);

    /*!
     *  Redraws the graph and removes the highlight from edge
     *
     * \param source Source node of edge
     * \param destination Destination node of edge
     */
    void unHighlightEdge(int source, int destination);

    /*!
     *  Redraws the graph and removes all highlights
     *
     */
    void unHighlightAll();

protected:
    /*! Called on update()
     *
     * \param event The QPaintEvent
     */
    void paintEvent(QPaintEvent *event) override;

private:
    /*!
     * Arranges all graph nodes on a single circle and returns coordinates of each node
     *
     * \param event The paint event, needed to take use its center as anchor
     * \return Vector of QPoints, each point being the center of a node
     */
    std::vector< QPoint > calculateNodeCoordinates(QPaintEvent *event);

    /*!
     * Draws a single edge, consisting of a line and arrow head
     * The visible line extends from the source node border to destination node border
     *
     * \param painter The painter used to draw
     * \param sourceDestLine Line from source node center to destination node center
     * \param sourceNodeRect Bounding rectangle of source node
     * \return Line from source node border to dest node border
     */
    QLineF drawEdge(QPainter *painter, QLine sourceDestLine, QRect sourceNodeRect);

    // Brushes and pens for drawing
    QBrush circleBrush;
    QPen circlePen;
    QPen textPen;
    QPen highlightPen;

    Graph *graph;
    std::vector< QPoint > nodeCoordinates;
    /*!
     * Holds all nodes which are currently highlighted
     */
    std::set< int > highlightedNodes;
    /*!
     * Holds all edges which are currently highlighted
     */
    std::set< std::pair< int, int > > highlightedEdges;

    /*!
     * An array of timers used to start a drawing process of path between source and destination;
     * An index in the array corresponds to a node's value
     */
    std::vector<QTimer*> timers;

    const QSize NODE_RECT_SIZE = QSize(20, 20);
    const int ARROW_HEAD_LENGTH = 10;
};


#endif // GRAPHWIDGET_H

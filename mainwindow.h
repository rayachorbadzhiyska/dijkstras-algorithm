#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Graph.h"
#include "GraphWidget.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/*!
 * Represents the main window.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /*!
     * Default constructor, sets up the UI.
     * \param parent he widget's parent.
     */
    MainWindow(QWidget *parent = nullptr);
    /*!
     * Destructor, deallocates memory.
     */
    ~MainWindow();
    /*!
     * Gets executed when the window is opened, contains the initial setup of the UI elements.
     */
    void showEvent(QShowEvent *event);

public slots:
    /*!
     * Slot, updating the current edge's label in the UI.
     * \param newValue The new value of the current edge count.
     */
    void updateCurrentEdgeLabel(int newValue);

private slots:
    /*!
     * Slot, connected to the text changed event of the core input, determines the enablement of the visualize graph button.
     */
    void onCoreInputChanged();
    /*!
     * Slot, executed when the visualize graph button is clicked.
     */
    void on_visualizeGraphButton_clicked();

    /*!
     * Slot, connected to the text changed event of the edge input, determines the enablement of the add edge button.
     */
    void onEdgeInputChanged();
    /*!
     * Slot, executed when the add edge button is clicked.
     */
    void on_addEdgeButton_clicked();

    /*!
     * Slot, connected to the text changed event of the dijkstra input, determines the enablement of the dijkstra button.
     */
    void onDijkstraInputChanged();
    /*!
     * Slot, executed when the dijkstra button is clicked.
     */
    void on_dijkstraButton_clicked();

    /*!
     * Slot, executed when the save button is clicked.
     */
    void on_saveButton_clicked();
    /*!
     * Slot, executed when the open button is clicked.
     */
    void on_openButton_clicked();

private:
    Ui::MainWindow *ui;
    /*!
     * Widget responsible for visualizing the graph
     */
    GraphWidget *graphWidget;

    int nodesCount;
    int edgesCount;
    int source;
    int destination;
    int weight;

    Graph *graph;

    /*!
     * Sets the validation of the input line edits.
     */
    void setValidation();
    /*!
     * Enables/disables the core input, depending on the flag, provided as a parameter.
     */
    void toggleCoreInput(bool flag);
    /*!
     * Enables/disables the edge input, depending on the flag, provided as a parameter.
     */
    void toggleEdgeInput(bool flag);
    /*!
     * Enables/disables the dijkstra input, depending on the flag, provided as a parameter.
     */
    void toggleDijkstraInput(bool flag);
    /*!
     * Enables/disables the open button, depending on the flag, provided as a parameter.
     */
    void toggleOpenButton(bool flag);
    /*!
     * Enables/disables the save button, depending on the flag, provided as a parameter.
     */
    void toggleSaveButton(bool flag);
    /*!
     * Connects the input slots to the corresponding signals.
     */
    void connectInputSlots();
    /*!
     * Redraws the graph widget and updates the list of edges and weights
     */
    void updateGraphVisualization();

    /*!
     * Pushes all edges (with their weights) to an QListWidget in the UI
     * in case two weights are overlapping on the graph visualization
     */
    void populateEdgeList();
};

#endif // MAINWINDOW_H

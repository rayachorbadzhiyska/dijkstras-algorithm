#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Graph.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

///Represents the main window.
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /// Default constructor, sets up the UI.
    MainWindow(QWidget *parent = nullptr);
    /// Destructor, deletes the UI.
    ~MainWindow();
    /// Gets executed when the window is opened, contains the initial setup of the UI elements.
    void showEvent(QShowEvent *event);

public slots:
    /// Slot, updating the current edge's label in the UI.
    void updateCurrentEdgeLabel(int newValue);

private slots:
    /// Slot, connected to the text changed event of the core input, determines the enablement of the visualize graph button.
    void onCoreInputChanged();
    /// Slot, executed when the visualize graph button is clicked.
    void on_visualizeGraphButton_clicked();

    /// Slot, connected to the text changed event of the edge input, determines the enablement of the add edge button.
    void onEdgeInputChanged();
    /// Slot, executed when the add edge button is clicked.
    void on_addEdgeButton_clicked();

    /// Slot, connected to the text changed event of the dijkstra input, determines the enablement of the dijkstra button.
    void onDijkstraInputChanged();
    /// Slot, executed when the dijkstra button is clicked.
    void on_dijkstraButton_clicked();

private:
    Ui::MainWindow *ui;

    int nodesCount;
    int edgesCount;
    int source;
    int destination;
    int weight;

    Graph *graph;

    /// Sets the validation of the input line edits.
    void setValidation();
    /// Enables/disables the core input, depending on the flag, provided as a parameter.
    void toggleCoreInput(bool flag);
    /// Enables/disables the edge input, depending on the flag, provided as a parameter.
    void toggleEdgeInput(bool flag);
    /// Enables/disables the dijkstra input, depending on the flag, provided as a parameter.
    void toggleDijkstraInput(bool flag);
    /// Connects the input slots to the corresponding signals.
    void connectInputSlots();
};

#endif // MAINWINDOW_H

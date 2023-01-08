#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "DijkstraInputException.h"
#include "Graph.h"
#include "QRegularExpressionValidator"
#include "QMessageBox"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);

    //Set the input validation.
    setValidation();

    //Disable all except core input.
    ui->visualizeGraphButton->setDisabled(true);
    disableEdgeInput(true);
    disableDijkstraInput(true);

    //Connect input slots.
    connectInputSlots();
}

#define VALIDATION {

void MainWindow::setValidation()
{
    ui->nodesCountText->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9]*")));
    ui->edgesCountText->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9]*")));

    ui->edgeSourceText->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9]*")));
    ui->edgeDestinationText->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9]*")));
    ui->edgeWeightText->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9]*")));

    ui->dijkstraSourceText->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9]*")));
    ui->dijkstraDestinationText->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9]*")));
}

#define VALIDATIONEND }

#define DISABLEINPUT {

void MainWindow::disableCoreInput(bool flag)
{
    ui->nodesCountText->setDisabled(flag);
    ui->edgesCountText->setDisabled(flag);
    ui->visualizeGraphButton->setDisabled(flag);
}

void MainWindow::disableEdgeInput(bool flag)
{
    ui->edgeSourceText->setDisabled(flag);
    ui->edgeDestinationText->setDisabled(flag);
    ui->edgeWeightText->setDisabled(flag);
    ui->addEdgeButton->setDisabled(flag);
}

void MainWindow::disableDijkstraInput(bool flag)
{
    ui->dijkstraSourceText->setDisabled(flag);
    ui->dijkstraDestinationText->setDisabled(flag);
    ui->dijkstraButton->setDisabled(flag);
}

#define DISABLEINPUTEND }

#define SLOTS {

void MainWindow::updateCurrentEdgeLabel(int newValue)
{
    //Compose the current edge label in the following format: [current / all], e.g [1 / 10].

    QString text = (new QString("%1 / %2"))->arg(newValue).arg(edgesCount);
    ui->currentEdgeNumberLabel->setText(text);
}

void MainWindow::onCoreInputChanged()
{
    //Enable the visualizeGraphButton only if the core input has been filled already (no empty line edits).

    bool isInputFilled = !ui->nodesCountText->text().isEmpty() && !ui->edgesCountText->text().isEmpty();

    ui->visualizeGraphButton->setEnabled(isInputFilled);
}

void MainWindow::on_visualizeGraphButton_clicked()
{
    int nodes = ui->nodesCountText->text().toInt();
    int edges = ui->edgesCountText->text().toInt();

    //If there are N nodes, there are N - 1 directed edges that can lead from a node,
    //therefore the maximum number of edges is N * (N - 1).
    int maximumEdgesCount = nodes * (nodes - 1);

    try
    {
        if (edges > maximumEdgesCount)
        {
            throw DijkstraInputException("Maximum number of edges exceeded.");
        }

        nodesCount = nodes;
        edgesCount = edges;

        graph = new Graph(nodes, edges);

        connect(graph, SIGNAL(currentEdgeCountValueChanged(int)), this, SLOT(updateCurrentEdgeLabel(int)));
        graph->setCurrentEdgeCount(0);

        //TODO: visualize graph

        disableEdgeInput(false);
        disableCoreInput(true);
    }
    catch (const DijkstraInputException& ex)
    {
        QMessageBox errorMessageBox;
        errorMessageBox.critical(this, "Error", ex.what());
    }
}

void MainWindow::onEdgeInputChanged()
{
    //Enable the addEdgeButton only if the edge input has been filled already (no empty line edits).

    bool isInputFilled = !ui->edgeSourceText->text().isEmpty()
            && !ui->edgeDestinationText->text().isEmpty()
            && !ui->edgeWeightText->text().isEmpty();

    ui->addEdgeButton->setEnabled(isInputFilled);
}

void MainWindow::on_addEdgeButton_clicked()
{
    int source = ui->edgeSourceText->text().toInt();
    int destination = ui->edgeDestinationText->text().toInt();
    int weight = ui->edgeWeightText->text().toInt();

    try
    {
        Edge* edge = new Edge(source, destination, weight);
        graph->addEdge(edge);

        //TODO: add visualization of the edge on the graph

        //If all the edges have been filled already,
        //the ability to input more should be disabled.
        if (graph->getCurrentEdgeCount() == edgesCount)
        {
            disableEdgeInput(true);
            disableDijkstraInput(false);
        }
    }
    catch (const DijkstraInputException& ex)
    {
        QMessageBox errorMessageBox;
        errorMessageBox.critical(this, "Error", ex.what());
    }
}

void MainWindow::onDijkstraInputChanged()
{
    //Enable the dijkstraButton only if the dijkstra input has been filled already (no empty line edits).

    bool isInputFilled = !ui->dijkstraSourceText->text().isEmpty() && !ui->dijkstraDestinationText->text().isEmpty();

    ui->dijkstraButton->setEnabled(isInputFilled);
}

void MainWindow::on_dijkstraButton_clicked()
{
    //TODO: display shortest path with dijkstra's algorithm
}

void MainWindow::connectInputSlots()
{
    //Core input slots
    connect(ui->nodesCountText, SIGNAL(textChanged(QString)), this, SLOT(onCoreInputChanged()));
    connect(ui->edgesCountText, SIGNAL(textChanged(QString)), this, SLOT(onCoreInputChanged()));

    //Edge input slots
    connect(ui->edgeSourceText, SIGNAL(textChanged(QString)), this, SLOT(onEdgeInputChanged()));
    connect(ui->edgeDestinationText, SIGNAL(textChanged(QString)), this, SLOT(onEdgeInputChanged()));
    connect(ui->edgeWeightText, SIGNAL(textChanged(QString)), this, SLOT(onEdgeInputChanged()));

    //Dijkstra input slots
    connect(ui->dijkstraSourceText, SIGNAL(textChanged(QString)), this, SLOT(onDijkstraInputChanged()));
    connect(ui->dijkstraDestinationText, SIGNAL(textChanged(QString)), this, SLOT(onDijkstraInputChanged()));
}

#define SLOTSEND }

MainWindow::~MainWindow()
{
    delete ui;
}


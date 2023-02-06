#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "DijkstraInputException.h"
#include "Graph.h"
#include "QRegularExpressionValidator"
#include "QMessageBox"
#include <QFileDialog>
#include <QTimer>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , graph(nullptr)
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
    toggleEdgeInput(false);
    toggleDijkstraInput(false);
    toggleSaveButton(false);
    toggleOpenButton(true);
    graphWidget = new GraphWidget(this);
    ui->graphLayout->addWidget(graphWidget, 0, 0);

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

void MainWindow::toggleCoreInput(bool flag)
{
    ui->nodesCountText->setEnabled(flag);
    ui->edgesCountText->setEnabled(flag);
    ui->visualizeGraphButton->setEnabled(flag);
}

void MainWindow::toggleEdgeInput(bool flag)
{
    ui->edgeSourceText->setEnabled(flag);
    ui->edgeDestinationText->setEnabled(flag);
    ui->edgeWeightText->setEnabled(flag);
    ui->addEdgeButton->setEnabled(flag);
}

void MainWindow::toggleDijkstraInput(bool flag)
{
    ui->dijkstraSourceText->setEnabled(flag);
    ui->dijkstraDestinationText->setEnabled(flag);
    ui->dijkstraButton->setEnabled(flag);
}

void MainWindow::toggleSaveButton(bool flag) {
    ui->saveButton->setEnabled(flag);
}

void MainWindow::toggleOpenButton(bool flag) {
    ui->openButton->setEnabled(flag);
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

        graphWidget->setGraph(graph);
        graphWidget->visualize();

        toggleEdgeInput(true);
        toggleCoreInput(false);
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
            toggleEdgeInput(false);
            toggleDijkstraInput(true);
            toggleSaveButton(true);
            graphWidget->visualize();
            populateEdgeList();
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

void MainWindow::testUnhighlight() {
//    graphWidget->unHighlightNode(2);
    graphWidget->unHighlightAll();
}

void MainWindow::on_dijkstraButton_clicked()
{
    //TODO: display shortest path with dijkstra's algorithm
    graphWidget->highlightNode(2);
    graphWidget->highlightEdge(3, 2);
    QTimer::singleShot(1000, this, &MainWindow::testUnhighlight);
}

void MainWindow::on_saveButton_clicked()
{
    if (!graph)
        return;

    QString filename= QFileDialog::getSaveFileName(this, "Save As");

    if (filename.isEmpty())
        return;

    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::ReadWrite)) {
        QTextStream out(&file);
        int nodes = graph->getCurrentNodeCount();
        out << nodes << "," << graph->getCurrentEdgeCount() << "\n";
        for (int source = 0; source < nodes; source++) {
            Node* sourceHead = graph->head[source];
            while (sourceHead != nullptr)
            {
                int destination = sourceHead->getValue();
                int weight = sourceHead->getCost();
                out << source << "," << destination << "," << weight << "\n";
                sourceHead = sourceHead->getNextNode();
            }
        }

        file.close();
    }
}


void MainWindow::on_openButton_clicked()
{
    QString filename= QFileDialog::getOpenFileName(this, "Choose File");
    if(filename.isEmpty())
           return;

    QFile file(filename);
    if(file.open(QIODevice::ReadOnly | QIODevice::ReadWrite)) {
        QString fileContent;
        QTextStream in(&file);
        // Read number of nodes and edges
        QString line = in.readLine();
        int nodes = line.split(u',')[0].toInt();
        int edges = line.split(u',')[1].toInt();

        nodesCount = nodes;
        edgesCount = edges;

        if (graph) {
            delete graph;
        }
        graph = new Graph(nodes, edges);
        updateCurrentEdgeLabel(edges);
        while (!in.atEnd()) {
            line = in.readLine();
            int source = line.split(u',')[0].toInt();
            int destination = line.split(u',')[1].toInt();
            int weight = line.split(u',')[2].toInt();
            Edge* edge = new Edge(source, destination, weight);
            graph->addEdge(edge);
        }
        graphWidget->setGraph(graph);
        graphWidget->visualize();
        populateEdgeList();

        file.close();
    }


    toggleCoreInput(false);
    toggleEdgeInput(false);
    toggleDijkstraInput(true);
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

void MainWindow::populateEdgeList() {
    int nodes = graph->getCurrentNodeCount();
    for (int source = 0; source < nodes; source++) {
        Node* sourceHead = graph->head[source];
        while (sourceHead != nullptr)
        {
            int destination = sourceHead->getValue();
            int weight = sourceHead->getCost();
            ui->edgeList->addItem(QString("%1 -> %2; Weight = %3").arg(QString::number(source), QString::number(destination), QString::number(weight)));
            sourceHead = sourceHead->getNextNode();
        }
    }
    ui->edgeList->repaint();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete graph;
    delete graphWidget;
}


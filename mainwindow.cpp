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

#define TOGGLEINPUT {

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

#define TOGGLEINPUTEND }

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

        updateGraphVisualization();

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

        updateGraphVisualization();

        //If all the edges have been filled already,
        //the ability to input more should be disabled.
        if (graph->getCurrentEdgeCount() == edgesCount)
        {
            toggleEdgeInput(false);
            toggleDijkstraInput(true);
            toggleSaveButton(true);
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
    // Unhighlight paths if there is any highlighted
    graphWidget->unHighlightAll();

    // Get entered source and destination
    int source = ui->dijkstraSourceText->text().toInt();
    int destination = ui->dijkstraDestinationText->text().toInt();

    try
    {
        if(!graph->doesNodeExist(source))
        {
            // Resets UI
            ui->shortestPathLabel->setText("Shortest path: ");
            ui->dijkstraSourceText->clear();
            graphWidget->unHighlightAll();
            throw DijkstraInputException("Source does not exist in the graph.");
        }
        else if(!graph->doesNodeExist(destination))
        {
            ui->shortestPathLabel->setText("Shortest path: ");
            ui->dijkstraDestinationText->clear();
            graphWidget->unHighlightAll();
            throw DijkstraInputException("Destination does not exist in the graph.");

        } else if(source == destination)
        {
            ui->shortestPathLabel->setText("Shortest path: ");
            ui->dijkstraSourceText->clear();
            ui->dijkstraDestinationText->clear();
            graphWidget->unHighlightAll();
            throw DijkstraInputException("Source and destination must be different.");
        }
        else
        {
            // Calculates the path and update label'e text
            std::string path = graph->calculateShortestPath(source, destination, graphWidget);
            QString text = QString::fromStdString(path);
            ui->shortestPathLabel->setWordWrap(true);
            ui->shortestPathLabel->setText(text);
        }
    } catch(const DijkstraInputException& ex)
    {
        QMessageBox errorMessageBox;
        errorMessageBox.critical(this, "Error", ex.what());
    }
}

void MainWindow::on_saveButton_clicked()
{
    if (!graph)
        return;

    QString filename= QFileDialog::getSaveFileName(this, "Save As", "", "Text files (*.txt");

    if (filename.isEmpty())
        return;

    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::ReadWrite)) {
        QTextStream out(&file);
        int nodes = graph->getCurrentNodeCount();
        // First line is always *node_count*,*edge_count
        out << nodes << "," << graph->getCurrentEdgeCount() << "\n";
        // Iterate over graph and output each edge on a separate line
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
    QString filename= QFileDialog::getOpenFileName(this, "Choose File", "", "Text files (*.txt)");
    if(filename.isEmpty())
           return;

    QFile file(filename);
    if(file.open(QIODevice::ReadOnly | QIODevice::ReadWrite)) {
        try {
            QString fileContent;
            QTextStream in(&file);

            // Read number of nodes and edges
            QStringList firstLineList = in.readLine().split(u',', Qt::SkipEmptyParts);
            if (firstLineList.size() != 2) {
                throw DijkstraException("File couldn't be parsed! First line is malformed.");
            }
            bool okNodes, okEdges;
            int nodes = firstLineList[0].toInt(&okNodes);
            int edges = firstLineList[1].toInt(&okEdges);

            // Check if conversion to int was successful
            if (!okNodes || !okEdges) {
                throw DijkstraException("File couldn't be parsed! First line is malformed.");
            }

            nodesCount = nodes;
            edgesCount = edges;

            if (graph) {
                delete graph;
            }
            graph = new Graph(nodes, edges);
            updateCurrentEdgeLabel(edges);

            // Read line by line, each line denotes an edge
            while (!in.atEnd()) {
                QStringList lineList = in.readLine().split(u',');
                if (lineList.size() != 3) {
                    throw DijkstraException("File couldn't be parsed! One of the lines is malformed.");
                }
                bool okSource, okDestination, okWeight;
                int source = lineList[0].toInt(&okSource);
                int destination = lineList[1].toInt(&okDestination);
                int weight = lineList[2].toInt(&okWeight);

                // Check if conversion to int was successful
                if (!okSource || !okDestination || !okWeight) {
                    throw DijkstraException("File couldn't be parsed! One of the lines is malformed.");
                }
                Edge* edge = new Edge(source, destination, weight);
                graph->addEdge(edge);
            }
            ui->nodesCountText->clear();
            ui->edgesCountText->clear();
            ui->shortestPathLabel->setText("Shortest path: ");
            ui->dijkstraSourceText->clear();
            ui->dijkstraDestinationText->clear();
            graphWidget->unHighlightAll();
            updateGraphVisualization();
        } catch (const std::exception& ex) {
            QMessageBox errorMessageBox;
            errorMessageBox.critical(this, "Error", ex.what());
            file.close();
            return;
        }
        file.close();
    } else {
        QMessageBox errorMessageBox;
        errorMessageBox.critical(this, "Error", "File could not be opened in the correct mode! Are you opening a .txt file?");
        return;
    }

    toggleCoreInput(false);
    toggleEdgeInput(false);
    toggleDijkstraInput(true);
    toggleSaveButton(true);
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

void MainWindow::updateGraphVisualization() {
    graphWidget->setGraph(graph);
    graphWidget->visualize();
    populateEdgeList();
}

void MainWindow::populateEdgeList() {
    ui->edgeList->clear();
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


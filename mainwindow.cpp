#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
}


MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::on_spinBox_SpecificActivity_valueChanged(double) {
    updateWasteData();
    updateMatrices();
}


void MainWindow::on_comboBox_SpecificActivityUnits_currentIndexChanged(int) {
    updateWasteData();
    updateMatrices();
}


void MainWindow::on_spinBox_WasteMass_valueChanged(double) {
    updateWasteData();
    updateMatrices();
}


void MainWindow::on_comboBox_WasteMassUnits_currentIndexChanged(int) {
    updateWasteData();
    updateMatrices();
}


void MainWindow::on_horizontalSlider_PercentToMatrix_valueChanged(int) {
    updateMatrices();
}


void MainWindow::on_listWidget_ContainmentMatrices_itemSelectionChanged() {
    updateMatrixRelatedFields();
}


void MainWindow::on_tableView_AvailableContainers_clicked(const QModelIndex &index) {
    ui->statusBar->showMessage(index.data().toString(), 2000);
}

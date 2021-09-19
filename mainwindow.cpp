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


void MainWindow::on_horizontalSlider_PercentToMatrix_valueChanged(int) {
    updateMatrices();
}


void MainWindow::on_listWidget_ContainmentMatrices_itemSelectionChanged() {
    updateMatrixRelatedFields();
}


void MainWindow::on_tableView_AvailableContainers_clicked(const QModelIndex &index) {
    ui->statusBar->showMessage(index.data().toString(), 2000);
}


void MainWindow::on_pushButton_SaveResults_clicked() {
    save_file();
}


void MainWindow::on_doubleSpinBox_WasteMass_valueChanged(double arg1) {
    wdata.SetMass(arg1);
    updateWasteData();
    updateMatrices();
    updateMatrixRelatedFields();
}


void MainWindow::on_radioButton_ZrCarrier_clicked() {
    wdata.SetZrCarrier();
    updateWasteData();
    updateMatrices();
    updateMatrixRelatedFields();
}


void MainWindow::on_radioButton_ZrO2Carrier_clicked() {
    wdata.SetZrO2Carrier();
    updateWasteData();
    updateMatrices();
    updateMatrixRelatedFields();
}

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

void MainWindow::updateWasteData() {
    wdata.CalcSpecificActivity(
                ui->spinBox_SpecificActivity->value(),
                ui->comboBox_SpecificActivityUnits->currentIndex());
    wdata.CalcMass(
                ui->spinBox_WasteMass->value(),
                ui->comboBox_WasteMassUnits->currentIndex());
}



void MainWindow::on_spinBox_SpecificActivity_valueChanged(double) {
    updateWasteData();
}


void MainWindow::on_comboBox_SpecificActivityUnits_currentIndexChanged(int) {
    updateWasteData();
}


void MainWindow::on_spinBox_WasteMass_valueChanged(double) {
    updateWasteData();
}


void MainWindow::on_comboBox_WasteMassUnits_currentIndexChanged(int) {
    updateWasteData();
}


void MainWindow::on_horizontalSlider_PercentToMatrix_valueChanged(int){
}


void MainWindow::on_listWidget_ContainmentMatrices_itemSelectionChanged() {
}


void MainWindow::on_tableView_AvailableContainers_clicked(const QModelIndex &index) {
}



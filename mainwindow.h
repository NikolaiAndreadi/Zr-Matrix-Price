#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "waste_classes.h"
#include "waste_containers.h"
#include "waste_data.h"
#include "waste_matrices.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    waste_classes wclasses;
    waste_containers wcontatiners;
    waste_data wdata;
    waste_matrices wmatrices;
    double zr_p_lower, zr_p_upper;
    QString currdir;

    void init();
    void updateWasteData();
    void updateContainers(matrix_calculated_params &);
    void updateMatrices();
    void updateMatrixRelatedFields();
    double getZrPercentage();
    void save_file();

private slots:
    void on_horizontalSlider_PercentToMatrix_valueChanged(int position);

    void on_listWidget_ContainmentMatrices_itemSelectionChanged();
    void on_tableView_AvailableContainers_clicked(const QModelIndex &index);

    void on_pushButton_SaveResults_clicked();

    void on_doubleSpinBox_WasteMass_valueChanged(double arg1);

    void on_radioButton_ZrCarrier_clicked();

    void on_radioButton_ZrO2Carrier_clicked();

    void on_tableWidget_Isotopes_cellChanged(int row, int column);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

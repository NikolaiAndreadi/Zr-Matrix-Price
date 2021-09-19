#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QColor>

#include <QStandardItemModel>
#include <QStandardItem>


void MainWindow::init() {
    wdata.SetMass(1);
    wdata.SetZrO2Carrier();
    ui->radioButton_ZrO2Carrier->setChecked(true);

    ui->horizontalSlider_PercentToMatrix->
            setRange(int(zr_p_lower*1000), int(zr_p_upper*1000));
    ui->horizontalSlider_PercentToMatrix->setValue(int(zr_p_lower*1000));

    wmatrices.Filter(zr_p_lower);
    ui->listWidget_ContainmentMatrices->addItems(wmatrices.Get_names());
    ui->listWidget_ContainmentMatrices->setCurrentRow(0);

    ui->tableView_AvailableContainers->resizeColumnsToContents();

    updateWasteData();
    updateMatrices();
    updateMatrixRelatedFields();
}


// divide by 1000 as horizontal slider uses integers (100.0% is 1000)
double MainWindow::getZrPercentage() {
    return double(ui->horizontalSlider_PercentToMatrix->value())/1000.0;
}


void MainWindow::updateWasteData() {
    ui->label_ActivityValue->setText(wdata.GetStrActivity());
}


void MainWindow::updateMatrices() {
    // DRAW ZR PERCENTAGE VALUE
    double p = getZrPercentage();
    auto text = QString("%1%").arg(p*100, 4, 'f', 1, ' ');
    ui->label_ZrPercentValue->setText(text);

    // FILTER AVAILABLE MATRICES
    QString sel_mtx_name;
    auto selected_matrix = ui->listWidget_ContainmentMatrices->
            currentItem();
    if (selected_matrix)
         sel_mtx_name = selected_matrix->text();

    wmatrices.Filter(p);
    ui->listWidget_ContainmentMatrices->clear();
    ui->listWidget_ContainmentMatrices->addItems(wmatrices.Get_names());

    auto search = ui->listWidget_ContainmentMatrices->
           findItems(sel_mtx_name, Qt::MatchExactly);
    if (search.length() == 1) {
        int row = ui->listWidget_ContainmentMatrices->row(search[0]);
        ui->listWidget_ContainmentMatrices->setCurrentRow(row);
    }
    else {
       selected_matrix = ui->listWidget_ContainmentMatrices->
               currentItem();

       if (!selected_matrix)
           ui->listWidget_ContainmentMatrices->setCurrentRow(0);
    }

    updateMatrixRelatedFields();
}


void MainWindow::updateMatrixRelatedFields() {
    QString text;

    // CALC MATRIX DATA
    QString sel_mtx_name = ui->listWidget_ContainmentMatrices->
            currentItem()->text();
    auto matrix_calc = wmatrices.Calculate_params(sel_mtx_name,
                                             getZrPercentage(),
                                             wdata.GetMass(),
                                             wdata.GetActivity());

    // MATRIX MASS WITH ZR
    text = QString("%1 кг").arg(matrix_calc.mass_with_zr, 5, 'e', 1, ' ');
    ui->label_MatrixMassValue->setText(text);

    // MATRIX MASS WITHOUT ZR
    text = QString("%1 кг").arg(matrix_calc.mass_pure, 5, 'e', 1, ' ');
    ui->label_MatrixPureMassValue->setText(text);

    // MATRIX VOLUME
    text = QString("%1 м³").arg(matrix_calc.volume, 5, 'e', 3, ' ');
    ui->label_MatrixVolumeValue->setText(text);

    // MATRIX PRICE
    text = QString("%1 руб").arg(matrix_calc.total_price, 6, 'f', 2, ' ');
    ui->label_MatrixPriceValue->setText(text);

    // MATRIX SPECIFIC ACTIVITY
    text = QString("%1 МБк/кг").arg(matrix_calc.specific_activity, 6, 'f', 2, ' ');
    ui->label_MatrixWasteSpecificActivityValue->setText(text);

    // MATRIX WASTE CLASS
    int waste_class = wclasses.CalcWasteClass(matrix_calc.specific_activity);
    text = QString("%1").arg(waste_class, 1);
    ui->label_MatrixWasteClassValue->setText(text);
    // introduce color
    QColor Source(255,0,0);
    QColor Target(0,180,0);
    int num_steps = wclasses.GetWasteClassCount();
    QColor tmpcolor;
    tmpcolor.setRed( Source.red()   + (((Target.red()   - Source.red())   * waste_class) / num_steps));
    tmpcolor.setGreen( Source.green() + (((Target.green() - Source.green()) * waste_class) / num_steps));
    tmpcolor.setBlue( Source.blue()  + (((Target.blue()  - Source.blue())  * waste_class) / num_steps));
    ui->label_MatrixWasteClassValue->setStyleSheet(QString("color: " + tmpcolor.name() + ";"));

    updateContainers(matrix_calc);
}


void MainWindow::updateContainers(matrix_calculated_params &matrix_calc) {
    QString sel_cnt_name;
    int sel_cnt_idx;
    auto sel_cnt = ui->tableView_AvailableContainers->currentIndex();
    if (sel_cnt.isValid()) {
        sel_cnt_name = wcontatiners[sel_cnt.row()].name;
    }


    auto *model = new QStandardItemModel;
    int n_containers;
    double container_price, disposal_price, sum_price;
    QStandardItem *item;
    QStringList headers;

    headers.append("Название");
    headers.append("Кол-во конт-ов");
    headers.append("Сумм. цена конт-ов");
    headers.append("Цена захоронения");
    headers.append("Сумма");
    model->setHorizontalHeaderLabels(headers);

    double mtx_class = wclasses.CalcWasteClass(matrix_calc.specific_activity);
    wcontatiners.Filter(mtx_class);

    for (int i=0; i<wcontatiners.len(); i++) {
        item = new QStandardItem(wcontatiners[i].name);
        item->setTextAlignment(Qt::AlignCenter);
        model->setItem(i,0, item);

        n_containers = ceil(matrix_calc.volume/wcontatiners[i].volume);
        item = new QStandardItem(QString("%1 шт.").arg(n_containers,2));
        item->setTextAlignment(Qt::AlignCenter);
        model->setItem(i,1, item);

        container_price = n_containers * wcontatiners[i].price;
        item = new QStandardItem(QString("%1 руб.").arg(container_price, 6, 'f', 2, ' '));
        item->setTextAlignment(Qt::AlignCenter);
        model->setItem(i,2, item);

        disposal_price = wclasses.CalcDisposalCost(mtx_class) * n_containers;
        item = new QStandardItem(QString("%1 руб.").arg(disposal_price, 6, 'f', 2, ' '));
        item->setTextAlignment(Qt::AlignCenter);
        model->setItem(i,3, item);

        sum_price = container_price + disposal_price + matrix_calc.total_price;
        item = new QStandardItem(QString("%1 руб.").arg(sum_price, 6, 'f', 2, ' '));
        item->setTextAlignment(Qt::AlignCenter);
        model->setItem(i,4, item);

        if (sel_cnt_name == wcontatiners[i].name)
            sel_cnt_idx = i;

    }

    ui->tableView_AvailableContainers->setModel(model);
    ui->tableView_AvailableContainers->setVisible(false);
    ui->tableView_AvailableContainers->sortByColumn(5, Qt::SortOrder::AscendingOrder);
    ui->tableView_AvailableContainers->setVisible(true);
    if (sel_cnt.isValid())
        ui->tableView_AvailableContainers->selectRow(sel_cnt_idx);
    else
        ui->tableView_AvailableContainers->selectRow(0);

}

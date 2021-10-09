#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QFile>
#include <QString>
#include <QMessageBox>


void MainWindow::save_file() {

    QString sel_mtx_name;
    auto selected_matrix = ui->listWidget_ContainmentMatrices->
            currentItem();
    if (selected_matrix)
         sel_mtx_name = selected_matrix->text();
    else {
        QMessageBox::information(this, tr("Ошибка сохранения"),
                                 tr("Для сохранения сначала нужно выбрать тип матрицы"));
        return;
    }

    auto sel_cnt_idx = ui->tableView_AvailableContainers->currentIndex();
    if (!sel_cnt_idx.isValid()) {
        QMessageBox::information(this, tr("Ошибка сохранения"),
                                 tr("Для сохранения сначала нужно выбрать тип контейнера"));
        return;
    }

    auto sel_mtx = wmatrices.Select_by_name(sel_mtx_name);
    auto data_sel_mtx = wmatrices.Calculate_params(sel_mtx_name,
                                                   getZrPercentage(),
                                                   wdata.GetMass(),
                                                   wdata.GetActivity());
    auto sel_cnt = wcontatiners[sel_cnt_idx.row()];

    QString filename = QFileDialog::getSaveFileName(this, tr("Сохранить результаты..."),
                                            currdir, tr("Text files (*.txt)"));
    if (filename == "")
        return;

    QFile f(filename);
    f.open(QFile::WriteOnly);
    QTextStream fout(&f);


    using namespace Qt;

    fout.setRealNumberPrecision(10);

    fout << "Отчёт стоимости переработки РАО Zr" << endl;
    fout << "Можно импортировать в excel с двоеточием как разделитель" << endl;
    fout << "По вопросам обращаться: nikolai.andreadi@chemistry.msu.ru" << endl;
    fout << "Исходный код программы: https://github.com/NikolaiAndreadi/ZrPrice" << endl;

    fout << endl << "Состав РАО" << endl;
    fout << "Название : Удельная активность, МБк/кг" << endl;
    foreach(const auto &isoname, wdata.GetIsotopeNames())
        fout << isoname << ": " << wdata.GetSpecificActivityByName(isoname) << endl;

    fout << endl << "Носитель РАО: " << (wdata.GetisZrCarrier() ? "Металлический цирконий" : "Диоксид циркония") << endl;
    fout << "Удельная активность, МБк/кг: " << wdata.GetSumSpecificActivity() << endl;
    fout << "Масса, кг: " << wdata.GetMass() << endl;
    fout << "Активность, МБк: " << wdata.GetActivity() << endl;

    fout << endl << "Данные выбранной матрицы" << endl;
    fout << "Название: " << sel_mtx_name << endl;
    fout << "Макс доля Zr в матрице (по массе): " << sel_mtx.max_zr_percentage << endl;
    fout << "Плотность, кг/м^3: " << sel_mtx.density_final << endl;
    fout << "Цена за кг, руб: " << sel_mtx.price_per_kg << endl;

    fout << endl << "Данные выбранной матрицы c РАО" << endl;
    fout << "Доля Zr в матрице (по массе): " << getZrPercentage() << endl;
    fout << "Масса чистой матрицы, кг: " << data_sel_mtx.mass_pure << endl;
    fout << "Стоимость матрицы руб: " << data_sel_mtx.total_price << endl;
    fout << "Масса матрицы c РАО, кг: " << data_sel_mtx.mass_with_zr << endl;
    fout << "Объем матрицы c РАО, м^3: " << data_sel_mtx.volume << endl;
    fout << "Уд. активность отходов, МБк/кг: " << data_sel_mtx.specific_activity << endl;

    double mtx_class = wclasses.CalcWasteClass(wdata, sel_mtx.max_zr_percentage);
    double one_cnt_disp_cost = wclasses.CalcDisposalCost(mtx_class);
    fout << endl << "Класс отходов: " << ui->label_MatrixWasteClassValue->text() << endl;
    fout << "Стоимость захоронения 1 контейнера: " <<
             one_cnt_disp_cost << endl;

    fout << endl << "Данные выбранного контейнера" << endl;
    fout << "Название: " << sel_cnt.name << endl;
    fout << "Объем, м^3: " << sel_cnt.volume << endl;
    fout << "Стоимость за шт., руб.: " << sel_cnt.price << endl;

    int n_containers = ceil(data_sel_mtx.volume/sel_cnt.volume);
    fout << endl << "Данные выбранного контейнера с отходами" << endl;
    fout << "Количество, шт: " << n_containers << endl;
    fout << "Стоимость контейнеров, руб.: " << sel_cnt.price*n_containers << endl;
    fout << "Стоимость захоронения контейнеров, руб.: " << one_cnt_disp_cost*n_containers << endl;

    fout << endl << "Фин. отчет" << endl;
    fout << "Наименование : Цена за еденицу : Кол-во : Итого" << endl;
    fout << "Матрица \"" << sel_mtx_name << "\" : " <<
            sel_mtx.price_per_kg << " : " <<
            data_sel_mtx.mass_pure << " : " <<
            data_sel_mtx.total_price << endl;
    fout << "Контейнер \"" << sel_cnt.name << "\" : " <<
            sel_cnt.price << " : " <<
            n_containers << " : " <<
            sel_cnt.price*n_containers << endl;
    fout << "Захоронение контейнеров " << mtx_class << " класса : " <<
            one_cnt_disp_cost << " : " <<
            n_containers << " : " <<
            one_cnt_disp_cost*n_containers << endl;
    fout << "----------------------------" << endl;
    fout << "Сумма: " << data_sel_mtx.total_price +
                         sel_cnt.price*n_containers +
                         one_cnt_disp_cost*n_containers;

    f.close();
}

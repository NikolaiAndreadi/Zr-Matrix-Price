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

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QSettings>
#include <QStringList>

#include "waste_classes.h"
#include "waste_containers.h"
#include "waste_matrices.h"


// TODO: send to console error place
// TODO: zr limits settings
waste_classes LoadClasses(QString path, bool &ok) {
    waste_classes result;
    QSettings file(path, QSettings::IniFormat);

    QStringList groups = file.childGroups();
    if (groups.length() == 0) { ok = false; return result; }

    foreach(const QString &tmpname, groups) {
        file.beginGroup(tmpname);

        double spa = file.value("SpecActivity").toDouble();
        if (spa <= 0) { ok = false; return result; }

        double dc = file.value("DisposalCost").toDouble();
        if (dc <= 0) { ok = false; return result; }

        file.endGroup();

        result.Append(waste_class_data(spa, dc));
    }

    ok = true;
    return result;
}

waste_containers LoadContainers(QString path, bool &ok) {
    waste_containers result;
    QSettings file(path, QSettings::IniFormat);

    QStringList groups = file.childGroups();
    if (groups.length() == 0) { ok = false; return result; }

    foreach(const QString &tmpname, groups) {
        file.beginGroup(tmpname);

        QString name = file.value("Name").toString();

        double volume = file.value("Volume").toDouble();
        if (volume <= 0) { ok = false; return result; }

        int price = file.value("Price").toInt();
        if (price <= 0) { ok = false; return result; }

        int waste_class = file.value("WasteClass").toInt();
        if (waste_class <= 0) { ok = false; return result; }

        file.endGroup();

        result.Append(waste_container_data(name, volume, price, waste_class));
    }

    ok = true;
    return result;
}


waste_matrices LoadMatrices(QString path, bool &ok) {
    waste_matrices result;
    QSettings file(path, QSettings::IniFormat);

    QStringList groups = file.childGroups();
    if (groups.length() == 0) { ok = false; return result; }

    foreach(const QString &tmpname, groups) {
        file.beginGroup(tmpname);

        QString name = file.value("Name").toString();

        double density = file.value("Density").toDouble();
        if (density <= 0) { ok = false; return result; }

        int price = file.value("Price").toInt();
        if (price <= 0) { ok = false; return result; }

        double max_zr_percent = file.value("MaxZrPercent").toDouble();
        if ((max_zr_percent <= 0) || (max_zr_percent > 1)) { ok = false; return result; }

        file.endGroup();

        result.Append(waste_matrix(name, density, price, max_zr_percent));
    }

    ok = true;
    return result;
}




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "ZrPrice_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;

    QString currdir = a.applicationDirPath();
    bool ok;

    w.wclasses = LoadClasses(currdir+"/settings/waste_classes.ini", ok);
    if (!ok) {
        qCritical("Failed to read ./settings/waste_classes.ini");
        exit(EXIT_FAILURE);
    }

    w.wcontatiners = LoadContainers(currdir+"/settings/containers.ini", ok);
    if (!ok) {
        qCritical("Failed to read ./settings/containers.ini");
        exit(EXIT_FAILURE);
    }

    w.wmatrices = LoadMatrices(currdir+"/settings/matrices.ini", ok);
    if (!ok) {
        qCritical("Failed to read ./settings/matrices.ini");
        exit(EXIT_FAILURE);
    }

    w.show();
    return a.exec();
}

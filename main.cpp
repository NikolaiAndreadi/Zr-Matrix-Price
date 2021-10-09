#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QSettings>
#include <QStringList>
#include <QMessageBox>
#include <QMap>

#include "waste_data.h"
#include "waste_classes.h"
#include "waste_containers.h"
#include "waste_matrices.h"


void ErrorLog(QString funcname, QString groupname, QString fieldname="") {
    if (fieldname == "")
        qCritical() << funcname + ": damaged '" + groupname+ "'";
    else
        qCritical() << funcname + ": field '" + fieldname +
                       "' is corrupted for '" + groupname + "'";
}


waste_classes LoadClasses(QString path, waste_data *wd, bool &ok) {
    waste_classes result;
    QSettings file(path, QSettings::IniFormat);

    QStringList groups = file.childGroups();
    if (groups.length() == 0) {
        ok = false;
        ErrorLog("LoadClasses", "not found");
        return result;
    }

    QStringList families = wd->GetIsotopeFamilies();

    foreach(const QString &tmpname, groups) {
        file.beginGroup(tmpname);

        double dc = file.value("DisposalCost").toDouble();
        if (dc <= 0) {
            ok = false;
            ErrorLog("LoadClasses", tmpname, "DisposalCost");
            return result;
        }

        if (tmpname == "Lowest") {
            result.SetLowestCost(dc);
            continue;
        }

        QMap<QString, double> boundaries; // familyname->sp.activity lower bound
        foreach(const QString &tmpfamily, families) {
            double spa = file.value("SpecActivity_"+tmpfamily).toDouble();
            if (spa > 0)
                boundaries[tmpfamily]=spa;
        }

        file.endGroup();

        if (boundaries.isEmpty()) {
            ok = false;
            ErrorLog("LoadClasses", tmpname, "Specific acivities fields");
            return result;
        }

        auto c = tmpname.back();
        if (c.isDigit())
            result.Append(waste_class_data(c.digitValue(), boundaries, dc));
        else {
            ErrorLog("LoadClasses", tmpname, "Does not contain digit at the end");
            return result;
        }
    }

    ok = true;
    return result;
}


waste_containers LoadContainers(QString path, bool &ok) {
    waste_containers result;
    QSettings file(path, QSettings::IniFormat);

    QStringList groups = file.childGroups();
    if (groups.length() == 0) {
        ok = false;
        ErrorLog("LoadContainers", "not found");
        return result;
    }

    foreach(const QString &tmpname, groups) {
        file.beginGroup(tmpname);

        QString name = file.value("Name").toString();
        if (name == "") {
            ok = false;
            ErrorLog("LoadContainers", tmpname, "Name");
            return result;
        }

        double volume = file.value("Volume").toDouble();
        if (volume <= 0) {
            ok = false;
            ErrorLog("LoadContainers", tmpname, "Volume");
            return result;
        }

        int price = file.value("Price").toInt();
        if (price <= 0) {
            ok = false;
            ErrorLog("LoadContainers", tmpname, "Price");
            return result;
        }

        int waste_class = file.value("WasteClass").toInt();
        if (waste_class <= 0) {
            ok = false;
            ErrorLog("LoadContainers", tmpname, "WasteClass");
            return result;
        }

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
    if (groups.length() == 0) {
        ok = false;
        ErrorLog("LoadMatrices", "not found");
        return result;
    }

    foreach(const QString &tmpname, groups) {
        file.beginGroup(tmpname);

        QString name = file.value("Name").toString();
        if (name == "") {
            ok = false;
            ErrorLog("LoadMatrices", tmpname, "Name");
            return result;
        }

        double density = file.value("Density").toDouble();
        if (density <= 0) {
            ok = false;
            ErrorLog("LoadMatrices", tmpname, "Density");
            return result;
        }

        int price = file.value("Price").toInt();
        if (price <= 0) {
            ok = false;
            ErrorLog("LoadMatrices", tmpname, "Price");
            return result;
        }

        double max_zr_percent = file.value("MaxZrPercent").toDouble();
        if ((max_zr_percent <= 0) || (max_zr_percent > 1)) {
            ok = false;
            ErrorLog("LoadMatrices", tmpname, "MaxZrPercent");
            return result;
        }

        file.endGroup();

        result.Append(waste_matrix(name, density, price, max_zr_percent));
    }

    ok = true;
    return result;
}


void SliderLimits(QString path, double &lower, double &upper, bool &ok) {
    QSettings file(path, QSettings::IniFormat);
    QStringList groups = file.childGroups();
    if (groups.length() == 0) {
        ok = false;
        ErrorLog("SliderLimits", "not found");
        return;
    }
    file.beginGroup(groups[0]);

    lower = file.value("WasteLowerPercentageLimit").toDouble();
    if ((lower <= 0) || (lower > 1)) {
        ok = false;
        ErrorLog("SliderLimits", "WasteLowerPercentageLimit");
        return;
    }

    upper = file.value("WasteUpperPercentageLimit").toDouble();
    if ((upper <= 0) || (upper > 1)) {
        ok = false;
        ErrorLog("SliderLimits", "WasteUpperPercentageLimit");
        return;
    }

    file.endGroup();
    ok = true;
}

waste_data LoadIsotopes(QString path, bool &ok) {
    waste_data result;
    QSettings file(path, QSettings::IniFormat);

    QStringList groups = file.childGroups();
    if (groups.length() == 0) {
        ok = false;
        ErrorLog("LoadIsotopes", "not found");
        return result;
    }

    foreach(const QString &tmpname, groups) {
        file.beginGroup(tmpname);

        QString name = file.value("Name").toString();
        if (name == "") {
            ok = false;
            ErrorLog("LoadIsotopes", tmpname, "Name");
            return result;
        }

        QString family = file.value("Family").toString();
        if (family  == "") {
            ok = false;
            ErrorLog("LoadIsotopes", tmpname, "Family");
            return result;
        }

        file.endGroup();

        result.AddIsotope(name, family);
    }

    result.SetMass(1);
    result.SetZrO2Carrier();
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
    w.currdir = currdir;
    bool ok;

    w.wdata = LoadIsotopes(currdir+"/settings/isotopes.ini", ok);
    if (!ok) {
        qCritical("Failed to read ./settings/isotopes.ini");
        QMessageBox::information(&w, "Ошибка открытия файла",
                                 "./settings/isotopes.ini\nПодробности в консоли");
        exit(EXIT_FAILURE);
    }

    w.wclasses = LoadClasses(currdir+"/settings/waste_classes.ini", &w.wdata, ok);
    if (!ok) {
        qCritical("Failed to read ./settings/waste_classes.ini");
        QMessageBox::information(&w, "Ошибка открытия файла",
                                 "./settings/waste_classes.ini\nПодробности в консоли");
        exit(EXIT_FAILURE);
    }

    w.wcontatiners = LoadContainers(currdir+"/settings/containers.ini", ok);
    if (!ok) {
        qCritical("Failed to read ./settings/containers.ini");
        QMessageBox::information(&w, "Ошибка открытия файла",
                                 "./settings/containers.ini\nПодробности в консоли");
        exit(EXIT_FAILURE);
    }

    w.wmatrices = LoadMatrices(currdir+"/settings/matrices.ini", ok);
    if (!ok) {
        qCritical("Failed to read ./settings/matrices.ini");
        QMessageBox::information(&w, "Ошибка открытия файла",
                                 "./settings/matrices.ini\nПодробности в консоли");
        exit(EXIT_FAILURE);
    }

    SliderLimits(currdir+"/settings/slider_limits.ini", w.zr_p_lower, w.zr_p_upper, ok);
    if (!ok) {
        qCritical("Failed to read ./settings/slider_limits.ini");
        QMessageBox::information(&w, "Ошибка открытия файла",
                                 "./settings/slider_limits.ini\nПодробности в консоли");
        exit(EXIT_FAILURE);
    }
    w.init();
    w.show();
    return a.exec();
}

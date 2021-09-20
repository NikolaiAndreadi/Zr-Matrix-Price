#ifndef WASTE_DATA_H
#define WASTE_DATA_H

#include <QString>
#include <QVector>
#include <QSet>


struct isotope
{
    QString name;
    QString family; // alpha/beta/thritium etc...
    double spec_activity; // MBq/kg

    isotope(QString name, QString family) :
        name(name),
        family(family)
    { spec_activity = 0.0; };
};


class waste_data
{
    QVector<isotope> isotopes;
    double mass; // kg
    bool isZrCarrier;

public:

    void SetZrCarrier() { isZrCarrier = true; };
    void SetZrO2Carrier() {isZrCarrier = false; };


    void SetMass(double m) { mass = m; };
    double GetMass() {
        if (isZrCarrier)
            return mass * 1.350719109;
        return mass;
    };


    double GetActivity() {
        if (isZrCarrier)
            return GetSumSpecificActivity() * mass * 1.350719109;
        return GetSumSpecificActivity() * mass;
    };
    QString GetStrActivity() {return QString("%1").arg(GetActivity(), 0, 'f', 3); };


    void AddIsotope(QString name, QString family) {
        isotopes.push_back(isotope(name, family));
    };


    QStringList GetIsotopeNames() {
        QStringList out;
        foreach (auto &item, isotopes)
            out.push_back(item.name);
        return out;
    };
    QStringList GetIsotopeFamilies() {
        QSet<QString> out;
        foreach (auto &item, isotopes)
            out.insert(item.family);
        return QStringList(out.begin(), out.end());
    };


    double GetSpecificActivityByFamily(QString family) {
        double sum = 0.0;
        foreach (auto &item, isotopes)
            if (item.family == family)
                sum += item.spec_activity;
        return sum;
    };
    double GetSpecificActivityByName(QString name) {
        foreach (auto &item, isotopes)
            if (item.name == name)
                return item.spec_activity;
        return 0;
    };
    double GetSumSpecificActivity() {
        double sum = 0.0;
        foreach (auto &item, isotopes)
            sum += item.spec_activity;
        return sum;
    };

};

#endif // WASTE_DATA_H

#ifndef WASTE_CLASSES_H
#define WASTE_CLASSES_H

#include <QVector>
#include <algorithm>
#include <QMap>
#include "waste_data.h"
#include <QDebug>


struct waste_class_data
{
    int class_number;
    QMap<QString, double> boundaries;
    double waste_container_disposal_cost;
    waste_class_data(int class_number, QMap<QString, double> boundaries, double waste_container_disposal_cost) :
        class_number(class_number),
        boundaries(boundaries),
        waste_container_disposal_cost(waste_container_disposal_cost)
    {}
    waste_class_data() = delete; // we can only fill with full information

    bool operator< (const waste_class_data &comp) const {
        return class_number < comp.class_number;
    }
};


class waste_classes
{
    // should be ordered from the highest to the lowest class (e.g. lowest spec. activity to highest)
    QVector<waste_class_data> classes;
    double lowest_cost;
public:

    void Append(waste_class_data new_waste_class) {
        classes.push_back(new_waste_class);
        std::sort(classes.begin(), classes.end());
    };
    void SetLowestCost(double cost) { lowest_cost = cost; }; // TODO: implement lowest cost

    unsigned int GetWasteClassCount() { return classes.length()+1; };

    unsigned int CalcWasteClass(waste_data wd, double percentage, bool isZrCarrier) {
        unsigned int waste_class = 1;
        foreach(auto &wclass, classes) {
            foreach(auto &familyname, wclass.boundaries.keys()) {
                double wasteFamilySpecificActivity = wd.GetSpecificActivityByFamily(familyname)*percentage;
                if (isZrCarrier)
                    wasteFamilySpecificActivity/=1.350719109;
                if (wasteFamilySpecificActivity > wclass.boundaries[familyname])
                    return waste_class;
            }
            waste_class++;
        }
        return waste_class;
    };

    double CalcDisposalCost(unsigned int waste_class) {
        if (waste_class > classes.length())
            return lowest_cost;
        return classes[classes.length()-waste_class].waste_container_disposal_cost;
    };
};

#endif // WASTE_CLASSES_H

#ifndef WASTE_CLASSES_H
#define WASTE_CLASSES_H

#include <QVector>
#include <algorithm>


struct waste_class_data
{
    double specific_activity_upper_limit;
    double waste_container_disposal_cost;
    waste_class_data(double specific_activity_upper_limit, double waste_container_disposal_cost) :
        specific_activity_upper_limit(specific_activity_upper_limit),
        waste_container_disposal_cost(waste_container_disposal_cost)
    {}
    waste_class_data() = delete; // we can only fill with full information

    bool operator< (const waste_class_data &comp) const {
        return specific_activity_upper_limit < comp.specific_activity_upper_limit;
    }
};


class waste_classes
{
    // should be ordered from the highest to the lowest class (e.g. lowest spec. activity to highest)
    QVector<waste_class_data> classes;
public:

    void Append(waste_class_data new_waste_class) {
        classes.push_back(new_waste_class);
        std::sort(classes.begin(), classes.end());
    };

    unsigned int GetWasteClassCount() { return classes.length()+1; };

    unsigned int CalcWasteClass(double specific_activity) {
        unsigned int waste_class = classes.length()+1;
        foreach(auto &item, classes) {
            if (specific_activity < item.specific_activity_upper_limit)
                return waste_class;
            waste_class--;
        }
        return waste_class;
    };

    double CalcDisposalCost(unsigned int waste_class) {
        return classes[classes.length()-waste_class].waste_container_disposal_cost;
    };
};

#endif // WASTE_CLASSES_H

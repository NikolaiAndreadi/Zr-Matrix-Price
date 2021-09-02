#ifndef WASTE_CONTAINERS_H
#define WASTE_CONTAINERS_H

#include <QString>
#include <QVector>

// waste_container_data contains info about single type of container
struct waste_container_data
{
    QString name;
    double volume; // in m^3
    double price;  // in Rub, per kg
    int waste_class;
    double max_zr_percentage; // in range 0..1

    waste_container_data(QString name, double volume, double price, int waste_class, double max_zr_percentage) :
        name(name),
        volume(volume),
        price(price),
        waste_class(waste_class),
        max_zr_percentage(max_zr_percentage)
    {};

    waste_container_data() = delete; // we can only fill with full information
};

class waste_containers
{
    QVector<waste_container_data> containers;
    QVector<waste_container_data> filtered_c;
public:
    int len() { return filtered_c.length(); };
    waste_container_data operator[](int i) { return filtered_c[i]; };

    void Append(waste_container_data new_waste_container) { containers.append(new_waste_container); };

    void Filter(int waste_class, double zr_percentage) {
        filtered_c.clear();
        foreach (auto &item, containers)
            if ((item.waste_class == waste_class) && (zr_percentage < item.max_zr_percentage))
                filtered_c.push_back(item);
    };
};

#endif // WASTE_CONTAINERS_H

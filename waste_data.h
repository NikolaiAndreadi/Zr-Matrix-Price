#ifndef WASTE_DATA_H
#define WASTE_DATA_H

#include <QString>


class waste_data
{
    double specific_activity; // MBq/kg
    double mass; // kg
    bool isZrCarrier;

public:

    void SetZrCarrier() { isZrCarrier = true; };
    void SetZrO2Carrier() {isZrCarrier = false; };

    void SetMass(double m) { mass = m; };

    double GetActivity() {
        if (isZrCarrier)
            return specific_activity * mass * 1.350719109;
        return specific_activity * mass;
    };

    QString GetStrActivity() {return QString("%1").arg(GetActivity(), 0, 'f', 3); };

    double GetMass() {
        if (isZrCarrier)
            return mass * 1.350719109;
        return mass;
    };

    double GetSpecificActivity() {return specific_activity; };

};

#endif // WASTE_DATA_H

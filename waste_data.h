#ifndef WASTE_DATA_H
#define WASTE_DATA_H

#include <QString>

class waste_data
{
    double specific_activity; // MBq/kg
    double mass; // kg

public:

    // bool false if fail
    bool CalcSpecificActivity(double a, int a_units) {
        switch(a_units) {
        case 0: // MBq/kg
        case 3: // GBq/tonne
            specific_activity = a;
            break;

        case 1: // MBq/tonne
        specific_activity = a*1e-3;
            break;

        case 2: // GBq/kg
            specific_activity = a*1e3;
            break;

        default:
            return false;
            break;
        }

        return true;
    };

    bool CalcMass(double m, int m_units) {
        switch (m_units) {
        case 0: // kg
            mass = m;
            break;

        case 1: // tonne
            mass = m * 1000;
            break;

        default:
            return false;
            break;
        }

        return true;
    };

    double GetActivity() {return specific_activity * mass; };
    QString GetStrActivity() {return QString("%1").arg(GetActivity(), 0, 'f', 3); };

    double GetMass() {return mass; };

};

#endif // WASTE_DATA_H

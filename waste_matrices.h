#ifndef WASTE_MATRICES_H
#define WASTE_MATRICES_H

#include <QString>
#include <QVector>

struct waste_matrix
{
    QString name;
    double density_final; // in kg/m^3
    double price_per_kg;         // per kg
    double max_zr_percentage; // in range 0..1
    bool ok; // if created  properely

    waste_matrix(QString name, double density_final, double price_per_kg, double max_zr_percentage, bool ok=true) :
        name(name),
        density_final(density_final),
        price_per_kg(price_per_kg),
        max_zr_percentage(max_zr_percentage),
        ok(ok)
    {};

    waste_matrix()  = delete; // take only fully inserted data fields
};

struct matrix_calculated_params
{
    double mass_pure;
    double mass_with_zr;
    double volume;
    double total_price;
    double specific_activity;
    bool ok; // if calculated properely

    matrix_calculated_params(double mass_pure, double mass_with_zr, double volume, double total_price,  double specific_activity, bool ok=true) :
        mass_pure(mass_pure),
        mass_with_zr(mass_with_zr),
        volume(volume),
        total_price(total_price),
        specific_activity(specific_activity),
        ok(ok)
    {};

    matrix_calculated_params() = delete; // take only fully inserted data fields
};

class waste_matrices
{
    QVector<waste_matrix> matrices;
public:
    void Append(waste_matrix new_waste_matrix) { matrices.push_back(new_waste_matrix); };

    waste_matrix Select_by_name(QString selected_name) {
        foreach (auto &item, matrices)
            if (item.name == selected_name)
                return waste_matrix(item.name, item.density_final, item.price_per_kg, item.max_zr_percentage);
        return  waste_matrix("", 0, 0, 0, false);
    };


    matrix_calculated_params Calculate_params(QString matrix_name, double zr_percent, double zr_mass, double activity) {
        waste_matrix selected = Select_by_name(matrix_name);
        if (!selected.ok) {
            return matrix_calculated_params(0,0,0,0,0,false);
        }

        double mass_pure = zr_mass * (1-zr_percent)/zr_percent;
        double mass_with_zr = mass_pure + zr_mass;
        double volume = mass_with_zr/selected.density_final;
        double total_price = selected.price_per_kg * mass_pure;
        double specific_activity = activity/mass_with_zr;
        return matrix_calculated_params(mass_pure, mass_with_zr, volume, total_price, specific_activity);
    };

    QStringList Get_filtered_names(double zr_percent){
        QStringList output;
        foreach(auto &item, matrices)
            if (zr_percent < item.max_zr_percentage)
                output.append(item.name);
        return output;
    };
};

#endif // WASTE_MATRICES_H

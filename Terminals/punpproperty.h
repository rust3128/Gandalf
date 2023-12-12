
#ifndef PUNPPROPERTY_H
#define PUNPPROPERTY_H

#include <QString>


class PunpProperty
{
public:
    PunpProperty(int disp_id, int pump_id, int tank_id, QString shortName);
    int disp_id() const;
    int pump_id() const;
    int tank_id() const;
    QString shortName() const;

private:
    int m_disp_id;
    int m_pump_id;
    int m_tank_id;
    QString m_shortName;
};

#endif // PUNPPROPERTY_H

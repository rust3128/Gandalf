
#ifndef TANKPROPERTY_H
#define TANKPROPERTY_H

#include <QString>

class TankProperty
{
public:
    TankProperty(int tankID, int fuelID, QString shortName, QString name, float maxValue, float minValue, float deadMax, float deadMin, float tubeAmount);
    int tankID() const;
    int fuelID() const;
    QString shortName() const;
    QString name() const;
    float maxValue() const;
    float minValue() const;
    float deadMax() const;
    float deadMin() const;
    float tubeAmount() const;

private:
    int m_tankID;
    int m_fuelID;
    QString m_shortName;
    QString m_name;
    float m_maxValue;
    float m_minValue;
    float m_deadMax;
    float m_deadMin;
    float m_tubeAmount;
};

#endif // TANKPROPERTY_H

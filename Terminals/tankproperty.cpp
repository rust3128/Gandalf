
#include "tankproperty.h"

TankProperty::TankProperty(int tankID, int fuelID, QString shortName, QString name, float maxValue, float minValue, float deadMax, float deadMin, float tubeAmount) :
    m_tankID(tankID), m_fuelID(fuelID), m_shortName(shortName), m_name(name),
    m_maxValue(maxValue), m_minValue(minValue), m_deadMax(deadMax), m_deadMin(deadMin), m_tubeAmount(tubeAmount)
{

}

int TankProperty::tankID() const
{
    return m_tankID;
}

int TankProperty::fuelID() const
{
    return m_fuelID;
}

QString TankProperty::shortName() const
{
    return m_shortName;
}

QString TankProperty::name() const
{
    return m_name;
}

float TankProperty::maxValue() const
{
    return m_maxValue;
}

float TankProperty::minValue() const
{
    return m_minValue;
}

float TankProperty::deadMax() const
{
    return m_deadMax;
}

float TankProperty::deadMin() const
{
    return m_deadMin;
}

float TankProperty::tubeAmount() const
{
    return m_tubeAmount;
}


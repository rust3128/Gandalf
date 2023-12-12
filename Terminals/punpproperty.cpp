
#include "punpproperty.h"

PunpProperty::PunpProperty(int disp_id, int pump_id, int tank_id, QString shortName) :
    m_disp_id(disp_id), m_pump_id(pump_id), m_tank_id(tank_id), m_shortName(shortName)
{

}

int PunpProperty::disp_id() const
{
    return m_disp_id;
}

int PunpProperty::pump_id() const
{
    return m_pump_id;
}

int PunpProperty::tank_id() const
{
    return m_tank_id;
}

QString PunpProperty::shortName() const
{
    return m_shortName;
}


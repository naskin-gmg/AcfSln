#ifndef imeas_CGeneralUnitInfo_included
#define imeas_CGeneralUnitInfo_included


// ACF-Solutions includes
#include "imeas/IUnitInfo.h"


namespace imeas
{


/**
	General implementation of the IUnitInfo interface.
*/
class CGeneralUnitInfo: virtual public IUnitInfo
{
public:
	CGeneralUnitInfo(
				int type = UT_UNKNOWN,
				const QString& name = "",
				double displayMultFactor = 1.0,
				const istd::CRange& range = istd::CRange::GetInvalid());

	void SetUnitType(int type);
	void SetUnitName(const QString& name);
	void SetDisplayMultiplicationFactor(double factor);
	void SetValueRange(const istd::CRange& range);

	//	reimplemented (imeas::IUnitInfo)
	virtual int GetUnitType() const;
	virtual QString GetUnitName() const;
	virtual double GetDisplayMultiplicationFactor() const;
	virtual istd::CRange GetValueRange() const;
	virtual const imath::IDoubleManip& GetValueManip() const;

private:
	int m_type;
	QString m_name;
	double m_displayMultFactor;
	istd::CRange m_range;
};


} // namespace imeas


#endif // !imeas_CGeneralUnitInfo_included



#ifndef imeas_CGeneralUnitInfo_included
#define imeas_CGeneralUnitInfo_included


// ACF-Solutions includes
#include "imeas/IUnitInfo.h"


namespace imeas
{


class CGeneralUnitInfo: virtual public IUnitInfo
{
public:
	CGeneralUnitInfo(
				int type = UT_UNKNOWN,
				const istd::CString& name = "",
				double displayMultFactor = 1.0,
				const istd::CRange& range = istd::CRange::GetInvalid());

	void SetUnitType(int type);
	void SetUnitName(const istd::CString& name);
	void SetDisplayMultiplicationFactor(double factor);
	void SetValueRange(const istd::CRange& range);

	//	reimplemented (imeas::IUnitInfo)
	virtual int GetUnitType() const;
	virtual istd::CString GetUnitName() const;
	virtual double GetDisplayMultiplicationFactor() const;
	virtual istd::CRange GetValueRange() const;
	virtual const imath::IDoubleManip& GetValueManip() const;

private:
	int m_type;
	istd::CString m_name;
	double m_displayMultFactor;
	istd::CRange m_range;
};


} // namespace imeas


#endif // !imeas_CGeneralUnitInfo_included



#ifndef imeas_CLinearAdjustParamsComp_included
#define imeas_CLinearAdjustParamsComp_included


#include "iser/ISerializable.h"

#include "icomp/CComponentBase.h"

#include "imeas/ILinearAdjustParams.h"
#include "imeas/ILinearAdjustConstraints.h"
#include "imeas/INumericValue.h"
#include "imeas/INumericConstraints.h"


namespace imeas
{


/**
	Basic implementation of component of linear adjust parameters.
*/
class CLinearAdjustParamsComp:
			public icomp::CComponentBase,
			virtual public ILinearAdjustParams,
			virtual public INumericValue,
			virtual protected INumericConstraints
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CLinearAdjustParamsComp);
		I_REGISTER_INTERFACE(ILinearAdjustParams);
		I_REGISTER_INTERFACE(INumericValue);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_ASSIGN(m_constraintsCompPtr, "Constraints", "Constraints describing allowed value ranges", false, "Constraints");
		I_ASSIGN(m_defaultScaleAttrPtr, "DefaultScale", "Default value of scale factor", true, 1);
		I_ASSIGN(m_defaultOffsetAttrPtr, "DefaultOffset", "Default value of offset factor", true, 0);
	I_END_COMPONENT;

	CLinearAdjustParamsComp();

	// reimplemented (imeas::ILinearAdjustParams)
	virtual const ILinearAdjustConstraints* GetAdjustConstraints() const;
	virtual double GetScaleFactor() const;
	virtual void SetScaleFactor(double scale);
	virtual double GetOffsetFactor() const;
	virtual void SetOffsetFactor(double offset);

	// reimplemented (imeas::INumericValue)
	virtual bool IsValueTypeSupported(ValueTypeId valueTypeId) const;
	virtual const INumericConstraints* GetNumericConstraints() const;
	virtual imath::CVarVector GetComponentValue(ValueTypeId valueTypeId) const;
	virtual imath::CVarVector GetValues() const;
	virtual bool SetValues(const imath::CVarVector& values);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	// reimplemented (imeas::INumericConstraints)
	virtual int GetNumericValuesCount() const;
	virtual QString GetNumericValueName(int index) const;
	virtual QString GetNumericValueDescription(int index) const;
	virtual const imeas::IUnitInfo& GetNumericValueUnitInfo(int index) const;

private:
	I_REF(ILinearAdjustConstraints, m_constraintsCompPtr);
	I_ATTR(double, m_defaultScaleAttrPtr);
	I_ATTR(double, m_defaultOffsetAttrPtr);

	double m_scale;
	double m_offset;
};


} // namespace imeas


#endif // !imeas_CLinearAdjustParamsComp_included



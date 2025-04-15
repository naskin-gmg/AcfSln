#ifndef imeas_CSimpleNumericValue_included
#define imeas_CSimpleNumericValue_included


// ACF includes
#include <iser/ISerializable.h>

// ACF-Solutions includes
#include <imeas/INumericValue.h>


namespace imeas
{


/**
	Basic implementation of imeas::INumericValue interface.
*/
class CSimpleNumericValue: virtual public INumericValue
{
public:
	// reimplemented (imeas::INumericValue)
	virtual bool IsValueTypeSupported(ValueTypeId valueTypeId) const override;
	virtual const INumericConstraints* GetNumericConstraints() const override;
	virtual imath::CVarVector GetComponentValue(ValueTypeId valueTypeId) const override;
	virtual imath::CVarVector GetValues() const override;
	virtual bool SetValues(const imath::CVarVector& values) override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;

protected:
	imath::CVarVector m_values;
};


} // namespace imeas


#endif // !imeas_CSimpleNumericValue_included



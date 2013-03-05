#ifndef imeas_CSimpleNumericValue_included
#define imeas_CSimpleNumericValue_included


// ACF includes
#include "iser/ISerializable.h"

// ACF-Solutions incldues
#include "imeas/INumericValue.h"


namespace imeas
{


/**
	Basic implementation of imeas::INumericValue interface.
*/
class CSimpleNumericValue: virtual public INumericValue
{
public:
	// reimplemented (imeas::INumericValue)
	virtual bool IsValueTypeSupported(ValueTypeId valueTypeId) const;
	virtual const INumericConstraints* GetNumericConstraints() const;
	virtual imath::CVarVector GetComponentValue(ValueTypeId valueTypeId) const;
	virtual imath::CVarVector GetValues() const;
	virtual bool SetValues(const imath::CVarVector& values);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);

protected:
	imath::CVarVector m_values;
};


} // namespace imeas


#endif // !imeas_CSimpleNumericValue_included



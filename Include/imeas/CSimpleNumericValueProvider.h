#ifndef imeas_CSimpleNumericValueProvider_included
#define imeas_CSimpleNumericValueProvider_included


// ACF includes
#include "iser/ISerializable.h"
#include "ibase/TLoggerCompWrap.h"
#include "imeas/CSimpleNumericValue.h"

// QSF includes
#include "imeas/INumericValueProvider.h"


namespace imeas
{


class CSimpleNumericValueProvider: virtual public INumericValueProvider
{
public:
	// reimplemented (imeas::INumericValueProvider)
	virtual int GetValuesCount() const;
	virtual const imeas::INumericValue& GetNumericValue(int index) const;
	
	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive & archive);

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);

private:
	typedef QVector<imeas::CSimpleNumericValue> Values;

	Values m_values;
};


} // namespace imeas


#endif // !imeas_CSimpleNumericValueProvider_included



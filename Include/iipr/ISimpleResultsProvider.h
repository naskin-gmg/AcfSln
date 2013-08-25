#ifndef iipr_CSimpleResultsContainer_included
#define iipr_CSimpleResultsContainer_included


// Qt includes
#include <QtCore/QVariant>


namespace iipr
{


class CSimpleResultsContainer: public QVariantList
{
public:
	virtual void Reset()
	{
		clear();
	}

	virtual void PushBack(const QVariant& value)
	{
		append(value);
	}
};


class ISimpleResultsConsumer: virtual public istd::IPolymorphic
{
public:
	virtual void SetResultsBuffer(CSimpleResultsContainer* bufferPtr) = 0;
};


class ISimpleResultsProvider: virtual public istd::IPolymorphic
{
public:
	virtual CSimpleResultsContainer* GetResults() const = 0;
};


} // namespace iipr


#endif // !iipr_CSimpleResultsContainer_included

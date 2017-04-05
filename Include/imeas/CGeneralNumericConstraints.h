#ifndef imeas_CGeneralNumericConstraints_included
#define imeas_CGeneralNumericConstraints_included


// ACF includes
#include <iser/ISerializable.h>
#include <icomp/CComponentBase.h>
#include <imath/CGeneralUnitInfo.h>

// ACF-Solutions includes
#include <imeas/INumericValue.h>
#include <imeas/INumericConstraints.h>


namespace imeas
{


/**
	Simple implementation of imeas::INumericConstraints interface.
*/
class CGeneralNumericConstraints: virtual public INumericConstraints
{
public:
	/**
		Removes all value informations.
	*/
	void Reset();
	/**
		Add new value information.
	*/
	void InsertValueInfo(const QString& name, const QString& description, const imath::CGeneralUnitInfo& unitInfo, int index = -1);
	/**
		Remove single value information.
	*/
	void RemoveValueInfo(int index);
	/**
		Get unit info stored alway in this structure.
	*/
	const imath::CGeneralUnitInfo& GetGeneralUnitInfo(int index) const;

	// reimplemented (imeas::INumericConstraints)
	virtual int GetNumericValuesCount() const;
	virtual QString GetNumericValueName(int index) const;
	virtual QString GetNumericValueDescription(int index) const;
	virtual const imath::IUnitInfo* GetNumericValueUnitInfo(int index) const;

private:
	struct Info
	{
		QString name;
		QString description;
		imath::CGeneralUnitInfo unitInfo;
	};

	QVector<Info> m_valueInfos;
};


} // namespace imeas


#endif // !imeas_CGeneralNumericConstraints_included



#ifndef imeas_CGeneralNumericConstraints_included
#define imeas_CGeneralNumericConstraints_included


// ACF includes
#include <imath/CGeneralUnitInfo.h>

// ACF-Solutions includes
#include <imeas/INumericValue.h>
#include <imeas/INumericConstraints.h>


namespace imeas
{


/**
	Simple implementation of imeas::INumericConstraints interface.
*/
class CGeneralNumericConstraints:
			virtual public INumericConstraints,
			virtual protected iprm::IOptionsList
{
public:
	/**
		Removes all value informations.
	*/
	void Reset();
	/**
		Add new value information.
	*/
	virtual bool InsertValueInfo(
				const QString& name,
				const QString& description,
				const QByteArray& valueId,
				const imath::CGeneralUnitInfo& unitInfo,
				int index = -1);

	/**
		Remove single value information.
	*/
	void RemoveValueInfo(int index);

	/**
		Get unit info stored alway in this structure.
	*/
	const imath::CGeneralUnitInfo& GetGeneralUnitInfo(int index) const;

	// reimplemented (imeas::INumericConstraints)
	virtual const iprm::IOptionsList& GetValueListInfo() const;
	virtual const imath::IUnitInfo* GetNumericValueUnitInfo(int index) const;

	// reimplemented (iprm::IOptionsList)
	virtual int GetOptionsFlags() const;
	virtual int GetOptionsCount() const;
	virtual QString GetOptionName(int index) const;
	virtual QString GetOptionDescription(int index) const;
	virtual QByteArray GetOptionId(int index) const;
	virtual bool IsOptionEnabled(int index) const;

private:
	struct Info
	{
		QString name;
		QString description;
		QByteArray id;
		imath::CGeneralUnitInfo unitInfo;
	};

	QVector<Info> m_valueInfos;
};


} // namespace imeas


#endif // !imeas_CGeneralNumericConstraints_included



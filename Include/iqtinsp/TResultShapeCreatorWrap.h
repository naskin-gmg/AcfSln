#pragma once


// ACF includes
#include <iview/IShapeFactory.h>


namespace iqtinsp
{


template <class Base>
class TResultShapeCreatorWrap: public Base
{
public:
	typedef Base BaseClass;

	I_BEGIN_COMPONENT(TResultShapeCreatorWrap);
		I_ASSIGN(m_okColorSchemeCompPtr, "OkColorSchema", "Color schema used for ok shapes", false, "OkColorSchema");
		I_ASSIGN(m_warningColorSchemeCompPtr, "WarningColorSchema", "Color schema used for warning shapes", false, "WarningColorSchema");
		I_ASSIGN(m_errorColorSchemeCompPtr, "ErrorColorSchema", "Color schema used for error shapes", false, "ErrorColorSchema");
		I_ASSIGN(m_resultShapeFactoryCompPtr, "ResultShapeFactory", "Creates shapes to display on preview (if iqt2d::IViewProvider is used)", false, "ResultShapeFactory");
	I_END_COMPONENT;

	/**
		Create a shape for the result object and corresponding status information.
	*/
	virtual iview::IShape* CreateResultShape(const istd::IChangeable* objectPtr, const istd::IInformationProvider* infoPtr, bool connectToModel = true) const;

private:
	I_REF(iview::IColorSchema, m_okColorSchemeCompPtr);
	I_REF(iview::IColorSchema, m_warningColorSchemeCompPtr);
	I_REF(iview::IColorSchema, m_errorColorSchemeCompPtr);
	I_REF(iview::IShapeFactory, m_resultShapeFactoryCompPtr);
};


// public methods
template <class Base>
iview::IShape* TResultShapeCreatorWrap<Base>::CreateResultShape(const istd::IChangeable* objectPtr, const istd::IInformationProvider* infoPtr, bool connectToModel) const
{
	if (m_resultShapeFactoryCompPtr.IsValid()){
		iview::IShape* shapePtr = m_resultShapeFactoryCompPtr->CreateShape(objectPtr, connectToModel);
		if (shapePtr == NULL){
			return NULL;
		}

		if (infoPtr == NULL){
			return shapePtr;
		}

		int category = infoPtr->GetInformationCategory();

		const iview::IColorSchema* userColorSchemaPtr = shapePtr->GetUserColorSchema();

		switch (category){
			case istd::IInformationProvider::IC_ERROR:
			case istd::IInformationProvider::IC_CRITICAL:
				if ((userColorSchemaPtr == NULL) && m_errorColorSchemeCompPtr.IsValid()){
					shapePtr->SetUserColorSchema(m_errorColorSchemeCompPtr.GetPtr());
				}
				break;

			case istd::IInformationProvider::IC_WARNING:
				if ((userColorSchemaPtr == NULL) && m_warningColorSchemeCompPtr.IsValid()){
					shapePtr->SetUserColorSchema(m_warningColorSchemeCompPtr.GetPtr());
				}
				break;

			case istd::IInformationProvider::IC_INFO:
			case istd::IInformationProvider::IC_NONE:
				if ((userColorSchemaPtr == NULL) && m_okColorSchemeCompPtr.IsValid()){
					shapePtr->SetUserColorSchema(m_okColorSchemeCompPtr.GetPtr());
				}
				break;

			default:;
		}

		return shapePtr;
	}

	return NULL;
}


} // namespace iqtinsp



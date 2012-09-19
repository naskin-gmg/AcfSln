#ifndef iqtipr_CMultiLineSupplierGuiComp_included
#define iqtipr_CMultiLineSupplierGuiComp_included


// ACF includes
#include "i2d/CPolyline.h"
#include "iqtinsp/TSupplierGuiCompBase.h"
#include "imeas/CSimpleNumericValueProvider.h"
#include "iprm/ISelectionParam.h"

// QSF includes
#include "Generated/ui_CMultiLineSupplierGuiComp.h"


namespace iqtipr
{


class CMultiLineSupplierGuiComp: 
			public iqtinsp::TSupplierGuiCompBase<
						Ui::CMultiLineSupplierGuiComp,
						imeas::INumericValueProvider>,
			virtual public iprm::ISelectionParam
{
	Q_OBJECT

public:
	typedef iqtinsp::TSupplierGuiCompBase<
				Ui::CMultiLineSupplierGuiComp,
				imeas::INumericValueProvider> BaseClass;

	I_BEGIN_COMPONENT(CMultiLineSupplierGuiComp);
		I_REGISTER_INTERFACE(iprm::ISelectionParam);
	I_END_COMPONENT;

	CMultiLineSupplierGuiComp();

protected Q_SLOTS:
	void on_TestButton_clicked();

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags = 0);

	// reimplemented (iqtinsp::TSupplierGuiCompBase)
	virtual void OnSupplierParamsChanged();
	virtual QWidget* GetParamsWidget() const;

	// reimplemented (iqt2d::TViewExtenderCompBase)
	virtual void CreateShapes(int sceneId, Shapes& result);

	// reimplemented (iprm::ISelectionParam)
	virtual const iprm::ISelectionConstraints* GetSelectionConstraints() const;
	virtual int GetSelectedOptionIndex() const;
	virtual bool SetSelectedOptionIndex(int index);
	virtual iprm::ISelectionParam* GetActiveSubselection() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& /*archive*/)
	{
		return true;
	}

private:
	class CShape: public iview::CShapeBase				
	{
	public:
		typedef iview::CShapeBase BaseClass;

		CShape(iprm::ISelectionParam& selection);

		// reimplemented (iview::IVisualizable)
		virtual void Draw(QPainter& drawContext) const;

	protected:
		void UpdateShape();

		// reimplemented (iview::CShapeBase)
		virtual i2d::CRect CalcBoundingBox() const;

	private:
		iprm::ISelectionParam& m_lineSelection;
	};

	int m_selectedLineIndex;

	imod::TModelWrap<imeas::CSimpleNumericValueProvider> m_results;
};


} // namespace iqtipr


#endif // !iqtipr_CMultiLineSupplierGuiComp_included

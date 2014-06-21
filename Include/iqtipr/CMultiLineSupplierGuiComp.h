#ifndef iqtipr_CMultiLineSupplierGuiComp_included
#define iqtipr_CMultiLineSupplierGuiComp_included


// ACF includes
#include "i2d/CPolyline.h"
#include "imeas/CSimpleNumericValueProvider.h"
#include "iprm/ISelectionParam.h"
#include "imod/CMultiModelDispatcherBase.h"

// ACF-Solutions includes
#include "iqtinsp/TSupplierGuiCompBase.h"

#include "GeneratedFiles/iqtipr/ui_CMultiLineSupplierGuiComp.h"


namespace iqtipr
{


class CMultiLineSupplierGuiComp:
			public iqtinsp::TSupplierGuiCompBase<
						Ui::CMultiLineSupplierGuiComp,
						imeas::INumericValueProvider>
{
	Q_OBJECT

public:
	typedef iqtinsp::TSupplierGuiCompBase<
				Ui::CMultiLineSupplierGuiComp,
				imeas::INumericValueProvider> BaseClass;

	I_BEGIN_COMPONENT(CMultiLineSupplierGuiComp);
		I_REGISTER_SUBELEMENT(LineSelection);
		I_REGISTER_SUBELEMENT_INTERFACE(LineSelection, iprm::ISelectionParam, ExtractLineSelection);
	I_END_COMPONENT;

	CMultiLineSupplierGuiComp();

protected Q_SLOTS:
	void on_TestButton_clicked();

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (iqtinsp::TSupplierGuiCompBase)
	virtual void OnSupplierParamsChanged();
	virtual QWidget* GetParamsWidget() const;

	// reimplemented (iqt2d::TViewExtenderCompBase)
	virtual void CreateShapes(int sceneId, Shapes& result);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiHidden();

protected:
	class LineSelection: public iprm::ISelectionParam
	{
	public:
		LineSelection();

		// reimplemented (iprm::ISelectionParam)
		virtual const iprm::IOptionsList* GetSelectionConstraints() const;
		virtual int GetSelectedOptionIndex() const;
		virtual bool SetSelectedOptionIndex(int index);
		virtual iprm::ISelectionParam* GetSubselection(int index) const;

		// reimplemented (iser::ISerializable)
		virtual bool Serialize(iser::IArchive& archive);

	protected:
		int m_selectedLineIndex;
	};

	class CShape:
				public iview::CShapeBase,
				protected imod::CMultiModelDispatcherBase
	{
	public:
		typedef iview::CShapeBase BaseClass;
		typedef imod::CMultiModelDispatcherBase BaseClass2;

		CShape(iprm::ISelectionParam& selection);
		virtual ~CShape();

		// reimplemented (iview::IVisualizable)
		virtual void Draw(QPainter& drawContext) const;

		// reimplemented (imod::CMultiModelDispatcherBase)
		virtual void OnModelChanged(int modelId, const istd::IChangeable::ChangeSet& changeSet);

	protected:
		// reimplemented (iview::CShapeBase)
		virtual i2d::CRect CalcBoundingBox() const;

	private:
		iprm::ISelectionParam& m_lineSelection;
	};

	imod::TModelWrap<LineSelection> m_lineSelection;

	static iprm::ISelectionParam* ExtractLineSelection(CMultiLineSupplierGuiComp& component)
	{
		return &component.m_lineSelection;
	}
};


} // namespace iqtipr


#endif // !iqtipr_CMultiLineSupplierGuiComp_included

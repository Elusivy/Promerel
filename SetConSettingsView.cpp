#include "SetConSettingsView.h"

SetConSettingsView::SetConSettingsView(QWidget *parent)
: QDialog{ parent },
ui_{},
branch_count_{}
{
	ui_.setupUi(this);
	setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

	connect(ui_.buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(ui_.buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
	connect(ui_.buttonBox, &QDialogButtonBox::accepted, this, &SetConSettingsView::OnSaveSettings);
}

SetConSettingsView::~SetConSettingsView()
{
}

void SetConSettingsView::OnSaveSettings()
{
	branch_count_ = ui_.spinBox->value();
}

int SetConSettingsView::GiveBranchCount()
{
	return branch_count_;
}

#include "ShowTheory.h"
#include <qtextstream.h>
#include "MyDataTypes.h"

ShowTheory::ShowTheory(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowTitle(QTEXT("Теоретические сведения"));
	ui.t->setReadOnly(true);
	//FillText();
}

ShowTheory::~ShowTheory()
{
}

void ShowTheory::FillText(const QString &file_name)
{
	QString text;
	QFile file(file_name);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QTextStream in(&file);
	while (!in.atEnd()) {
		QString line = in.readLine();
		text += line;
	}
	ui.t->setPlainText(text);
}

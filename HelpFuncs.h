#include <QString>
#include <QRegExp>

float GetFloatFromStr(const QString &str)
{
	bool is_checked_ = false;
	const float value = str.toFloat(&is_checked_);
	if (is_checked_)
		return value;
	else
		return 1001.;
}

int GetCommaPos(const QString &str)
{
	QRegExp r(QString(","));
	if (const int pos = r.indexIn(str))
	{
		return pos;
	}
	else
	{
		return  -1;
	}
}
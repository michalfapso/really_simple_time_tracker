#include "timer_data.h"
#include "time_helper.h"

#include <QString>
#include <QDateTime>
#include <QDir>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

#define DIR_NAME "timer_data"
#define DATETIME_FORMAT "yyyy-MM-dd_hh:mm:ss"

TimerData::TimerData(QObject* parent)
	: QObject(parent)
{
	Update();
}

void TimerData::ForEachTimer(std::function<void(const QString& name)> f)
{
	QDir dir(DIR_NAME);
	qDebug() << "current dir:" << dir.absolutePath();
	dir.setFilter(QDir::Files);
	dir.setSorting(QDir::Name);
	QFileInfoList list = dir.entryInfoList();
	for (auto fi : list) {
		if (fi.suffix() == "log") {
			f(fi.baseName());
		}
	}
}

void TimerData::Add(const QString& name, const QDateTime& startTime, const QDateTime& stopTime)
{
	QString filename = QString("%1/%2.log").arg(DIR_NAME).arg(name);
	QFile f(filename);
	if (!f.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
		QMessageBox::warning(nullptr, "Error", tr("Unable to open file '%1' for writing").arg(filename));
	}
	QTextStream t(&f);
	QString format = DATETIME_FORMAT;
	//qDebug() << mStartTime << " ... " << stop_time<<" = " << mStartTime.msecsTo(stop_time) << " = " << QDateTime::fromMSecsSinceEpoch(mStartTime.msecsTo(stop_time), QTimeZone::utc());
	t
		<< startTime.toString(format)
		<< '\t'
		<< seconds_to_datetime(startTime.msecsTo(stopTime)/1000).toString("hh:mm:ss")
		<< '\n';
}

void TimerData::Update()
{
	mData   .clear();
	mDataSum.clear();

	ForEachTimer([this](const QString& name){
		QString filename = QString("%1/%2.log").arg(DIR_NAME).arg(name);
		QFile f(filename);
		if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QMessageBox::warning(nullptr, "Error", tr("Unable to open file '%1' for reading").arg(filename));
		}
		QTextStream t(&f);

		while (!t.atEnd())
		{
			QString line = t.readLine(); //read one line at a time
			QStringList a = line.split(QRegExp("[\\s_]+"));
			//qDebug() << "a:" << a;
			QString date_str = a[0];
			auto it = mData[name].find(date_str);
			if (it == mData[name].end()) {
				mData[name][date_str] = 0;
			}
			QDateTime t0 = QDateTime::fromString("1900-01-01_00:00:00", DATETIME_FORMAT);
			QDateTime elapsed = QDateTime::fromString(a.last(), "hh:mm:ss");
			float elapsed_secs = t0.secsTo(elapsed);
			qDebug() << "elapsed:" << elapsed.toString(DATETIME_FORMAT) << " secsTo:"<<elapsed_secs;

			mData[name][date_str] += elapsed_secs;
		}
		qDebug() << "mData[" << name << "]: " << mData[name];
	});

	QSet<QString> dates;
	for (auto it = mData.begin(); it != mData.end(); it++) {
		for (auto it2 = it.value().begin(); it2 != it.value().end(); it2++) {
			dates.insert(it2.key());
		}
	}

	for (const QString& date : dates) {
		float sum = 0;
		for (auto it = mData.begin(); it != mData.end(); it++) {
			auto it2 = it.value().find(date);
			if (it2 == it.value().end()) {
				it.value().insert(date, 0);
			} else {
				sum += it2.value();
			}
		}
		mDataSum[date] = sum;
	}
}

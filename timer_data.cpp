#include "timer_data.h"
#include "time_helper.h"

#include <QString>
#include <QDateTime>
#include <QDir>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <cassert>

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

void TimerData::Start(const QString& name)
{
	qDebug() << "TimerData::Start() name:"<<name;
	auto start_time = QDateTime::currentDateTime();
	mStartTimes[name] = start_time;
}

void TimerData::Update(const QString& name)
{
	qDebug() << "TimerData::Start() name:"<<name;
	
	QString filename = GetFilename(name);
	QFile f(filename);
	if (!f.open(QIODevice::ReadWrite | QIODevice::Text)) {
		QMessageBox::warning(nullptr, "Error", tr("Unable to open file '%1' for reading and writing").arg(filename));
	}
	
	QByteArray last_line;
	qint64 last_line_pos = 0;
	while (!f.atEnd()) {
		last_line_pos = f.pos();
		last_line = f.readLine();
		qDebug() << "pos:"<<f.pos()<<" line:"<<QString::fromUtf8(last_line);
	}
	QString format = DATETIME_FORMAT;
	
	QDateTime start_time;
	if (!GetStartTime(name, &start_time))
	{
		qDebug() << "timer "<<name<<" has not started yet.";
		return;
	}
			
	QStringList a = QString::fromUtf8(last_line).split(QRegExp("[\\s]+"));
	QDateTime now = QDateTime::currentDateTime();
	qDebug() << "a[0]:"<<a[0]<<" start_time:"<<start_time.toString(format)<<" now:"<<now.toString(format)<<" msecsTo:"<<(start_time.msecsTo(now))<<" seconds:"<<(start_time.msecsTo(now)/1000)<<" toDateTime:"<<seconds_to_datetime(start_time.msecsTo(now)/1000)<<" toString:"<<seconds_to_datetime(start_time.msecsTo(now)/1000).toString("hh:mm:ss");
	// When the start time on the last line matches the current start time, that line is updated
	QString last_line_new;
	{
		QTextStream t(&last_line_new, QIODevice::WriteOnly);
		t
			<< start_time.toString(format)
			<< '\t'
			<< seconds_to_datetime(start_time.msecsTo(now)/1000).toString("hh:mm:ss")
			<< '\n';
	}
	if (a.size() >= 2 && a[0] == start_time.toString(format))
	{
		qDebug() << "equal -> updating last row";
		f.seek(last_line_pos);
		//qDebug() <<"written: "<<QString::fromUtf8(last_line_new);
		qDebug() <<"written: "<<last_line_new;
		f.write(last_line_new.toUtf8());
		f.resize(f.pos());
	} else {
		f.write(last_line_new.toUtf8());
	}
	//qDebug() << mStartTime << " ... " << stop_time<<" = " << mStartTime.msecsTo(stop_time) << " = " << QDateTime::fromMSecsSinceEpoch(mStartTime.msecsTo(stop_time), QTimeZone::utc());
}

void TimerData::Stop(const QString& name)
{
	QDateTime stop_time = QDateTime::currentDateTime();
	Update(name);
}

float TimerData::GetRunningSeconds(const QString& name)
{
	QDateTime start_time;
	if (!GetStartTime(name, &start_time)) {
		return 0;
	}
	QDateTime now = QDateTime::currentDateTime();
	return start_time.msecsTo(now)/1000;
}

QString TimerData::GetFilename(const QString& name)
{
	return QString("%1/%2.log").arg(DIR_NAME).arg(name);
}

bool TimerData::GetStartTime(const QString& name, QDateTime* startTimeOut)
{
	assert(startTimeOut);
	auto it = mStartTimes.find(name);
	if (it != mStartTimes.end()) {
		*startTimeOut = it.value();
		return true;
	}
	return false;
}


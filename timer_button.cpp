#include "timer_button.h"
#include <QTimeZone>
#include <QDebug>
#include <QLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDateTime>
#include <QTimer>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

#define DIR_NAME "timer_data"

namespace {
	QDateTime seconds_to_datetime(float seconds) {
		return QDateTime::fromMSecsSinceEpoch(seconds*1000, QTimeZone::utc());
	}
}

TimerButton::TimerButton(QWidget *parent)
	: QPushButton(parent)
{
	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->setContentsMargins(4, 0, 4, 0);
	layout->setSpacing(2);
	setLayout(layout);
	
	mpTimer = new QTimer(this);
	mpTimer->setTimerType(Qt::PreciseTimer);
	mpTimer->setInterval(1000);
	connect(mpTimer, &QTimer::timeout, [this]{
		mElapsedSeconds++;
		Update();
	});

//	mpLabelName = new QLabel(this);
//	layout->addWidget(mpLabelName);
	
//	mpLabelTime = new QLabel(this);
//	layout->addWidget(mpLabelTime);
}

void TimerButton::Update()
{
	QString time_format = mElapsedSeconds > 1200 ? "hh:mm:ss" : "mm:ss";
//	QString time = QDateTime::fromTime_t(mElapsedSeconds).toString(time_format);
	QString time = seconds_to_datetime(mElapsedSeconds).toString(time_format);
	
	setText(QString("%1 %2").arg(mName).arg(time));
}

void TimerButton::SetName(QString s)
{
	mName = s;
	Update();
//	mpLabelName->setText(s);
}

void TimerButton::SetElapsedSeconds(float f)
{
	mElapsedSeconds = f;
	Update();
}

void TimerButton::Stop()
{
	if (mpTimer->isActive()) {
		mpTimer->stop();
		setStyleSheet("");
		QDateTime stop_time = QDateTime::currentDateTime();
		{
			QString filename = QString("%1/%2.log").arg(DIR_NAME).arg(mName);
			QFile f(filename);
			if (!f.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
				QMessageBox::warning(this, "Error", tr("Unable to open file '%1' for writing").arg(filename));
			}
			QTextStream t(&f);
			QString format = "yyyy-MM-dd_hh:mm:ss";
			//qDebug() << mStartTime << " ... " << stop_time<<" = " << mStartTime.msecsTo(stop_time) << " = " << QDateTime::fromMSecsSinceEpoch(mStartTime.msecsTo(stop_time), QTimeZone::utc());
			t << mStartTime.toString(format) << '\t' << stop_time.toString(format) << '\t' << seconds_to_datetime(mStartTime.msecsTo(stop_time)/1000).toString("hh:mm:ss") << '\n';
		}
	}
}

void TimerButton::Start()
{
	mStartTime = QDateTime::currentDateTime();
	mpTimer->start();
	setStyleSheet("border: 1px solid #007700; color: #007700; font-weight: bold;");
}

bool TimerButton::IsRunning()
{
	return mpTimer->isActive();
}

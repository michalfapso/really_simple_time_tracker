#include "timer_button.h"
#include "timer_data.h"
#include "time_helper.h"
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

TimerButton::TimerButton(TimerData* pTimerData, QWidget *parent)
	: QPushButton(parent)
	, mpTimerData(pTimerData)
{
	setCheckable(true);
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
		//setStyleSheet("");
		QDateTime stop_time = QDateTime::currentDateTime();
		mpTimerData->Add(mName, mStartTime, stop_time);
	}
}

void TimerButton::Start()
{
	mStartTime = QDateTime::currentDateTime();
	mpTimer->start();
	//setStyleSheet("border: 1px solid #007700; color: #007700; font-weight: bold;");
	//setChecked(true);
}

bool TimerButton::IsRunning()
{
	return mpTimer->isActive();
}

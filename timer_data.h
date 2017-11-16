#ifndef TIMER_DATA_H
#define TIMER_DATA_H

#include <QObject>
#include <QMap>
#include <QDateTime>
#include <functional>
class QString;

class TimerData : public QObject
{
	Q_OBJECT
	public:
		typedef QMap<QString, float> Date2Seconds;
		typedef QMap<QString, Date2Seconds> Name2Date2Seconds;
		typedef QMap<QString, QDateTime> Name2StartTime;

		TimerData(QObject* parent = nullptr);
		void ForEachTimer(std::function<void(const QString& name)> f);
		void Update();
		const Name2Date2Seconds& GetData() const { return mData; }
		const Date2Seconds& GetDataSum() const { return mDataSum; }

		void Start (const QString& name);
		void Update(const QString& name);
		void Stop  (const QString& name);
		float GetRunningSeconds(const QString& name);
	private:
		QString GetFilename(const QString& name);
		bool GetStartTime(const QString& name, QDateTime* startTimeOut);
		
		Name2Date2Seconds mData; // timer_name -> date -> seconds
		Date2Seconds mDataSum; // timer_name -> date -> seconds
		Name2StartTime mStartTimes;
};

#endif // TIMER_DATA_H

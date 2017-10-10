#ifndef TIMER_DATA_H
#define TIMER_DATA_H

#include <QObject>
#include <QMap>
#include <functional>
class QString;
class QDateTime;

class TimerData : public QObject
{
	Q_OBJECT
	public:
		typedef QMap<QString, float> Date2Seconds;
		typedef QMap<QString, Date2Seconds> Name2Date2Seconds;

		TimerData(QObject* parent = nullptr);
		void ForEachTimer(std::function<void(const QString& name)> f);
		void Add(const QString& name, const QDateTime& startTime, const QDateTime& stopTime);
		void Update();
		const Name2Date2Seconds& GetData() const { return mData; }
		const Date2Seconds& GetDataSum() const { return mDataSum; }
	private:
		Name2Date2Seconds mData; // timer_name -> date -> seconds
		Date2Seconds mDataSum; // timer_name -> date -> seconds

};

#endif // TIMER_DATA_H

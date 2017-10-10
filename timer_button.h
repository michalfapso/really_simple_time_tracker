#ifndef TIMERBUTTON_H
#define TIMERBUTTON_H

#include <QPushButton>
#include <QDateTime>
class QLabel;
class QTimer;
class TimerData;

class TimerButton : public QPushButton
{
	Q_OBJECT
	public:
		TimerButton(TimerData* pTimerData, QWidget* parent = 0);
		void SetName(QString s);
		void SetElapsedSeconds(float f);
		void Stop();
		void Start();
		bool IsRunning();
	protected:
		void Update();
		TimerData* mpTimerData;
		float mElapsedSeconds;
		QString mName;
		QLabel* mpLabelName;
		QLabel* mpLabelTime;
		QTimer* mpTimer;
		QDateTime mStartTime;
};

#endif // TIMERBUTTON_H

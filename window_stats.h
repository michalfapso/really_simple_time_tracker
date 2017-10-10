#ifndef WINDOW_STATS_H
#define WINDOW_STATS_H

#include <QWidget>
class TimerData;

namespace Ui {
class WindowStats;
}

class WindowStats : public QWidget
{
		Q_OBJECT

	public:
		explicit WindowStats(TimerData* pTimerData, QWidget *parent = 0);
		~WindowStats();

	protected:
		void showEvent(QShowEvent *event) override;
		void keyPressEvent(QKeyEvent *event) override;

	signals:
		void hidden();

	private:
		Ui::WindowStats *ui;
		TimerData* mpTimerData;
};

#endif // WINDOW_STATS_H

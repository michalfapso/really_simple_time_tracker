#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>

namespace Ui {
class MainWindow;
}

class WindowStats;
class TimerData;
class QButtonGroup;

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	
protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void closeEvent(QCloseEvent *event);

private:
	void CreateTimerButton(QLayout* layout, QButtonGroup* buttonGroup, QString name, float elapsedSeconds);
	void StopAllTimers();
	Ui::MainWindow *ui;
	QPoint mDragPosition;
	WindowStats* mpWindowStats;
	TimerData* mpTimerData;
};

#endif // MAINWINDOW_H

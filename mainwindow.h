#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	
protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
private:
	void CreateTimerButton(QLayout* layout, QString name, float elapsedSeconds);
	void StopAllTimers();
	Ui::MainWindow *ui;
	QPoint mDragPosition;
};

#endif // MAINWINDOW_H

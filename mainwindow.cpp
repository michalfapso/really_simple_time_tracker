#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "timer_button.h"
#include <QHBoxLayout>
#include <QDebug>
#include <QDir>

#define DIR_NAME "timer_data"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	
	setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
//	setContentsMargins(0, 0, 0, 0);
//	setWindowFlags( Qt::CustomizeWindowHint );
	
	setStyleSheet("QPushButton { border: 1px solid #777777; }");
	
	QHBoxLayout* layout = new QHBoxLayout(ui->centralWidget);
	layout->setContentsMargins(2,0,2,0);
	
	int buttons_count = 0;
	{
		QDir dir(DIR_NAME);
		qDebug() << "current dir:" << dir.absolutePath();
		dir.setFilter(QDir::Files);
		dir.setSorting(QDir::Name);
		QFileInfoList list = dir.entryInfoList();
		for (auto fi : list) {
			if (fi.suffix() == "log") {
				CreateTimerButton(layout, fi.baseName(), 0);
				buttons_count ++;
			}
		}
	}
//	CreateTimerButton(layout, "T1", 60*60 + 11); buttons_count ++;
//	CreateTimerButton(layout, "T2",  5*60 +45); buttons_count ++;
	setMinimumWidth(90*buttons_count + 30);
	setMaximumWidth(90*buttons_count + 30);
	
	QPushButton* button_close = new QPushButton("X", ui->centralWidget);
	button_close->setMaximumWidth(20);
	connect(button_close, &QPushButton::clicked, [this]{
		StopAllTimers();
		close();
	});
	layout->addWidget(button_close);
	
	ui->centralWidget->setLayout(layout);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::CreateTimerButton(QLayout* layout, QString name, float elapsedSeconds)
{
	TimerButton* b = new TimerButton(ui->centralWidget);
	qDebug() << "creating timerButton: "<<b;
	b->SetName(name);
	b->SetElapsedSeconds(elapsedSeconds);
	layout->addWidget(b);
	connect(b, &QPushButton::clicked, [this, b]{
		bool run = !b->IsRunning();
		StopAllTimers();
		if (run) {
			b->Start();
		}
	});
}

void MainWindow::StopAllTimers()
{
	for (TimerButton* bb : ui->centralWidget->findChildren<TimerButton*>()) {
		qDebug() << "stopping timerButton: "<<bb;
		bb->Stop();
	}
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		mDragPosition = event->globalPos() - frameGeometry().topLeft();
		event->accept();
	}
}
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::LeftButton) {
		move(event->globalPos() - mDragPosition);
		event->accept();
	}
}

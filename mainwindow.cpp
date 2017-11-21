#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "timer_button.h"
#include "timer_data.h"
#include "window_stats.h"
#include <QHBoxLayout>
#include <QDebug>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	mpTimerData = new TimerData(this);
	mpWindowStats = new WindowStats(mpTimerData);
	mpWindowStats->setGeometry(0, 0, 500, 400);

	setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
//	setContentsMargins(0, 0, 0, 0);
//	setWindowFlags( Qt::CustomizeWindowHint );
	
	setStyleSheet(
		"QPushButton         { border: 1px solid #777777; }"
		"QPushButton:checked { border: 1px solid #007700; color: #007700; font-weight: bold; }"
	);
	
	QHBoxLayout* layout = new QHBoxLayout(ui->centralWidget);
	layout->setContentsMargins(2,0,2,0);

	int buttons_count = 0;
	QButtonGroup* button_group = new QButtonGroup(this);
	mpTimerData->ForEachTimer([&](QString name){
		CreateTimerButton(layout, button_group, name, 0);
		buttons_count ++;
	});
//	CreateTimerButton(layout, "T1", 60*60 + 11); buttons_count ++;
//	CreateTimerButton(layout, "T2",  5*60 +45); buttons_count ++;
	setMinimumWidth(90*buttons_count + 30);
	setMaximumWidth(90*buttons_count + 30);
	
	QPushButton* button_stats = new QPushButton("S", ui->centralWidget);
	button_stats->setMaximumWidth(20);
	button_stats->setCheckable(true);
	connect(button_stats, &QPushButton::toggled, [this](bool checked){
		if (checked) {
			QRect rect = mpWindowStats->geometry();
			rect.moveLeft(geometry().center().x() - rect.width()/2);
			rect.moveTop (geometry().bottom());
			mpWindowStats->setGeometry(rect);
			mpWindowStats->show();
		} else {
			mpWindowStats->hide();
		}
	});
	connect(mpWindowStats, &WindowStats::hidden, [this, button_stats]{
		button_stats->setChecked(false);
	});
	layout->addWidget(button_stats);

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
	mpWindowStats->close();
	delete ui;
}

void MainWindow::CreateTimerButton(QLayout* layout, QButtonGroup* buttonGroup, QString name, float elapsedSeconds)
{
	TimerButton* b = new TimerButton(mpTimerData, ui->centralWidget);
	qDebug() << "creating timerButton: "<<b;
	b->SetName(name);
	b->SetElapsedSeconds(elapsedSeconds);
	layout->addWidget(b);
	buttonGroup->addButton(b);
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

void MainWindow::closeEvent(QCloseEvent *event)
{
	mpWindowStats->close();
}

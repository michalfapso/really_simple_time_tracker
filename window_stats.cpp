#include "window_stats.h"
#include "timer_data.h"
#include "time_helper.h"
#include "ui_window_stats.h"
#include <QKeyEvent>
#include <QDebug>
#include <QScrollBar>

WindowStats::WindowStats(TimerData* pTimerData, QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::WindowStats)
	, mpTimerData(pTimerData)
{
	ui->setupUi(this);
	setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
}

WindowStats::~WindowStats()
{
	delete ui;
}

void WindowStats::showEvent(QShowEvent *event)
{
	mpTimerData->Update();
	const auto& data = mpTimerData->GetData();
	const auto& data_sum = mpTimerData->GetDataSum();
	ui->table->clear();

	// headers:
	{
		QStringList labels_horizontal;
		QStringList labels_vertical;
		for (auto it = data.begin(); it != data.end(); it++) {
			labels_horizontal << it.key();
		}
		labels_horizontal << "SUM";

		for (auto it = data_sum.begin(); it != data_sum.end(); it++) {
			labels_vertical << it.key();
		}
		std::reverse(labels_vertical.begin(), labels_vertical.end());
		ui->table->setColumnCount(labels_horizontal.size());
		ui->table->setRowCount(labels_vertical.size());
		ui->table->setHorizontalHeaderLabels(labels_horizontal);
		ui->table->setVerticalHeaderLabels  (labels_vertical);
		{
			auto item = new QTableWidgetItem("SUM");
			QFont font = item->font();
			font.setBold(true);
			item->setFont(font);
			ui->table->setHorizontalHeaderItem(ui->table->columnCount()-1, item);
		}
	}

	// projects:
	int col = 0;
	for (auto it = data.begin(); it != data.end(); it++) {
		int row = ui->table->rowCount()-1;
		for (auto it2 = it.value().begin(); it2 != it.value().end(); it2++) {
			ui->table->setItem(row, col, new QTableWidgetItem(seconds_to_datetime(it2.value()).toString("hh:mm:ss")));
			row--;
		}
		col++;
	}

	// sum:
	int row = ui->table->rowCount()-1;
	for (auto it = data_sum.begin(); it != data_sum.end(); it++) {
		auto item = new QTableWidgetItem(seconds_to_datetime(it.value()).toString("hh:mm:ss"));
		item->setBackgroundColor(QColor("#eeeeff"));
//		QFont font = item->font();
//		font.setBold(true);
//		item->setFont(font);
		ui->table->setItem(row, col, item);
		row--;
	}

	// resize window to table width
	{
		ui->table->resizeColumnsToContents();
		ui->table->hide();
		ui->table->show();

		int w = ui->table->horizontalHeader()->length() + ui->table->verticalHeader()->width();
		if (ui->table->verticalScrollBar()->isVisible()) {
			w += ui->table->verticalScrollBar()->width();
		}

		int h = ui->table->verticalHeader()->length() + ui->table->horizontalHeader()->height();
		if (ui->table->horizontalScrollBar()->isVisible()) {
			h += ui->table->horizontalScrollBar()->width();
		}

		QPoint center = geometry().center();
		QRect rect = geometry();
		rect.setWidth(w + layout()->margin()*2 + 4);
		rect.moveLeft(center.x() - rect.width()/2);
		if (h < 500) {
			rect.setHeight(h + layout()->margin()*2 + 4);
		}
		setGeometry(rect);
	}
}

void  WindowStats::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Escape) {
		hide();
		emit hidden();
	}
	int w = ui->table->verticalHeader()->width() + 4; // +4 seems to be needed
	qDebug() << "w: " << w;
}

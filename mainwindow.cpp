#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	m = 4;
	ui->spinBox->setMinimum(m);
	ui->spinBox->setValue(m);
    bClosed = ui->checkBox_Closed->isChecked();
    bClamped = ui->checkBox_Clamped->isChecked();
	ui->graphicsView->setFrameShape(QFrame::NoFrame);
	scene.setSceneRect(ui->graphicsView->rect());
	ui->graphicsView->setScene(&scene);
    pCurve = nullptr;
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
	if(ui->graphicsView->geometry().contains(event->pos()) &&
	   event->button() == Qt::LeftButton){
		if(cps.size() == m){
			cps.clear();
			scene.clear();
			pCurve = nullptr;
		}
		cps << event->pos() - ui->graphicsView->pos();
		scene.addEllipse(cps.last().rx() - 2, cps.last().ry() - 2, 4, 4);
		if(cps.size() == m)
			drawBsp();
	}
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
	m = arg1;
	cps.clear();
	scene.clear();
	pCurve = nullptr;
}

void MainWindow::on_checkBox_Closed_toggled(bool checked)
{
    bClosed = checked;
    if(bClosed && bClamped){
        ui->checkBox_Clamped->setChecked(false);
    }else{
        if(cps.size() == m)
            drawBsp();
    }
}

void MainWindow::on_checkBox_Clamped_toggled(bool checked)
{
    bClamped = checked;
    if(bClamped && bClosed){
        ui->checkBox_Closed->setChecked(false);
    }else{
        if(cps.size() == m)
            drawBsp();
    }
}

// B-スプラインを描画する
void MainWindow::drawBsp()
{
    if(pCurve){
		scene.removeItem(pCurve);
        delete pCurve;
		pCurve = nullptr;
	}
	pp.clear();
	if(bClamped){
        switch(m){
        case 4:
            drawClampedBspSegment(0);
            break;
        case 5:
            drawClampedBspSegment(0);
            drawClampedBspSegment(1);
            break;
        case 6:
            drawClampedBspSegment(0);
            drawClampedBspSegment(1);
            drawClampedBspSegment(2);
            break;
        default:
            drawClampedBspSegment(0);
            drawClampedBspSegment(1);
            for(int i = 2; i <= m - 6; i++)
                drawBspSegment(i);
            drawClampedBspSegment(m - 5);
            drawClampedBspSegment(m - 4);
            break;
        }
	}else{
        for(int i = 0; i <= m - 4; i++)
            drawBspSegment(i);
		if(bClosed){
            drawBspSegment(m - 3);
            drawBspSegment(m - 2);
            drawBspSegment(m - 1);
		}
	}
	pCurve = scene.addPath(pp);
}

// B-スプラインの1セグメントを描画する
void MainWindow::drawBspSegment(int i)
{
    QPointF p1 = cps[(i + 1) % m];
    QPointF p2 = cps[(i + 2) % m];
    QPointF p3 = cps[(i + 3) % m];
    QPointF bzp1 = (2 * p1 + p2) / 3;
    QPointF bzp2 = (p1 + 2 * p2) / 3;
    QPointF bzp3 = (p1 + 4 * p2 + p3) / 6;
    if(i == 0){
        QPointF p0 = cps[i];
        QPointF bzp0 = (p0 + 4 * p1 + p2) / 6;
		pp.moveTo(bzp0);
	}
	pp.cubicTo(bzp1, bzp2, bzp3);
}

// Clamped B-スプラインの1セグメントを描画する
void MainWindow::drawClampedBspSegment(int i)
{
    QPointF p1 = cps[i + 1];
    QPointF p2 = cps[i + 2];
    QPointF p3 = cps[i + 3];
    QPointF bzp1, bzp2, bzp3;
    switch(m){
    case 4:
        bzp1 = p1;
        bzp2 = p2;
        bzp3 = p3;
        break;
    case 5:
        if(i == 0){
            bzp1 = p1;
            bzp2 = (p1 + p2) / 2;
            bzp3 = (p1 + 2 * p2 + p3) / 4;
        }else{
            bzp1 = (p1 + p2) / 2;
            bzp2 = p2;
            bzp3 = p3;
        }
        break;
    case 6:
        if(i == 0){
            bzp1 = p1;
            bzp2 = (p1 + p2) / 2;
            bzp3 = (3 * p1 + 7 * p2 + 2 * p3) / 12;
        }else if(i == 1){
            bzp1 = (2 * p1 + p2) / 3;
            bzp2 = (p1 + 2 * p2) / 3;
            bzp3 = (2 * p1 + 7 * p2 + 3 * p3) / 12;
        }else{
            bzp1 = (p1 + p2) / 2;
            bzp2 = p2;
            bzp3 = p3;
        }
        break;
    default:
        if(i == 0){
            bzp1 = p1;
            bzp2 = (p1 + p2) / 2;
            bzp3 = (3 * p1 + 7 * p2 + 2 * p3) / 12;
        }else if(i == 1){
            bzp1 = (2 * p1 + p2) / 3;
            bzp2 = (p1 + 2 * p2) / 3;
            bzp3 = (p1 + 4 * p2 + p3) / 6;
        }else if(i == m - 5){
            bzp1 = (2 * p1 + p2) / 3;
            bzp2 = (p1 + 2 * p2) / 3;
            bzp3 = (2 * p1 + 7 * p2 + 3 * p3) / 12;
        }else{
            bzp1 = (p1 + p2) / 2;
            bzp2 = p2;
            bzp3 = p3;
            break;
        }
        break;
	}
    if(i == 0)
        pp.moveTo(cps[i]);
    pp.cubicTo(bzp1, bzp2, bzp3);
}

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPainterPath>
#include <QGraphicsPathItem>
#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

protected:
	void mousePressEvent(QMouseEvent *event) override;

private slots:
	void on_spinBox_valueChanged(int arg1);
    void on_checkBox_Closed_toggled(bool checked);
    void on_checkBox_Clamped_toggled(bool checked);

private:
	QGraphicsScene scene;
	QGraphicsPathItem *pCurve;
	QPainterPath pp;
	QList<QPointF> cps;	// コントロールポイント
	int m;				// コントロールポイント数
	bool bClosed;
	bool bClamped;
	void drawBsp();
    void drawBspSegment(int i);
    void drawClampedBspSegment(int i);

private:
	Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

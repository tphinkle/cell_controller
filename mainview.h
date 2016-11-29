#ifndef MAINVIEW_H
#define MAINVIEW_H

// Qt
#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

// Qwt
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_text.h>


namespace Ui {
class MainView;
}

class MainView : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainView(QWidget *parent = 0);
    ~MainView();

    // UI Elements

    // Syringe

    // Buttons
    QPushButton* syringe_local_button_;
    QPushButton* syringe_remote_button_;
    QPushButton* syringe_forward_button_;
    QPushButton* syringe_stop_button_;
    QPushButton* syringe_reverse_button_;
    QPushButton* syringe_switch_button_;
    QPushButton* syringe_set_rate_button_;

    // Labels and fields
    QLabel* syringe_rate_label_;
    QLineEdit* syringe_rate_field_;

    // RP

    // Plot and plot items
    QwtPlot* rp_plot_;
    QwtPlotCurve* rp_plot_curve_;


    // Buttons
    QPushButton* rp_create_task_button_;
    QPushButton* rp_start_task_button_;

private:
    Ui::MainView *ui;

    // Private objects



    // Private methods
    void setup();
    void setup_main_view();
    void setup_rp_view();
    void setup_oi_view();
    void setup_syringe_view();

};

#endif // MAINVIEW_H

#ifndef MAINVIEW_H
#define MAINVIEW_H

// Qt
#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFont>


// Qwt
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_text.h>



/////////////////////////////////////////////////////
// NOTES
/////////////////////////////////////////////////////
/// Document on fast QImage display:
/// http://www.qtcentre.org/threads/6929-Any-fast-way(-lt-10ms)-to-display-640*480-QImage-on-QGraphicsScene
///
/// Need to change way the UI is updated. Should create QTimer object for each UI element that is updated
/// and call a function update() when the timer is ready (through signal and slot connections). Then,
/// the UI needs to have a reference/pointer to the source of the data that it is responsible for displaying.
/// This reference/pointer should point to data allocated on the heap. Allocation occurs in the relevant ____model.
/// This significantly reduces the complexity of the UI calls.



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

    ///////////////////////////////////////////
    // Fonts
    ///////////////////////////////////////////

    QFont* major_label_font_;
    QFont* minor_label_font_;
    QFont* button_font_;
    QFont* checkbox_font_;


    ///////////////////////////////////////////
    // Syringe
    ///////////////////////////////////////////

    // Main label
    QLabel* syringe_label_;

    // Buttons
    QPushButton* syringe_local_button_;
    QPushButton* syringe_remote_button_;
    QPushButton* syringe_forward_button_;
    QPushButton* syringe_stop_button_;
    QPushButton* syringe_reverse_button_;
    QPushButton* syringe_switch_button_;
    QPushButton* syringe_get_rate_button_;
    QPushButton* syringe_set_rate_button_;

    // Labels and fields
    QLabel* syringe_current_rate_label_;
    QLineEdit* syringe_current_rate_field_;
    QLabel* syringe_set_rate_label_;


    QLineEdit* syringe_set_rate_field_;


    ///////////////////////////////////////////
    // RP
    ///////////////////////////////////////////

    // Label
    QLabel* rp_control_label_;


    // Plot and plot items
    QwtPlot* rp_plot_;
    QwtPlotCurve* rp_plot_curve_;
    QwtPlotCurve* rp_baseline_mean_plot_curve_;
    QwtPlotCurve* rp_baseline_lower_thresh_plot_curve_;
    QwtPlotCurve* rp_baseline_upper_thresh_plot_curve_;


    // Buttons
    QPushButton* rp_start_button_;
    QPushButton* rp_stop_button_;

    // Check boxes
    QCheckBox* rp_control_syringe_checkbox_;
    QCheckBox* rp_control_camera_checkbox_;

    ///////////////////////////////////////////
    // Camera
    ///////////////////////////////////////////

    // Label
    QLabel* camera_control_label_;

    // Buttons
    QPushButton* camera_start_button_;
    QPushButton* camera_stop_button_;

    // Plot
    QGraphicsScene* camera_scene_;
    QGraphicsPixmapItem* camera_pixmapitem_;
    QGraphicsView* camera_view_;



private:
    Ui::MainView *ui;

    // Private objects



    // Private methods
    void setup();
    void setup_main_view();
    void setup_rp_view();
    void setup_camera_view();
    void setup_syringe_view();
    void setup_layout();

};

#endif // MAINVIEW_H

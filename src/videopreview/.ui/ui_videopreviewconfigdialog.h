/********************************************************************************
** Form generated from reading UI file 'videopreviewconfigdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIDEOPREVIEWCONFIGDIALOG_H
#define UI_VIDEOPREVIEWCONFIGDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include "filechooser.h"

QT_BEGIN_NAMESPACE

class Ui_VideoPreviewConfigDialog
{
public:
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QLabel *label;
    FileChooser *filename_edit;
    QHBoxLayout *hboxLayout1;
    QLabel *dvd_device_label;
    QLineEdit *dvd_device_edit;
    QHBoxLayout *hboxLayout2;
    QLabel *label_2;
    QSpinBox *columns_spin;
    QSpacerItem *spacerItem;
    QLabel *label_3;
    QSpinBox *rows_spin;
    QCheckBox *osd_check;
    QFrame *line_2;
    QHBoxLayout *hboxLayout3;
    QLabel *label_5;
    QComboBox *aspect_ratio_combo;
    QSpacerItem *spacerItem1;
    QHBoxLayout *hboxLayout4;
    QLabel *label_4;
    QSpinBox *initial_step_spin;
    QSpacerItem *spacerItem2;
    QHBoxLayout *hboxLayout5;
    QLabel *label_6;
    QSpinBox *max_width_spin;
    QSpacerItem *spacerItem3;
    QFrame *line_3;
    QHBoxLayout *hboxLayout6;
    QLabel *label_7;
    QComboBox *format_combo;
    QSpacerItem *spacerItem4;
    QFrame *line;
    QCheckBox *save_last_directory_check;
    QFrame *line_4;
    QDialogButtonBox *button_box;

    void setupUi(QDialog *VideoPreviewConfigDialog)
    {
        if (VideoPreviewConfigDialog->objectName().isEmpty())
            VideoPreviewConfigDialog->setObjectName(QString::fromUtf8("VideoPreviewConfigDialog"));
        VideoPreviewConfigDialog->resize(452, 382);
        vboxLayout = new QVBoxLayout(VideoPreviewConfigDialog);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        label = new QLabel(VideoPreviewConfigDialog);
        label->setObjectName(QString::fromUtf8("label"));

        hboxLayout->addWidget(label);

        filename_edit = new FileChooser(VideoPreviewConfigDialog);
        filename_edit->setObjectName(QString::fromUtf8("filename_edit"));

        hboxLayout->addWidget(filename_edit);


        vboxLayout->addLayout(hboxLayout);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        dvd_device_label = new QLabel(VideoPreviewConfigDialog);
        dvd_device_label->setObjectName(QString::fromUtf8("dvd_device_label"));

        hboxLayout1->addWidget(dvd_device_label);

        dvd_device_edit = new QLineEdit(VideoPreviewConfigDialog);
        dvd_device_edit->setObjectName(QString::fromUtf8("dvd_device_edit"));

        hboxLayout1->addWidget(dvd_device_edit);


        vboxLayout->addLayout(hboxLayout1);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        label_2 = new QLabel(VideoPreviewConfigDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        hboxLayout2->addWidget(label_2);

        columns_spin = new QSpinBox(VideoPreviewConfigDialog);
        columns_spin->setObjectName(QString::fromUtf8("columns_spin"));
        columns_spin->setMinimum(1);
        columns_spin->setMaximum(10);

        hboxLayout2->addWidget(columns_spin);

        spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout2->addItem(spacerItem);

        label_3 = new QLabel(VideoPreviewConfigDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        hboxLayout2->addWidget(label_3);

        rows_spin = new QSpinBox(VideoPreviewConfigDialog);
        rows_spin->setObjectName(QString::fromUtf8("rows_spin"));
        rows_spin->setMinimum(1);

        hboxLayout2->addWidget(rows_spin);


        vboxLayout->addLayout(hboxLayout2);

        osd_check = new QCheckBox(VideoPreviewConfigDialog);
        osd_check->setObjectName(QString::fromUtf8("osd_check"));

        vboxLayout->addWidget(osd_check);

        line_2 = new QFrame(VideoPreviewConfigDialog);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(line_2->sizePolicy().hasHeightForWidth());
        line_2->setSizePolicy(sizePolicy);
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        vboxLayout->addWidget(line_2);

        hboxLayout3 = new QHBoxLayout();
        hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
        label_5 = new QLabel(VideoPreviewConfigDialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        hboxLayout3->addWidget(label_5);

        aspect_ratio_combo = new QComboBox(VideoPreviewConfigDialog);
        aspect_ratio_combo->setObjectName(QString::fromUtf8("aspect_ratio_combo"));

        hboxLayout3->addWidget(aspect_ratio_combo);

        spacerItem1 = new QSpacerItem(101, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout3->addItem(spacerItem1);


        vboxLayout->addLayout(hboxLayout3);

        hboxLayout4 = new QHBoxLayout();
        hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
        label_4 = new QLabel(VideoPreviewConfigDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        hboxLayout4->addWidget(label_4);

        initial_step_spin = new QSpinBox(VideoPreviewConfigDialog);
        initial_step_spin->setObjectName(QString::fromUtf8("initial_step_spin"));
        initial_step_spin->setMaximum(9999);

        hboxLayout4->addWidget(initial_step_spin);

        spacerItem2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout4->addItem(spacerItem2);


        vboxLayout->addLayout(hboxLayout4);

        hboxLayout5 = new QHBoxLayout();
        hboxLayout5->setObjectName(QString::fromUtf8("hboxLayout5"));
        label_6 = new QLabel(VideoPreviewConfigDialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        hboxLayout5->addWidget(label_6);

        max_width_spin = new QSpinBox(VideoPreviewConfigDialog);
        max_width_spin->setObjectName(QString::fromUtf8("max_width_spin"));
        max_width_spin->setMinimum(100);
        max_width_spin->setMaximum(2000);

        hboxLayout5->addWidget(max_width_spin);

        spacerItem3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout5->addItem(spacerItem3);


        vboxLayout->addLayout(hboxLayout5);

        line_3 = new QFrame(VideoPreviewConfigDialog);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        sizePolicy.setHeightForWidth(line_3->sizePolicy().hasHeightForWidth());
        line_3->setSizePolicy(sizePolicy);
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        vboxLayout->addWidget(line_3);

        hboxLayout6 = new QHBoxLayout();
        hboxLayout6->setObjectName(QString::fromUtf8("hboxLayout6"));
        label_7 = new QLabel(VideoPreviewConfigDialog);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        hboxLayout6->addWidget(label_7);

        format_combo = new QComboBox(VideoPreviewConfigDialog);
        format_combo->setObjectName(QString::fromUtf8("format_combo"));

        hboxLayout6->addWidget(format_combo);

        spacerItem4 = new QSpacerItem(71, 23, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout6->addItem(spacerItem4);


        vboxLayout->addLayout(hboxLayout6);

        line = new QFrame(VideoPreviewConfigDialog);
        line->setObjectName(QString::fromUtf8("line"));
        sizePolicy.setHeightForWidth(line->sizePolicy().hasHeightForWidth());
        line->setSizePolicy(sizePolicy);
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        vboxLayout->addWidget(line);

        save_last_directory_check = new QCheckBox(VideoPreviewConfigDialog);
        save_last_directory_check->setObjectName(QString::fromUtf8("save_last_directory_check"));

        vboxLayout->addWidget(save_last_directory_check);

        line_4 = new QFrame(VideoPreviewConfigDialog);
        line_4->setObjectName(QString::fromUtf8("line_4"));
        sizePolicy.setHeightForWidth(line_4->sizePolicy().hasHeightForWidth());
        line_4->setSizePolicy(sizePolicy);
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);

        vboxLayout->addWidget(line_4);

        button_box = new QDialogButtonBox(VideoPreviewConfigDialog);
        button_box->setObjectName(QString::fromUtf8("button_box"));
        button_box->setOrientation(Qt::Horizontal);
        button_box->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        vboxLayout->addWidget(button_box);

#if QT_CONFIG(shortcut)
        label->setBuddy(filename_edit);
        dvd_device_label->setBuddy(dvd_device_edit);
        label_2->setBuddy(columns_spin);
        label_3->setBuddy(rows_spin);
        label_5->setBuddy(aspect_ratio_combo);
        label_4->setBuddy(initial_step_spin);
        label_6->setBuddy(max_width_spin);
        label_7->setBuddy(format_combo);
#endif // QT_CONFIG(shortcut)
        QWidget::setTabOrder(filename_edit, dvd_device_edit);
        QWidget::setTabOrder(dvd_device_edit, columns_spin);
        QWidget::setTabOrder(columns_spin, rows_spin);
        QWidget::setTabOrder(rows_spin, osd_check);
        QWidget::setTabOrder(osd_check, aspect_ratio_combo);
        QWidget::setTabOrder(aspect_ratio_combo, initial_step_spin);
        QWidget::setTabOrder(initial_step_spin, max_width_spin);
        QWidget::setTabOrder(max_width_spin, format_combo);
        QWidget::setTabOrder(format_combo, button_box);

        retranslateUi(VideoPreviewConfigDialog);
        QObject::connect(button_box, SIGNAL(accepted()), VideoPreviewConfigDialog, SLOT(accept()));
        QObject::connect(button_box, SIGNAL(rejected()), VideoPreviewConfigDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(VideoPreviewConfigDialog);
    } // setupUi

    void retranslateUi(QDialog *VideoPreviewConfigDialog)
    {
        VideoPreviewConfigDialog->setWindowTitle(QCoreApplication::translate("VideoPreviewConfigDialog", "Thumbnail Generator", nullptr));
        label->setText(QCoreApplication::translate("VideoPreviewConfigDialog", "&File:", nullptr));
        dvd_device_label->setText(QCoreApplication::translate("VideoPreviewConfigDialog", "&DVD device:", nullptr));
        label_2->setText(QCoreApplication::translate("VideoPreviewConfigDialog", "&Columns:", nullptr));
        label_3->setText(QCoreApplication::translate("VideoPreviewConfigDialog", "&Rows:", nullptr));
        osd_check->setText(QCoreApplication::translate("VideoPreviewConfigDialog", "Add playing &time to thumbnails", nullptr));
        label_5->setText(QCoreApplication::translate("VideoPreviewConfigDialog", "&Aspect ratio:", nullptr));
        label_4->setText(QCoreApplication::translate("VideoPreviewConfigDialog", "&Seconds to skip at the beginning:", nullptr));
        label_6->setText(QCoreApplication::translate("VideoPreviewConfigDialog", "&Maximum width:", nullptr));
        label_7->setText(QCoreApplication::translate("VideoPreviewConfigDialog", "&Extract frames as", nullptr));
        save_last_directory_check->setText(QCoreApplication::translate("VideoPreviewConfigDialog", "Remember folder used to &save the preview", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VideoPreviewConfigDialog: public Ui_VideoPreviewConfigDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIDEOPREVIEWCONFIGDIALOG_H

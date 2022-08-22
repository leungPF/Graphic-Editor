#pragma once

#include <QDialog>
#include "ui_AboutDialog.h"

class AboutDialog : public QDialog/*, public Ui::AboutDialogClass*/
{
	Q_OBJECT

public:
	AboutDialog(QWidget* parent = nullptr);
	~AboutDialog();

private:

};

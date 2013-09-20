#ifndef ZERORESERVEDIALOG_H
#define ZERORESERVEDIALOG_H


#include "ui_ExampleDialog.h"

#include "retroshare-gui/mainpage.h"


class ExampleDialog : public MainPage
{
    Q_OBJECT

public:
    ExampleDialog( QWidget *parent = 0 );


private:

    Ui::ExampleDialog ui;

};

#endif // ZERORESERVEDIALOG_H

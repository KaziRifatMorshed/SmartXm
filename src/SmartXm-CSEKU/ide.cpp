#include "ide.h"
#include "ui_ide.h"

IDE::IDE(QWidget* parent) : QMainWindow(parent), ui(new Ui::IDE) { ui->setupUi(this); }

IDE::~IDE() { delete ui; }

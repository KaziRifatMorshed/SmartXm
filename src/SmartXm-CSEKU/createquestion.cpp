#include "createquestion.h"
#include "ui_createquestion.h"

CreateQuestion::CreateQuestion(QWidget* parent) : QWidget(parent), ui(new Ui::CreateQuestion) { ui->setupUi(this); }

CreateQuestion::~CreateQuestion() { delete ui; }

/*
 A C*KNOWLEDGEMENT: https://github.com/LynxLine/qpdfjs
 */
#include "src/qpdfjswindow.h"
#include <QApplication>
#include <QSettings>
#include <QFileInfo>
#include <QDir>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	a.setApplicationName("QPdfJs PDF Viewer");

	QString app_path = qApp->applicationDirPath();
#ifdef Q_OS_MACOS
	QDir app_path_dir(app_path);
	app_path_dir.cdUp();
	app_path_dir.cdUp();
	app_path_dir.cdUp();
	app_path = app_path_dir.absolutePath();
#endif

	QSettings s;
	QVector<QWidget *> windows;

	bool no_one = true;
	for(auto pdf_path : a.arguments()) {
		QFileInfo pdf_fi(pdf_path);
		if (!pdf_fi.exists()) continue;
		if (!pdf_path.endsWith(".pdf", Qt::CaseInsensitive)) continue;

		pdf_path = pdf_fi.absoluteFilePath();

		no_one = false;
		QPdfJsWindow * window = new QPdfJsWindow(pdf_path);
		int w = s.value("geomw", 1000).toInt();
		int h = s.value("geomh", 800).toInt();
		window->resize(w,h);
		window->show();
		windows.push_back(window);
	}

	if (no_one) {
		QString pdf_path = app_path+"/SmartXm.pdf";
		QPdfJsWindow * window = new QPdfJsWindow(pdf_path);
		int w = s.value("geomw", 1000).toInt();
		int h = s.value("geomh", 800).toInt();
		window->resize(w,h);
		window->show();
		windows.push_back(window);
	}

	int code = a.exec();

	for (auto window : windows) {
		if (window) {
			s.setValue("geomw", window->width());
			s.setValue("geomh", window->height());
		}
		delete window;
	}

	return code;
}

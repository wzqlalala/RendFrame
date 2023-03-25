#include "MxRendTest.h"
#include <QtWidgets/QApplication>
#include <QOpenGLContext>

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QApplication a(argc, argv);

	QSurfaceFormat format;
	format.setMajorVersion(4);
	format.setMinorVersion(5);
	format.setProfile(QSurfaceFormat::OpenGLContextProfile::CoreProfile);
	format.setSamples(4);
	QSurfaceFormat::setDefaultFormat(format);
	//setFormat(format);

	//QOpenGLContext *context1 = new QOpenGLContext;
	//context1->setFormat(format);
    //MxRendTest w(0);
    //w.show();

	//QOpenGLContext *context2 = new QOpenGLContext;
	//context2->setFormat(format);
	//context2->setShareContext(context1);
	//if (!context2->create()) {
	//	// context2´´½¨Ê§°Ü
	//}
	//context2->setFormat(format);
	MxRendTest w1(1);
	w1.show();

	//MxRendTest w2(2);
	//w2.show();
    return a.exec();
}

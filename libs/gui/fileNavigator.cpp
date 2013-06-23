
#include <QFileSystemModel>

#include "fileNavigator.h"


FileNavigator::FileNavigator(
	QWidget * parent)
:	QTreeView(parent)
,	m_model(new QFileSystemModel)
{
	this->setModel(m_model);

	setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
	setRoot(QDir::rootPath());

	this->setColumnHidden(1, true);
	this->setColumnHidden(2, true);
	this->setColumnHidden(3, true);

	QObject::connect(
        this, SIGNAL(activated(const QModelIndex &)),
        this, SLOT(emitClickedDirectory(const QModelIndex &)));

	QObject::connect(
        this, SIGNAL(clicked(const QModelIndex &)),
        this, SLOT(emitClickedDirectory(const QModelIndex &)));
};

void FileNavigator::setRoot(const QString & rootPath)
{
	this->setRootIndex(m_model->setRootPath(rootPath));
}

void FileNavigator::setFilter(const QDir::Filters & filters)
{
	m_model->setFilter(filters);
}

FileNavigator::~FileNavigator()
{
	delete m_model;
}

void FileNavigator::emitClickedDirectory(const QModelIndex & index)
{
	QString path = m_model->fileInfo(index).absoluteFilePath();
	emit clickedDirectory(path);
}

#include <iostream>
void FileNavigator::on_activatedDir(const QString & path)
{
    m_model->setRootPath(path);
    QDir * dir = new QDir(m_model->rootDirectory());

    while (dir->cdUp())
    {
        this->expand(m_model->index(m_model->rootPath()));
        m_model->setRootPath(dir->path());
    }

    delete dir;
}

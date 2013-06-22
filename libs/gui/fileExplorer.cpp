
#include <QFileSystemModel>
#include <QMenu>

#include "fileExplorer.h"



FileExplorer::FileExplorer(
	QWidget * parent)
:	QListView(parent)
,	m_model(new QFileSystemModel)
,	m_menu(new QMenu)
,	m_clickedFile()
{
	setModel(m_model);
	setContextMenuPolicy(Qt::CustomContextMenu);

	setFilter(QDir::NoDot | QDir::Files | QDir::AllDirs);
	setRoot("");

	m_model->setNameFilterDisables(false);

	this->menu()->addAction(QString("Open"), this, SLOT(triggeredOpen(const bool)));

	QObject::connect(
		this, SIGNAL(customContextMenuRequested(const QPoint &)),
		this, SLOT(showContextMenu(const QPoint &)));

	QObject::connect(
        this, SIGNAL(activated(const QModelIndex &)),
        this, SLOT(emitActivatedItem(const QModelIndex &)));
};

FileExplorer::~FileExplorer()
{
	delete m_model;
	delete m_menu;
}


QMenu * FileExplorer::menu()
{
	return m_menu;
}

QModelIndex FileExplorer::clickedFile()
{
	return m_clickedFile;
}


void FileExplorer::setFilter(const QDir::Filters & filters)
{
	m_model->setFilter(filters);
}

void FileExplorer::setAllLoadableTypes(const QStringList & list)
{
	m_model->setNameFilters(list);
}

void FileExplorer::setClickedFile(const QModelIndex & index)
{
	m_clickedFile = index;
}

void FileExplorer::triggeredOpen(const bool & triggered)
{
    emitActivatedItem(clickedFile());
}

void FileExplorer::showContextMenu(const QPoint & point)
{
	this->setClickedFile(this->indexAt(point));
	m_menu->exec(this->mapToGlobal(point));
}

void FileExplorer::emitActivatedItem(const QModelIndex & index)
{
	QString path = m_model->fileInfo(index).absoluteFilePath();

	if (m_model->fileInfo(index).isDir())
	{
		setRoot(path);
	} else {
		emit activatedItem(path);
	}
}

void FileExplorer::setRoot(QString rootPath)
{
	this->setRootIndex(m_model->setRootPath(rootPath));
}
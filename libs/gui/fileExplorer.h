
#pragma once



#include <QListView>
#include <QFileSystemModel>
#include <QMenu>

#include <core/declspec.h>


class FileNavigator;


class CGSEE_API FileExplorer : public QListView
{
	Q_OBJECT
	
public:
	FileExplorer(QObject * parent = nullptr);
	virtual ~FileExplorer();

	FileNavigator * navigator();

	void setNavigator(FileNavigator * fileNavigator);
	void setFilter(QDir::Filters filters);


public slots:
	void callSetRoot(const QModelIndex & index);
	void setClickedFile(const QModelIndex & index);
	void showContextMenu(const QPoint & point);
	void triggeredLoadFile(const bool & triggered);
	void loadFile(const QModelIndex & index);

protected:
	void setRoot(QString rootPath);

	FileNavigator * m_navigator;
	QFileSystemModel * m_model;
	QMenu * m_menu;
	
	QModelIndex m_clickedFile;
};
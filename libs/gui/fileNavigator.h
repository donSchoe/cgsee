
#pragma once



#include <QTreeView>
#include <QDir>

#include <core/declspec.h>


class QFileSystemModel;


class CGSEE_API FileNavigator : public QTreeView
{
	Q_OBJECT

public:
	FileNavigator(QObject * parent = nullptr);
	virtual ~FileNavigator();


	void setFilter(const QDir::Filters & filters);

protected:
	void setRoot(const QString & rootPath);

	QFileSystemModel * m_model;
};
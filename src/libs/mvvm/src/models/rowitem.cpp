#include "rowitem.h"
#include <QStack>
#include "rowitemmodel.h"
#include <QObject>

RowItem::RowItem(RowItemModel *rowItemModel, RowItem *parent)
    : m_parent(parent), m_row(-1), m_model(rowItemModel)
{
    if(parent != Q_NULLPTR)
    {
        parent->addChild(this);
    }
}

RowItem::~RowItem()
{
    removeChildren();
}

void RowItem::addChild(RowItem *item)
{
    if(item == Q_NULLPTR || m_children.contains(item))
    {
        return;
    }

    int index = m_children.size();
    m_model->beginInsertRows(m_model->indexFromItem(this), index, index);

    item->m_row = m_children.size();
    m_children.append(item);
    item->m_parent = this;

    m_model->endInsertRows();
}

void RowItem::insertChild(RowItem *item, int index)
{
    if (item == Q_NULLPTR || m_children.contains(item) || index < 0 || index > m_children.size())
    {
        return;
    }
    m_model->beginInsertRows(m_model->indexFromItem(this), index, index);

    item->m_row = index;
    m_children.insert(index, item);
    item->m_parent = this;

    int count = m_children.count();
    for(int i = index + 1; i < count; i++)
    {
        this->child(i)->setRow(i);
    }

    m_model->endInsertRows();
}

void RowItem::removeChild(RowItem* item, bool deletePtr)
{
    if (item == Q_NULLPTR || !m_children.contains(item))
    {
        return;
    }

    int index = item->row();

    m_model->beginRemoveRows(m_model->indexFromItem(this), index, index);

    m_children.removeOne(item);
    if(deletePtr) {
        delete item;
    }

    int  count = childCount();
    for(int i = index; i < count; i++)
    {
        this->child(i)->setRow(i);
    }
    m_model->endRemoveRows();
}

void RowItem::removeChild(int row, bool deletePtr)
{
    if (row < 0 || row >= m_children.size())
    {
        m_children.removeAt(row);
    }

    m_model->beginRemoveRows(m_model->indexFromItem(this), row, row);

    RowItem *item = m_children.takeAt(row);
    if(deletePtr) {
        delete item;
    }

    int  count = childCount();
    for(int i = row; i < count; i++)
    {
        this->child(i)->setRow(i);
    }

    m_model->endRemoveRows();
}

void RowItem::removeChildren(bool deletePtr)
{
    if(deletePtr) {
        qDeleteAll(m_children);
    }
    m_children.clear();
}

RowItemModel *RowItem::model() const
{
    return m_model;
}


QVariant RowItem::data(int column, int role) const
{    
    if (role == Qt::BackgroundRole)
    {
        return backgroundColor();
    }
    else if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        if(m_data.isEmpty())
        {
            return QVariant();
        }
        return m_data.at(column).isNull() ? QVariant() : m_data.at(column);
    }
    return QVariant();
}

void RowItem::setData(int column, const QVariant &value, int role)
{
    if (role == Qt::BackgroundRole)
    {
        m_color = value.value<QColor>();
    }
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        m_data[column] = value;
    }
}

int RowItem::level() const
{
    if (m_parent == Q_NULLPTR)
    {
        return 0;
    }

    int level = 1;
    RowItem *item = m_parent;
    while (item != m_model->root())
    {
        level++;
        item = item->parent();
    }
    return level;
}
void RowItem::setItemData(const QVariantList &list)
{
    m_data = list;

    QModelIndex topLeft = m_model->createIndex(m_row, 0, this);
    int acolumn = m_model->columnCount() - 1;
    QModelIndex bottomRight = m_model->createIndex(m_row, acolumn, this);
    emit  m_model->dataChanged(topLeft, bottomRight, QVector<int>({Qt::DisplayRole,Qt::EditRole}));
}

void RowItem::setItemData(const QObject &entity)
{
    QVariantList list;
    foreach(auto key, m_model->m_headerKeys)
    {
        auto kv = key.toLocal8Bit();
        auto value = entity.property(kv);
        list << value;
    }
    m_data = list;

    QModelIndex topLeft = m_model->createIndex(m_row, 0, this);
    int acolumn = m_model->columnCount() - 1;
    QModelIndex bottomRight = m_model->createIndex(m_row, acolumn, this);
    emit  m_model->dataChanged(topLeft, bottomRight, QVector<int>({Qt::DisplayRole,Qt::EditRole}));
}

void RowItem::setItemData(const QVariantMap &map)
{
    QVariantList list;
    foreach(auto key, m_model->m_headerKeys)
    {
        list << map.value(key);
    }
    m_data = list;

    QModelIndex topLeft = m_model->createIndex(m_row, 0, this);
    int acolumn = m_model->columnCount() - 1;
    QModelIndex bottomRight = m_model->createIndex(m_row, acolumn, this);
    emit  m_model->dataChanged(topLeft, bottomRight, QVector<int>({Qt::DisplayRole,Qt::EditRole}));
}

QVariantList RowItem::itemDataList() const
{
    return m_data;
}

void RowItem::setBackgroundColor(const QColor &color)
{
    m_color = color;
}

QColor RowItem::backgroundColor() const
{
    if (m_color.isValid())
    {
        return m_color;
    }
    else
    {
        return Qt::white;
    }
}

QVariantMap RowItem::itemDataMap() const
{
    QVariantMap result;
    const QStringList &headerKeys = m_model->m_headerKeys;

    for (int i = 0; i < headerKeys.size(); i++)
    {
        result.insert(headerKeys.at(i), m_data.at(i));
    }
    return result;
}



QVariant RowItem::value(const QString &headerKey) const
{
    int index = m_model->m_headerKeys.indexOf(headerKey);
    if (index < 0 || index > m_data.size())
    {
        return QVariant();
    }

    return m_data.at(index);
}

void RowItem::setValue(const QString &key, const QVariant &value)
{
    if(m_data.isEmpty())
    {
        return;
    }

    int index = m_model->m_headerKeys.indexOf(key);
    if (index < 0)
    {
        return ;
    }

    m_data.replace(index, value);

    QModelIndex topLeft = m_model->createIndex(m_row, 0, this);
    int acolumn = m_model->columnCount() - 1;
    QModelIndex bottomRight = m_model->createIndex(m_row, acolumn, this);
    emit m_model->dataChanged(topLeft, bottomRight, QVector<int>({Qt::DisplayRole,Qt::EditRole}));
}

void RowItem::setModel(RowItemModel *model)
{
    if (m_children.isEmpty())
    {
        m_model = model;
    }
    else
    {
        QStack<RowItem *> stack;
        stack.push(this);
        while (!stack.isEmpty())
        {
            RowItem *item = stack.pop();
            item->m_model = model;

            const QList<RowItem *> &childList = item->m_children;
            int count = childList.count();
            for (int i = 0; i < count; ++i)
            {
                RowItem *child = childList.at(i);
                if (child != Q_NULLPTR)
                {
                    stack.push(child);
                }
            }
        }
    }
}

QList<RowItem *> RowItem::children() const
{
    return m_children;
}









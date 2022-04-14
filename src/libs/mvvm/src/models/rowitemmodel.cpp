#include "rowitemmodel.h"
#include "rowitem.h"

RowItemModel::RowItemModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    m_rootItem = new RowItem(this);
    m_rootItem->m_model = this;
}

RowItemModel::RowItemModel(const QStringList& headers, QObject *parent)
    : QAbstractItemModel(parent)
{
    m_headers = headers;
    m_rootItem = new RowItem(this);
    m_rootItem->m_model = this;
}

RowItemModel::~RowItemModel()
{
    m_rootItem->removeChildren();
    delete m_rootItem;
}

RowItem *RowItemModel::itemFromIndex(const QModelIndex &index) const
{
    if (index.isValid())
    {
        RowItem *item = static_cast<RowItem*>(index.internalPointer());
        return item;
    }
    return m_rootItem;
}

RowItem *RowItemModel::root() const
{
    return m_rootItem;
}

QVariant RowItemModel::headerData(int section, Qt::Orientation orientation,
                                  int role) const
{
    if (orientation == Qt::Horizontal)
    {
        if(role == Qt::DisplayRole)
        {
            return m_headers.at(section);
        }
    }
    return QVariant();
}

QVariant RowItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    RowItem *item = itemFromIndex(index);
    if (role == Qt::DisplayRole || role == Qt::EditRole || role == Qt::BackgroundRole)
    {
        int column = index.column();
        QVariant data = item->data(column, role);
        if (!m_relationMaps.contains(column))
        {
            return data;
        }
        else
        {
            return m_relationMaps.value(column).value(data.toString());
        }
    }
    return QVariant();
}

bool RowItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
    {
        return false;

    }

    RowItem *item = itemFromIndex(index);

    int column = index.column();
    if (!m_relationMaps.contains(column))
    {
        item->setData(column, value, role);
    }
    else
    {
        item->setData(column, m_relationMaps.value(column).key(value), role);
    }

    QVector<int> roles((role == Qt::DisplayRole) ?
                           QVector<int>({Qt::DisplayRole, Qt::EditRole}) :
                           QVector<int>({role}));

    //发出信号 通知 data函数重新读取数据
    if(role == Qt::BackgroundRole)
    {
        emit dataChanged(createIndex(item->row(), 0, item),
                         createIndex(item->row(), columnCount() - 1, item), roles);
    }
    else
    {
        emit dataChanged(index, index, roles);
    }

    return true;
}

bool RowItemModel::setHeaderData(int section, Qt::Orientation orientaion,
                                 const QVariant &value, int role)
{
    if (role != Qt::EditRole && orientaion == Qt::Horizontal) {
        m_headers.replace(section, value.toString());
        emit headerDataChanged(Qt::Horizontal, section, section);
        return true;
    }
    return false;
}

QModelIndex RowItemModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    RowItem *parentItem = itemFromIndex(parent);
    RowItem *item = parentItem->child(row);
    if (item)
        return createIndex(row, column, item);
    else
        return QModelIndex();
}

QModelIndex RowItemModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    RowItem *item = itemFromIndex(index);
    RowItem *parentItem = item->parent();

    if (parentItem == Q_NULLPTR || parentItem == m_rootItem)
    {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

int RowItemModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    RowItem* item = itemFromIndex(parent);
    return item->childCount();
}

int RowItemModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_headers.size();
}

QModelIndex RowItemModel::indexFromItem(RowItem *item) const
{
    if (item != Q_NULLPTR && item->parent() != Q_NULLPTR) {
        return createIndex(item->row(), 0, item);
    }
    return QModelIndex();
}

void RowItemModel::clear()
{
    beginResetModel();

    m_rootItem->removeChildren();

    endResetModel();
}

Qt::ItemFlags RowItemModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
    {
        return Qt::ItemIsEnabled;
    }
    //保留原来已经存在的那些标记。
    return QAbstractItemModel::flags(index) |= Qt::ItemIsEditable;
}

bool RowItemModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (row < 0 || row > rowCount(parent) || count <= 0)
    {
        return true;
    }

    beginInsertRows(parent, row, row + count -1);

    RowItem* parentItem = itemFromIndex(parent);
    for(int i =  0; i < count; i++)
    {
        new RowItem(this, parentItem);
    }

    endInsertRows();
    return true;
}
bool RowItemModel::removeRows(int row, int count, const QModelIndex &parent)
{
    int rows = rowCount(parent);
    if (rows <= 0 || rows < row + count)
    {
        return false;
    }

    beginRemoveRows(parent, row, row + count - 1);

    RowItem* parentItem = itemFromIndex(parent);
    for(int i = 0; i < count; i++)
    {
        parentItem->removeChild(row);
    }

    endRemoveRows();
    return true;
}
bool RowItemModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    return QAbstractItemModel::insertColumns(column, count, parent);
}
bool RowItemModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    return QAbstractItemModel::removeColumns(column, count, parent);
}

void RowItemModel::setHeaders(const QStringList &headers)
{
    beginResetModel();
    m_headers = headers;
    emit headerDataChanged(Qt::Horizontal, 0, m_headers.size() - 1);
    endResetModel();
}

QStringList RowItemModel::headerKeys() const
{
    return m_headerKeys;
}

void RowItemModel::setHeaderKeys(const QStringList &headerKeys)
{
    m_headerKeys = headerKeys;
}

QVariantMap RowItemModel::getDataMap(RowItem *item) const
{
    QVariantMap result;
    for (int i = 0; i < m_headerKeys.size(); i++)
    {
        result.insert(m_headerKeys.at(i), item->data(i));
    }
    return result;
}

void RowItemModel::setRelationMap(int column, const QMap<QString, QVariant> &relationMap)
{
    m_relationMaps.insert(column, relationMap);
}

void RowItemModel::setRelationMap(const QString &columnName, const QMap<QString, QVariant> &relationMap)
{
    int index = m_headerKeys.indexOf(columnName);
    if (index >= 0)
    {
        m_relationMaps.insert(index, relationMap);
    }
}

int RowItemModel::headerIndex(const QString &columnName) const
{
    return m_headerKeys.indexOf(columnName);
}

void RowItemModel::constructTree(const QList<QVariantList> &source, int idIndex, int pidIndex, int rootId)
{
    int size = source.size();

    RowItem *rootItem = m_rootItem;
    RowItem *item = Q_NULLPTR;
    RowItem *parentItem = Q_NULLPTR;
    int parentId(-1);
    QHash<int, RowItem *> items;//存放树节点的id和对应的 Item


    int rootIndex = 0;
    if (rootId != -1)//根节点默认为结果集第一行,不在第一行时，先找根节点
    {
        for (; rootIndex < size; rootIndex++)
        {
            const QVariantList &row = source.at(rootIndex);
            if (row.at(idIndex) == rootId)
            {
                break;
            }
        }
    }
    if (rootIndex >= size) return;//找不到根节点
    //构造根节点
    const QVariantList &row = source.at(rootIndex);
    item = new RowItem(this);
    item->setItemData(row);
    parentItem = rootItem;
    parentItem->addChild(item);
    auto id = row.at(idIndex).toInt();
    items.insert(row.at(idIndex).toInt(), item);
    parentId = row.at(idIndex).toInt();
    parentItem = item;


    for(int i = rootIndex + 1; i < size; i++)
    {
        const QVariantList &row = source.at(i);
        item = new RowItem(this);
        item->setItemData(row);

        if (parentId != row.at(pidIndex).toInt())
        {
            parentId = row.at(pidIndex).toInt();
            parentItem = items.value(parentId);
        }
        //处理脏数据
        if(parentItem == Q_NULLPTR)
        {
            continue;
        }

        parentItem->addChild(item);
        id = row.at(idIndex).toInt();
        items.insert(row.at(idIndex).toInt(), item);
    }
}

QMap<QString, QVariant> RowItemModel::getRelationMap(int column) const
{
    return m_relationMaps.value(column);
}

QMap<QString, QVariant> RowItemModel::getRelationMap(const QString &columnName) const
{
    int column = m_headerKeys.indexOf(columnName);
    return m_relationMaps.value(column);
}










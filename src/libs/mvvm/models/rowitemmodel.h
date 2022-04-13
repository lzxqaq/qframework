
/******************************************************************************
 *
 * @file       rowitemmodel.h
 * @brief      通用的行模型
 *
 * @author     lzx
 * @date       2021/09/29
 *
 * @history
 *****************************************************************************/


#ifndef RPWITEMMODEL_H
#define RPWITEMMODEL_H

#include <QAbstractItemModel>
#include "../mvvm_global.h"

class RowItem;

class MVVM_EXPORT RowItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:

    explicit RowItemModel(QObject *parent = nullptr);
    explicit RowItemModel(const QStringList& headers, QObject *parent = nullptr);
    ~RowItemModel();

    /******************************************************************************
     *
     *                            模型显示需实现
     *
     *****************************************************************************/
    /**
     * @brief 返回给定索引的 Item 项标志。
     */
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    /**
     * @brief 返回存储在给定索引、角色下的数据。
     */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    /**
     * @brief 返回给定父节点索引下的行数。 当父节点有效时，意味着rowCount将返回父节点的子节点数量。
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    /**
     * @brief 返回给定父节点索引下的子节点的列数。
     */
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    /**
     * @brief 给定角色和位置，返回指定方向下，在标题中对应的数据。
     */
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;



    /******************************************************************************
     *
     *                           模型编辑需实现
     *
     *****************************************************************************/
    /**
     * @brief 将索引处的项的角色数据设置为value。
     */
    bool setData(const QModelIndex &index, const QVariant &value, int role = 0)  override;

    /**
     * @brief 将标题中给定角色和位置的数据以指定方向设置为提供的值。
     */
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;


    /******************************************************************************
     *
     *                            模型插入和删除需实现
     *
     *****************************************************************************/
    /**
     * @brief 在给定行之前插入指定数量的行到模型中
     */
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    /**
     * @brief 从模型中删除以父索引下的给定行开始的指定数量行。
     */
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    /**
     * @brief 在给定列之前插入指定数量的列到模型中
     */
    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    /**
     * @brief 从模型中删除以父索引下的给定列开始的指定数量列。
     */
    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;




    /******************************************************************************
     *
     *                            自定义模型索引
     *
     *****************************************************************************/

    /**
     * @brief 返回由给定的行、列和父索引指定的模型的索引。
     */
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

    /**
     * @brief 返回具有给定索引的模型项的父项。 如果项没有父项，则返回无效的QModelIndex。
     */
    QModelIndex parent(const QModelIndex &index) const override;




    /******************************************************************************
     *
     *                            以下部分为扩展功能
     *
     *****************************************************************************/
    /**
     * @brief 根据 Index 索引返回对应的 Item。
     */
    RowItem *itemFromIndex(const QModelIndex &index) const;

    /**
     * @brief 根据 Item 返回对应的 Index索引
     */
    QModelIndex indexFromItem(RowItem* item) const;

    /**
     * @brief 清空 Model 的所有项
     */
    void clear();

    /**
     * @brief 返回根
     */
    RowItem *root() const;

    /**
     * @brief 设置列的图标
     */
//    void setColIcons(int index, const QVariantList &values, const QStringList &icons, int sourceCol = -1);

    /**
     * @brief 设置表头
     */
    void setHeaders(const QStringList &headers);

    /**
     * @brief 返回表头字段
     */
    QStringList headerKeys() const;

    /**
     * @brief 设置表头字段，与表头一一对应
     */
    void setHeaderKeys(const QStringList &headerKeys);

    /**
     * @brief 将 Item 的数据组装成 map 返回
     */
    QVariantMap getDataMap(RowItem *item) const;

    /**
     * @brief 设置某一列的关系映射
     */
    void setRelationMap(int column, const QMap<QString, QVariant> &relationMap);

    /**
     * @brief 设置某一列的关系映射
     */
    void setRelationMap(const QString &columnName, const QMap<QString, QVariant> &relationMap);

    /**
     * @brief 返回某一列的关系映射
     */
    QMap<QString, QVariant> getRelationMap(int column) const;

    /**
     * @brief 返回某一列的关系映射
     */
    QMap<QString, QVariant> getRelationMap(const QString &columnName) const;


    /**
     * @brief 返回给定表头字段在表头对应的位置
     */
    int headerIndex(const QString &columnName) const;

    /**
     * @brief 构造节点为{id, pid}类型的树结构 Model，注意 source 为排序后的数据集，idIndex 为节点编号的索引，
     *        pid 为父节点编号的索引,默认根节点为数据集的第一行
     */
    void constructTree(const QList<QVariantList> &source, int idIndex, int pidIndex, int rootId = -1);

protected:
    QStringList m_headers; //表头
    QStringList m_headerKeys; //表头字段
    QHash<int, QMap<QString, QVariant>> m_relationMaps; //关系映射集合
    RowItem* m_rootItem; //根项

    friend class RowItem;
};


#endif // RPWITEMMODEL_H

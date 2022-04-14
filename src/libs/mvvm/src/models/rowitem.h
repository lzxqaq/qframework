/******************************************************************************
 *
 * @file       rowitem.h
 * @brief      通用的行 Item，管理一行的数据，并且实现一些通用的方法，如管理背景颜色，图标等
 *
 * @author     lzx
 * @date       2021/09/29
 *
 * @history
 *****************************************************************************/

#ifndef ROWITEM_H
#define ROWITEM_H

#include <QObject>
#include <QVariant>
#include <QList>
#include <QColor>
#include "../mvvm_global.h"

class RowItemModel;

class BaseVO;
/**
 * @brief 通用的行 Item
 */


class MVVM_EXPORT RowItem
{
public:
    explicit RowItem(RowItemModel *rowItemModel, RowItem *parent = nullptr);
    virtual ~RowItem();

    /**
     * @brief 添加子项（在末尾添加子项）
     */
    void addChild(RowItem *item);

    /**
     * @brief 添加子项（指定位置索引）。
     */
    void insertChild(RowItem *item, int index);

    /**
     * @brief 删除子项，默认析构指针
     */
    void removeChild(RowItem* item, bool deletePtr = true);

    /**
     * @brief 删除子项，默认析构指针
     */
    void removeChild(int row, bool deletePtr = true);

    /**
     * @brief 清空所有子项，默认析构指针
     */
    void removeChildren(bool deletePtr = true);

    /**
     * @brief 根据索引获取对应的子项
     */
    RowItem *child(int row) const { return m_children.value(row, Q_NULLPTR); }

    /**
     * @brief 返回父项的指针。
     */
    RowItem *parent() const { return m_parent; }

    /**
     * @brief 返回子项数量
     */
    int childCount() const { return m_children.count(); }

    /**
     * @brief 返回 Item 绑定的 Model。
     */
    RowItemModel *model() const;

    /**
     * @brief 返回 item 中的数据。column:列索引， role:角色
     */
    virtual QVariant data(int column, int role = Qt::DisplayRole) const;

    /**
     * @brief 设置 item 中的数据。column:列索引，value:设置的值 role:角色
     */
    virtual void setData(int column, const QVariant &value, int role= Qt::EditRole);


    /**
     * @brief 保存该节点是其父项的第几个子项，查询优化所用
     */
    void setRow(int row) { m_row = row; }
    /**
     * @brief 返回本节点位于父项下第几个子项
     */
    int row() const { return m_row; }

    /**
     * @brief 返回本节点处于树的层级,从 1 开始
     */
    int level() const;

    /**
     * @brief 设置节点存的数据
     */
    virtual void setItemData(const QVariantList &list);

    /**
     * @brief 设置节点存的数据
     */
    virtual void setItemData(const QObject &entity);

    /**
     * @brief 设置节点存的数据
     */
    virtual void setItemData(const QVariantMap &map);

    /**
     * @brief 获取节点存的数据
     */
    virtual QVariantList itemDataList() const;

    /**
     * @brief 将 Item 的数据组装成 map 返回
     */
    QVariantMap itemDataMap() const;

    /**
     * @brief 根据字段名从 Item 中取出数据
     */
    QVariant value(const QString &headerKey) const;

    /**
     * @brief 根据字段名将数据存入 Item
     */
    void setValue(const QString &key, const QVariant &value);

    /**
     * @brief 设置背景颜色
     */
    void setBackgroundColor(const QColor &color);

    /**
     * @brief 返回背景颜色
     */
    QColor backgroundColor() const;

    QList<RowItem *> children() const;

private:

    /**
     * @brief   给 Item 和它的孩子们设置 model
     */
    void setModel(RowItemModel *model);


private:
    QList<RowItem *> m_children;// 子项
    RowItem *m_parent;// 父项

    QColor m_color;// 背景颜色
    int m_row;// 此item位于父项中第几个
    QVariantList m_data;// 存储的数据
    RowItemModel *m_model; //绑定的 Model
    friend class RowItemModel;

};

#endif // ROWITEM_H

#ifndef PICTUREMATCHINGCORE_H
#define PICTUREMATCHINGCORE_H

// qt 支持
#ifdef QT_CORE_LIB
#include <QObject>
#endif

#include <vector>
#include <list>
#include <queue>
#include <thread>

#include "LockClass.h"

// 0 表示没有图片
#define CELL_NO_HAS_IMAGE 0

// 单元格无效值
#define CELL_INVALID 0xffffffff

// 连连看核心算法
class PictureMatchingCore
{
public:
    PictureMatchingCore();
    ~PictureMatchingCore();

public:
    // 如果创建成功, 返回值为空, 否则返回错误信息
    std::string CreateMap(int row, int col, int imageKind);

    // 重新布局, 位置不变, 重新排列
    bool ReLayout();

    // 清空
    void Clean();

    // 是否是空的
    bool IsEmpty();

    // 是否是失效的数据
    bool IsInvalid() { LockClass lock(&m_mutex); return m_invalid; }

    // 返回 直线数 加 0, 途径所有点的坐标
    std::vector<int> Connect(int srcRow, int srcCol, int destRow, int destCol);

    // 得到 第row行(从1开始数) 第col列(从1开始数) 的单元格  返回 CELL_INVALID 表示值无效
    int GetCell(int row, int col);

    // 设置 第row行(从1开始数) 第col列(从1开始数) 的单元格   返回 true 表示成功
    bool SetCell(int row, int col, int value);

    // 获得行列信息, 错误返回 0
    int GetRow() { LockClass lock(&m_mutex); return m_invalid ? 0 : m_row - 2; }
    int GetCol() { LockClass lock(&m_mutex); return m_invalid ? 0 : m_col - 2; }

    // 获得图片种类的个数
    int GetImageKind() { LockClass lock(&m_mutex); return m_invalid ? 0 : m_imageKind; }

public:
    // 在构造函数中生成种子
    // 生成 从 frist 到 last 之间的随机数, 包含 frist, 但不包含 last
    // last 要大于 frist
    int Random(int frist, int last);

public:
    // 判断两个点是否能连接的最大直线数
    static const int MaxConnectLine = 3;

    // 图片最少与最多的种类
    static const int MinimageKind = 8;
    static const int MaximageKind = 100;

    // 最大与最小的尺寸
    static const int MinUnitNumber = 1 * 1;
    static const int MaxUnitNumber = 16 * 16;

protected:

    // 方向
    enum Direction
    {
        To_No,
        To_Top,
        To_Bottom,
        To_Left,
        To_Right
    };

    struct CellPoint
    {
        int m_row;		// 行坐标
        int m_col;		// 列坐标

        // 0: 无方向
        // 1: 向上
        // 2: 向下
        // 3: 向左
        // 4: 向右
        Direction m_direction;	// 方向

        int m_lineNumber;	// 直线数
    };

    // 互斥量		当前节点		新节点容器		地图镜像
    void FindNext(std::mutex *pMutex, std::vector<struct CellPoint> node, std::list<std::vector<struct CellPoint>> *pPush, std::vector<int> *pMap);

private:
    std::mutex m_mutex;

    // 当 m_invalid 值为 true 时, 表示m_map数据无效, 应该使用 CreateMap 重新创建
    bool m_invalid;

    // 假设创建的地图是8x12的, 该值为 10 , 地图中有效的数据为从1 - 8, 0与9为边框
    int m_row;

    // 假设创建的地图是12x8的, 该值为 10 , 地图中有效的数据为从1 - 8, 0与9为边框
    int m_col;

    // 图片种类
    int m_imageKind;

    // 0 表示没有图片
    // 假设创建的地图为 8x8 的, 则地图大小为100, 中间的8x8为有效的数据, 四周为边框
    std::vector<int> m_map;
};

#endif // PICTUREMATCHINGCORE_H

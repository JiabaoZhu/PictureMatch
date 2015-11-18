#include "PictureMatchingCore.h"

#include <time.h>
#include <stdlib.h>
#include <sstream>

// 判断一个数是不是偶数
#define _NUMBER_IS_EVEN_(x) (((x & 0x00000001) == 0) ? true : false)

PictureMatchingCore::PictureMatchingCore()
{
    m_row = 0;
    m_col = 0;
    m_imageKind = 0;
    m_map.clear();

    m_invalid = true;

    // 产生随机数种子
    srand((int)time(NULL));
}

PictureMatchingCore::~PictureMatchingCore()
{
}

void PictureMatchingCore::Clean()
{
    LockClass lock(&m_mutex);

    m_row = 0;
    m_col = 0;
    m_imageKind = 0;
    m_map.clear();

    m_invalid = true;
}

bool PictureMatchingCore::IsEmpty()
{
    if (m_invalid)
    {
        return false;
    }
    int totalCellNember = m_row * m_col;
    for (int i = 0; i < totalCellNember; i++)
    {
        // 地图不是 0
        if (CELL_NO_HAS_IMAGE != m_map[i])
        {
            return false;
        }
    }
    return true;
}

std::string PictureMatchingCore::CreateMap(int row, int col, int imageKind)
{
    // 线程锁
    LockClass lock(&m_mutex);

    // 数据无效
    m_invalid = true;

    // 返回值
    std::string result = "";

    // 单元格的总数目
    int totalCellNumber = row * col;

    // 行与列的乘积必须是偶数
    if (!_NUMBER_IS_EVEN_(totalCellNumber))
    {
        result = "Error: The row and column multiplication must be an even number.";
        return result;
    }

    // 单元格数目太少了
    if (totalCellNumber < MinUnitNumber)
    {
        std::stringstream temp;
        temp << MinUnitNumber << ".";
        result = "Error: Cell number should be more than " + temp.str();
        return result;
    }

    // 单元格数目太多了
    if (totalCellNumber > MaxUnitNumber)
    {
        std::stringstream temp;
        temp << MaxUnitNumber << ".";
        result = "Error: Cell number should be less than " + temp.str();
        return result;
    }

    // 图片数目太少了
    if (imageKind < MinimageKind)
    {
        std::stringstream temp;
        temp << MinimageKind << ".";
        result = "Error: Image kind should be more than " + temp.str();
        return result;
    }

    // 图片数目太多了
    if (imageKind > MaximageKind)
    {
        std::stringstream temp;
        temp << MaximageKind << ".";
        result = "Error: Image kind should be less than " + temp.str();
        return result;
    }

    // 清空地图, 重新分配大小
    m_row = row + 2;
    m_col = col + 2;
    m_map.clear();
    m_map.resize(m_row * m_col);

    m_imageKind = imageKind;

    // 保存各种图片的个数
    std::vector<int> imageNumber;
    imageNumber.resize(imageKind);

    // 各类图片的个数是奇数的和, 用来使整个地图中的各类图片都为偶数个
    // 每多 一类图片的个数为奇数个, 该值加1;  为偶数个, 该值减1
    int individualNumber = 0;

    // 已分配单元格的个数
    int allotCellNumber = 0;

    // 暂时保存
    int iRow = 1;
    int iCol = 1;

    // 开始随机分配
    // 先随机分配图片
    while (true)
    {
        // 未分配的单元格的个数
        // 如果未分配的单元格个数小于等于单个图片的个数, 最后几个未分配的单元格应该是填充单个图片
        // 这样才可以保证所有图片都是偶数个;   individualNumber + 1 只是为了提前跳出循环
        if ((totalCellNumber - allotCellNumber) <= (individualNumber + 1))
        {
            break;
        }

        if (iCol > col)
        {
            iRow++;
            iCol = 1;
        }

        // 分配随机数 得到图片的索引
        int random = Random(0, imageKind);


        // 该图片个数加1
        imageNumber[random] += 1;

        // 如果图片数是偶数
        if (_NUMBER_IS_EVEN_(imageNumber[random]))
        {
            // 如果图片数是偶数, 则表示之前为奇数个, 所以individualNumber需要减1
            individualNumber -= 1;
        }
        else
        {
            // 如果图片数是奇数, 则表示之前为偶数个, 所以individualNumber需要减1
            individualNumber += 1;
        }

        // random + 1: 因为 0 表示空单元格
        m_map[iRow * m_col + iCol] = random + 1;

        allotCellNumber++;
        iCol++;
    }

    // 填充最后几个单独的图片
    for (; allotCellNumber < totalCellNumber; allotCellNumber++, iCol++)
    {
        if (iCol > col)
        {
            iRow++;
            iCol = 1;
        }
        for (int i = 0; i < imageKind; i++)
        {
            // 如果图片个数是奇数个
            if (!_NUMBER_IS_EVEN_(imageNumber[i]))
            {
                m_map[iRow * m_col + iCol] = i + 1;
                imageNumber[i] += 1;
                individualNumber -= 1;
                break;
            }
        }
    }

    // 数据完成
    m_invalid = false;

    return result;
}

int PictureMatchingCore::Random(int frist, int last)
{
    return frist + rand() % (last - frist);
}

bool PictureMatchingCore::ReLayout()
{
    // 线程锁
    LockClass lock(&m_mutex);

    if (m_invalid)
    {
        return false;
    }

    int totalCellNember = m_row * m_col;

    // 重新布列2次
    for (int i = 0; i < 2; i++)
    {
        // 重新排列
        for (int j = 0; j < totalCellNember; j++)
        {
            // 地图不是 0
            if (CELL_NO_HAS_IMAGE != m_map[j])
            {
                int random = 0;

                // 随机得到一个不为空的位置
                do
                {
                    random = Random(0, totalCellNember);
                } while (CELL_NO_HAS_IMAGE == m_map[random]);

                // 更换位置
                int temp = m_map[random];
                m_map[random] = m_map[j];
                m_map[j] = temp;
            }
        }
    }

    return true;
}

int PictureMatchingCore::GetCell(int row, int col)
{
    // 线程锁
    LockClass lock(&m_mutex);

    if (m_invalid)
    {
        return CELL_INVALID;
    }

    // 不符合的行
    if ((row < 1) || (row > m_row - 1))
    {
        return CELL_INVALID;
    }

    // 不符合的列
    if ((col < 1) || (col > m_col - 1))
    {
        return CELL_INVALID;
    }

    return m_map[row * m_col + col];
}

bool PictureMatchingCore::SetCell(int row, int col, int value)
{
    // 线程锁
    LockClass lock(&m_mutex);

    if (m_invalid)
    {
        return false;
    }

    // 不符合的行
    if ((row < 1) || (row > m_row - 1))
    {
        return false;
    }

    // 不符合的列
    if ((col < 1) || (col > m_col - 1))
    {
        return false;
    }

    // 不符合的值
    if ((value < CELL_NO_HAS_IMAGE) || (value > m_imageKind))
    {
        return false;
    }

    m_map[row * m_col + col] = value;
    return true;
}

void PictureMatchingCore::FindNext(std::mutex *pMutex, std::vector<struct CellPoint> node, std::list<std::vector<struct CellPoint>> *pPush, std::vector<int> *pMap)
{
    // 线程锁
    LockClass lock(pMutex);

    // 得到最端点的坐标,方向,直线数
    int iRow = node[node.size() - 1].m_row;
    int iCol = node[node.size() - 1].m_col;
    Direction iDirection = node[node.size() - 1].m_direction;
    int iLineNumber = node[node.size() - 1].m_lineNumber;

    // 向上
    if ((iRow - 1) >= 0)
    {
        // 如果不是无效值
        if ((*pMap)[(iRow - 1) * m_col + iCol] != CELL_INVALID)
        {
            struct CellPoint cell;
            std::vector<struct CellPoint> nodeTemp = node;
            cell.m_row = iRow - 1;
            cell.m_col = iCol;
            cell.m_lineNumber = iLineNumber;
            cell.m_direction = iDirection;

            // 判断方向
            if (iDirection != To_Top)
            {
                cell.m_direction = To_Top;
                cell.m_lineNumber += 1;
            }

            // 连线数
            if (cell.m_lineNumber <= MaxConnectLine)
            {
                // 该点没有遍历过
                if ((*pMap)[cell.m_row * m_col + cell.m_col] == CELL_NO_HAS_IMAGE)
                {
                    (*pMap)[cell.m_row * m_col + cell.m_col] = cell.m_lineNumber;
                    nodeTemp.push_back(cell);
                    pPush->push_back(nodeTemp);
                }
                else if ((*pMap)[cell.m_row * m_col + cell.m_col] >= cell.m_lineNumber)
                {
                    (*pMap)[cell.m_row * m_col + cell.m_col] = cell.m_lineNumber;
                    nodeTemp.push_back(cell);
                    pPush->push_back(nodeTemp);
                }
            }
        }
    }


    // 向下
    if ((iRow + 1) < m_row)
    {
        // 如果不是无效值
        if ((*pMap)[(iRow + 1) * m_col + iCol] != CELL_INVALID)
        {
            struct CellPoint cell;
            std::vector<struct CellPoint> nodeTemp = node;
            cell.m_row = iRow + 1;
            cell.m_col = iCol;
            cell.m_lineNumber = iLineNumber;
            cell.m_direction = iDirection;

            // 判断方向
            if (iDirection != To_Bottom)
            {
                cell.m_direction = To_Bottom;
                cell.m_lineNumber += 1;
            }

            // 连线数
            if (cell.m_lineNumber <= MaxConnectLine)
            {
                // 该点没有遍历过
                if ((*pMap)[cell.m_row * m_col + cell.m_col] == CELL_NO_HAS_IMAGE)
                {
                    (*pMap)[cell.m_row * m_col + cell.m_col] = cell.m_lineNumber;
                    nodeTemp.push_back(cell);
                    pPush->push_back(nodeTemp);
                }
                else if ((*pMap)[cell.m_row * m_col + cell.m_col] >= cell.m_lineNumber)
                {
                    (*pMap)[cell.m_row * m_col + cell.m_col] = cell.m_lineNumber;
                    nodeTemp.push_back(cell);
                    pPush->push_back(nodeTemp);
                }
            }
        }
    }

    // 向左
    if ((iCol - 1) >= 0)
    {
        // 如果不是无效值
        if ((*pMap)[iRow * m_col + (iCol - 1)] != CELL_INVALID)
        {
            struct CellPoint cell;
            std::vector<struct CellPoint> nodeTemp = node;
            cell.m_row = iRow;
            cell.m_col = iCol - 1;
            cell.m_lineNumber = iLineNumber;
            cell.m_direction = iDirection;

            // 判断方向
            if (iDirection != To_Left)
            {
                cell.m_direction = To_Left;
                cell.m_lineNumber += 1;
            }

            // 连线数
            if (cell.m_lineNumber <= MaxConnectLine)
            {
                // 该点没有遍历过
                if ((*pMap)[cell.m_row * m_col + cell.m_col] == CELL_NO_HAS_IMAGE)
                {
                    (*pMap)[cell.m_row * m_col + cell.m_col] = cell.m_lineNumber;
                    nodeTemp.push_back(cell);
                    pPush->push_back(nodeTemp);
                }
                else if ((*pMap)[cell.m_row * m_col + cell.m_col] >= cell.m_lineNumber)
                {
                    (*pMap)[cell.m_row * m_col + cell.m_col] = cell.m_lineNumber;
                    nodeTemp.push_back(cell);
                    pPush->push_back(nodeTemp);
                }
            }
        }
    }

    // 向右
    if ((iCol + 1) < m_col)
    {
        // 如果不是无效值
        if ((*pMap)[iRow * m_col + (iCol + 1)] != CELL_INVALID)
        {
            struct CellPoint cell;
            std::vector<struct CellPoint> nodeTemp = node;
            cell.m_row = iRow;
            cell.m_col = iCol + 1;
            cell.m_lineNumber = iLineNumber;
            cell.m_direction = iDirection;

            // 判断方向
            if (iDirection != To_Right)
            {
                cell.m_direction = To_Right;
                cell.m_lineNumber += 1;
            }

            // 连线数
            if (cell.m_lineNumber <= MaxConnectLine)
            {
                // 该点没有遍历过
                if ((*pMap)[cell.m_row * m_col + cell.m_col] == CELL_NO_HAS_IMAGE)
                {
                    (*pMap)[cell.m_row * m_col + cell.m_col] = cell.m_lineNumber;
                    nodeTemp.push_back(cell);
                    pPush->push_back(nodeTemp);
                }
                else if ((*pMap)[cell.m_row * m_col + cell.m_col] >= cell.m_lineNumber)
                {
                    (*pMap)[cell.m_row * m_col + cell.m_col] = cell.m_lineNumber;
                    nodeTemp.push_back(cell);
                    pPush->push_back(nodeTemp);
                }
            }
        }
    }
    // if()
}

std::vector<int> PictureMatchingCore::Connect(int srcRow, int srcCol, int destRow, int destCol)
{
    // 线程锁
    LockClass lock(&m_mutex);

    std::vector<int> result;

    result.clear();

    if (m_invalid)
    {
        return result;
    }

    // 不符合的行
    if (((srcRow < 1) || (srcRow > m_row - 1)) || ((destRow < 1) || (destRow > m_row - 1)))
    {
        return result;
    }

    // 不符合的列
    if (((srcCol < 1) || (srcCol > m_col - 1)) || ((destCol < 1) || (destCol > m_col - 1)))
    {
        return result;
    }

    // 单元格值为空
    if (CELL_NO_HAS_IMAGE == m_map[srcRow * m_col + srcCol])
    {
        return result;
    }

    // 单元格值不同
    if (m_map[srcRow * m_col + srcCol] != m_map[destRow * m_col + destCol])
    {
        return result;
    }

    // Src 点
    struct CellPoint src;
    src.m_row = srcRow;
    src.m_col = srcCol;
    src.m_direction = To_No;
    src.m_lineNumber = 0;

    // Dest 点
    struct CellPoint dest;
    dest.m_row = destRow;
    dest.m_col = destCol;
    src.m_direction = To_No;

    // 用来储存下次要访问的节点
    std::list<std::vector<struct CellPoint>> pop;

    // 用来存储本次找到的新节点
    std::list<std::vector<struct CellPoint>> push;

    // 线程锁
    std::mutex mutex;

    // 添加src点
    std::vector<struct CellPoint> fristNode;
    fristNode.push_back(src);
    pop.push_back(fristNode);

    // 地图的镜像
    std::vector<int> map = m_map;
    {
        // 将地图镜像反转, 有效值改为无效值
        std::vector<int>::iterator it;
        for (it = map.begin(); it != map.end(); it++)
        {
            int value = *it;
            if (value != CELL_NO_HAS_IMAGE)
            {
                *it = CELL_INVALID;
            }
        }

        // 将终点的值改为0, 这样在搜索时可以在新节点中看到这点
        map[destRow * m_col + destCol] = CELL_NO_HAS_IMAGE;
    }

    // 遍历
    std::list<std::vector<struct CellPoint>>::iterator it;
    while (true)
    {
        // 不存在没有扫描的节点
        if (pop.empty())
        {
            return result;
        }

        // 通过已找到的节点去搜索未找到的新节点
        for (it = pop.begin(); it != pop.end(); it++)
        {
            std::vector<struct CellPoint> node = *it;
            std::thread td(&PictureMatchingCore::FindNext, this, &mutex, node, &push, &map);
            // td.detach();
            td.join();
        }

        // 线程锁
        LockClass lock(&mutex);

        // 在新找到的节点中判断是否找到终点
        for (it = push.begin(); it != push.end(); it++)
        {
            std::vector<struct CellPoint> node = *it;

            int size = node.size();

            struct CellPoint cell = node[size - 1];

            // 找到终点了
            if ((cell.m_col == destCol) && (cell.m_row == destRow))
            {
                // 直线数
                result.push_back(cell.m_lineNumber);
                result.push_back(0);

                for (int i = 0; i < size; i++)
                {
                    result.push_back(node[i].m_row);
                    result.push_back(node[i].m_col);
                }
                return result;
            }
        }

        // 清空已扫描的节点
        pop.clear();

        // 将扫描到的新节点作为下一次扫描的节点
        pop = push;

        // 清空新节点
        push.clear();
    }


    // std::thread td(&PictureMatchingCore::FindNext, this, node, &mutex);

    return result;
}


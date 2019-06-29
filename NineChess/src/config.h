#ifndef CONFIG_H
#define CONFIG_H

// 打印更多调试信息
#define DEBUG

// 播放声音
#define PLAY_SOUND

// Alpha-Beta 随机排序孩子结点
//#define AB_RANDOM_SORT_CHILDREN

// 调试博弈树 (耗费大量内存)
//#define DEBUG_AB_TREE

// 摆棋阶段动态调整搜索深度
#define GAME_PLACING_DYNAMIC_DEPTH

// 摆棋阶段固定搜索深度
#define GAME_PLACING_FIXED_DEPTH  2

// 走棋阶段固定搜索深度
#define GAME_MOVING_FIXED_DEPTH  10

// 摆棋阶段在叉下面显示被吃的子
#define GAME_PLACING_SHOW_CAPTURED_PIECES

// 启动时窗口最大化
//#define SHOW_MAXIMIZED_ON_LOAD

// 绘制 SEAT 编号
#define DRAW_SEAT_NUMBER

#endif // CONFIG_H
#include "Heuristic.h"
/*
    ���ڼ��������پ��룬�õ���������Ӧ�ڵ�λ�á�
    @grideSize: ��������Ĵ�С��8��������Ӧ�ð���0-8������sizeΪ9
    @value: �����ֵ

    ������
    0 1 2
    3 4 5
    6 7 8

    Position position = getPosition(9, 5);	// ����ֵ5Ӧ���ڵ�λ��
    position��ֵӦ��Ϊ(1, 2)��Ҳ����5Ӧ���ڵ�2�е�3��
*/
heuristic::Position heuristic::getPosition(int gridSize, int value) {
    int width = (int)sqrt(gridSize);
    return { value / width, value % width };
}

int heuristic::misplace(std::vector<int>& state, std::vector<int>& goalState) {
    /*
        h1 = the number of misplaced tiles.
        Example:
        Goal:
        0 1 2
        3 4 5
        6 7 8
        Current:
        7 2 4
        5 0 6
        8 3 1

        All of the eight tiles are out of position, so the start state would have h1 = 8.
        h1 is an admissible heuristic because it is clear that any tile that is out of place must be
       moved at least once.

        Reference:
        3.6 HEURISTIC FUNCTIONS
    */

    int h1 = 0;
    for (int i = 0; i < state.size(); ++i)
        if (state[i] && state[i] != goalState[i])
            ++h1;
    return h1;
}

int heuristic::manhattan(std::vector<int>& state, std::vector<int>& goalState) {
    /*
        h2 = the sum of the distances of the tiles from their goal positions.
        Example:
        Goal:
        0 1 2
        3 4 5
        6 7 8
        Current:
        7 2 4
        5 0 6
        8 3 1

        Because tiles cannot move along diagonals, the distance we will count is the sum of the
       horizontal and vertical distances. This is sometimes called the city block distance or
       Manhattan distance. h2 is also admissible because all any move can do is move one tile one
       step closer to the goal. Tiles 1 to 8 in the start state give a Manhattan distance of h2 =
       3+1 + 2 + 2+ 2 + 3+ 3 + 2 = 18.

        Reference:
        3.6 HEURISTIC FUNCTIONS
    */
    int h2 = 0, problemSize = std::sqrt(state.size());
    for (int i = 0; i < state.size(); ++i)
        if (state[i]) {
            auto [x, y] = getPosition(state.size(), state[i]);
            h2 += std::abs(x - i / problemSize) + std::abs(y - i % problemSize);
        }

    return h2;
}

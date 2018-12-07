#ifndef WZQ_POINT_H
#define WZQ_POINT_H

#include <cstdio>

class WZQPoint
{
public:
    WZQPoint(const int &x_ = 0, const int &y_ = 0, const int &score_ = 0);

    static bool AscendingOrder(const WZQPoint &lhs, const WZQPoint &rhs);
    static bool DescendingOrder(const WZQPoint &lhs, const WZQPoint &rhs);
    bool operator==(const WZQPoint &rhs) const;

    void stdPrint();
private:
    static const int dx[8];
    static const int dy[8];

    int x;
    int y;
    int score;

    WZQPoint &Move_(const int &direction, const int &dist = 1);
    WZQPoint Find_(const int &direction, const int &dist = 1);
    
friend class WZQBoard;
friend class WZQMinimax;
};

inline WZQPoint::WZQPoint(const int &x_, const int &y_, const int &score_) {
    x = x_;
    y = y_;
    score = score_;
}

inline bool WZQPoint::operator==(const WZQPoint &rhs) const {
    return x == rhs.x && y == rhs.y;
}

inline bool WZQPoint::AscendingOrder(const WZQPoint &lhs, const WZQPoint &rhs) {
    return lhs.score < rhs.score; 
}

inline bool WZQPoint::DescendingOrder(const WZQPoint &lhs, const WZQPoint &rhs) {
    return lhs.score > rhs.score; 
}

inline WZQPoint &WZQPoint::Move_(const int &direction, const int &dist) {
    x += dist * dx[direction];
    y += dist * dy[direction];
    return *this;
}

inline WZQPoint WZQPoint::Find_(const int &direction, const int &dist) {
    WZQPoint ret = *this;
    return ret.Move_(direction, dist);
}

inline void WZQPoint::stdPrint() {
    printf("(%4d,%4d)", x, y);
}

#endif
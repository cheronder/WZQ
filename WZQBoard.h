#ifndef WZQ_BOARD_H
#define WZQ_BOARD_H

#include <vector>
#include <cassert>
#include <random>

#include "WZQPoint.h"

#define GRID_MAXSIZE 20

typedef unsigned long long WZQHash;

enum WZQPiece {
    WZQPieceBlank = 0,
    WZQPieceBlack = 1,
    WZQPieceWhite = 2
}; 

enum WZQPlayer {
    WZQPlayerBlack = 0,
    WZQPlayerWhite = 1
};

enum WZQStatus {
    WZQStatusTie = 0,
    WZQStatusBlack = 1,
    WZQStatusWhite = 2
};

class WZQBoard
{
public:
    WZQBoard(const int &size);
    ~WZQBoard();

    void Reset();
    bool MakeMove(const WZQPlayer &player, const WZQPoint &pos);
    void UndoMove(const WZQPoint &pos);

    WZQPiece GetPiece(const WZQPoint &pos);
    WZQStatus GetStatus();
    WZQHash GetHash();

    int GetTotalScore(const WZQPlayer &who);
    void ChoiceSettings(const int &maxChoice, const int &neibOffest);
    std::vector<WZQPoint> *GetHeuristicChoices(const WZQPlayer &who);

    void StdPrint();

private:
    static const int patternScores_[3][6];

    const int size_;
    WZQStatus status_;
    int maxChoice_ = 10;
    int neibOffset_ = 2;
    WZQHash hash_;
    std::mt19937_64 rndEngine_;

    WZQPiece grid_[GRID_MAXSIZE][GRID_MAXSIZE];
    int numNeighbours_[GRID_MAXSIZE][GRID_MAXSIZE];
    WZQHash hashGrid_[GRID_MAXSIZE][GRID_MAXSIZE][3];

    void SetPiece_(const int &x, const int &y, const WZQPiece &piece);
    void SetPiece_(const WZQPoint &pos, const WZQPiece &piece);
    bool IsValidPos_(const int &x);
    bool IsValidPos_(const WZQPoint &pos);

    bool CheckWin_(const WZQPoint &pos);
    std::vector<WZQPoint> *GetPossibleChoices_();
    int GetLineScore_(const WZQPlayer &who,
                      WZQPoint beginPos, 
                      const int &direction);
};

inline WZQPiece WZQBoard::GetPiece(const WZQPoint &pos) {
    assert(IsValidPos_(pos));
    return grid_[pos.x][pos.y];
}

inline WZQStatus WZQBoard::GetStatus() {
    return status_;
}

inline WZQHash WZQBoard::GetHash() {
    return hash_;
}

inline void WZQBoard::SetPiece_(const int &x,
                                const int &y,
                                const WZQPiece &piece)
{
    assert(IsValidPos_(x));
    assert(IsValidPos_(y));
    grid_[x][y] = piece;
}

inline void WZQBoard::SetPiece_(const WZQPoint &pos, const WZQPiece &piece) {
    assert(IsValidPos_(pos));
    grid_[pos.x][pos.y] = piece;
}

inline bool WZQBoard::IsValidPos_(const int &x) {
    return 0 <= x && x < size_;
}

inline bool WZQBoard::IsValidPos_(const WZQPoint &pos) {
    return IsValidPos_(pos.x) && IsValidPos_(pos.y);
}

inline void WZQBoard::ChoiceSettings(const int &maxChoice, 
                                     const int &neibOffest) 
{
    maxChoice_ = maxChoice;
    neibOffset_ = neibOffest;
}

#endif
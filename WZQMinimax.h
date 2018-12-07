#ifndef WZQ_MINIMAX_H
#define WZQ_MINIMAX_H

#include <map>
#include <vector>
#include "WZQBoard.h"

class WZQBoardInfo
{
public:
    WZQBoardInfo(const int &eval = 0,
                 const int &depth = 0,
                 std::vector<WZQPoint> *choice = nullptr);
    ~WZQBoardInfo();
private:
    int eval_;
    int depth_;
    std::vector<WZQPoint> *choices_;
friend class WZQMinimax;
};

inline WZQBoardInfo::WZQBoardInfo(const int &eval,
                                  const int &depth, 
                                  std::vector<WZQPoint> *choice)
{
    eval_ = eval;
    depth_ = depth;
    choices_ = choice;
}

inline WZQBoardInfo::~WZQBoardInfo() {
    // delete choices_;
}

class WZQMinimax
{
public:
    WZQMinimax(WZQBoard &board, const WZQPlayer &who);
    ~WZQMinimax();

    int GetEvaluation();
    WZQPoint GetBestMove();
    void SearchSettings(const int &maxDepth, const int &victoryEval);

private:
    static const int scoreInf_;
    WZQBoard &board_;
    const WZQPlayer who_;
    int maxDepth_ = 7;
    int enoughEvalDepth_ = 4;
    int victoryEval_ = 80000;

    WZQHash curBoardHash_ = 0;
    int curEval_ = 0;
    WZQPoint bestMove_;
    std::map<WZQHash, WZQBoardInfo> *searchedBoards_ = nullptr;

    void Minimax_();
    void Negamax_();
    int IDS_(const WZQPlayer &who,
             const int &depth, 
             int alpha, 
             int beta);
};

inline int WZQMinimax::GetEvaluation() {
    if (curBoardHash_ != board_.GetHash()) Minimax_();
    return curEval_;
}

inline WZQPoint WZQMinimax::GetBestMove() {
    if (curBoardHash_ != board_.GetHash()) Minimax_();
    return bestMove_;
}

inline void WZQMinimax::SearchSettings(const int &maxDepth, const int &victoryEval) {
    maxDepth_ = maxDepth;
    victoryEval_ = victoryEval;
}

#endif
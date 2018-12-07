#include "WZQMinimax.h"

#include <algorithm>

const int WZQMinimax::scoreInf_ = 0x7fffffff;

WZQMinimax::WZQMinimax(WZQBoard &board, const WZQPlayer &who)
    :board_(board), who_(who), searchedBoards_()
{
    searchedBoards_ = new std::map<WZQHash, WZQBoardInfo>;
}

WZQMinimax::~WZQMinimax() {
    delete searchedBoards_;
    if (curBoardInfo_.choices_ != nullptr) 
        delete curBoardInfo_.choices_;
}

void WZQMinimax::Minimax_() {
    for (int i = 1; i <= maxDepth_; i++) {
        int eval = IDS_(who_, i, -scoreInf_, scoreInf_);
        if (eval >= victoryEval_) break;
    }

    auto &choices = curBoardInfo_.choices_;
    if (choices != nullptr) delete choices;
    curBoardHash_ = board_.GetHash();
    curBoardInfo_ = searchedBoards_->at(curBoardHash_);
    // choices = new std::vector<WZQPoint>;

    /*
    for (auto &info : *searchedBoards_) {
        delete info.second.choices_;
    }
    */

    searchedBoards_->clear();
}

int WZQMinimax::IDS_(const WZQPlayer &who, 
                    const int &depth, 
                    int alpha, 
                    int beta)
{

    // board_.StdPrint();
    WZQHash hash = board_.GetHash();
    std::vector<WZQPoint> *choices = nullptr;
    WZQPlayer enemy = who == WZQPlayerBlack ? WZQPlayerWhite : WZQPlayerBlack;
    
    if (board_.GetStatus() != WZQStatusTie)
        return (int) board_.GetStatus() == (int) enemy + 1 ? scoreInf_ : -scoreInf_;
    if (depth == 0) return board_.GetTotalScore(enemy);

    if (searchedBoards_->count(hash)) {
        WZQBoardInfo &boardInfo = searchedBoards_->at(hash);
        int eval = boardInfo.eval_;
        if (boardInfo.depth_ >= depth || eval >= victoryEval_)
            return eval;
        choices = boardInfo.choices_;
    }
    
    if (choices == nullptr) choices = board_.GetHeuristicChoices(who);

    int eval;
    auto iterPos = choices->begin();
    if (who == who_) {
        for (; iterPos != choices->end(); iterPos++) {
            board_.MakeMove(who, *iterPos);
            int subEval = IDS_(enemy, depth-1, alpha, beta);
            board_.UndoMove(*iterPos);

            iterPos->score = subEval;
            alpha = std::max(subEval, alpha);
            if (alpha >= beta) { break; }
        }
        eval = alpha;
        std::sort(choices->begin(), ++iterPos, WZQPoint::DescendingOrder);
    } else {
        for (; iterPos != choices->end(); iterPos++) {
            board_.MakeMove(who, *iterPos);
            int subEval = IDS_(enemy, depth-1, alpha, beta);
            board_.UndoMove(*iterPos);

            iterPos->score = subEval;
            beta = std::min(subEval, beta);
            if (alpha >= beta) { break; }
        }
        eval = beta;
        std::sort(choices->begin(), ++iterPos, WZQPoint::AscendingOrder);
    }

    assert(choices != nullptr);
    (*searchedBoards_)[hash] = WZQBoardInfo(eval, depth, choices);

    return eval;
}
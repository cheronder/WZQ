#include <cstdio>
#include <ctime>
#include <algorithm>

#include "WZQBoard.h"

const int WZQBoard::patternScores_[3][6] 
        = {{0, 10, 100, 1000, 10000, 100000},
           {0,  1,  10,  100,  1000, 100000},
           {0,  0,   0,    0,     0, 100000}};

WZQBoard::WZQBoard(const int &size)
    :size_(size)
{
    Reset();
}

WZQBoard::~WZQBoard() {}

void WZQBoard::Reset() {
    status_ = WZQStatusTie;
    for (int i = 0; i < size_; i++)
        for (int j = 0; j < size_; j++) {
            grid_[i][j] = WZQPieceBlank;
            numNeighbours_[i][j] = 0;
        }
    
    rndEngine_ = std::mt19937_64(std::time(nullptr));
    hash_ = 0ull;
    for (int i = 0; i < size_; i++)
        for (int j = 0; j < size_; j++) {
            hashGrid_[i][j][WZQPieceBlank] = rndEngine_();
            hashGrid_[i][j][WZQPieceBlack] = rndEngine_();
            hashGrid_[i][j][WZQPieceWhite] = rndEngine_();
            hash_ ^= hashGrid_[i][j][WZQPieceBlank];
        }
}

bool WZQBoard::MakeMove(const WZQPlayer &player, const WZQPoint &pos) {
    assert(status_ == WZQStatusTie);
    if (status_ != WZQStatusTie) return false;
    if (GetPiece(pos) != WZQPieceBlank) return false;

    hash_ ^= hashGrid_[pos.x][pos.y][WZQPieceBlank];
    if (player == WZQPlayerBlack) 
        SetPiece_(pos, WZQPieceBlack);
    else
        SetPiece_(pos, WZQPieceWhite);

    if (GetLocalScore_(player, pos) >= patternScores_[0][5]) {
        status_ = player == WZQPlayerBlack ? WZQStatusBlack : WZQStatusWhite;
        lastMove_ = pos;
    }
        
    for (int i = pos.x - neibOffset_; i <= pos.x + neibOffset_; i++) {
        if (!IsValidPos_(i)) continue;
        for (int j = pos.y - neibOffset_; j <= pos.y + neibOffset_; j++) {
            if (!IsValidPos_(j)) continue;
            numNeighbours_[i][j] += 1;
        }
    }

    hash_ ^= hashGrid_[pos.x][pos.y][GetPiece(pos)];
    return true;
}

bool WZQBoard::UndoMove(const WZQPoint &pos) {
    if (GetPiece(pos) == WZQPieceBlank) return false;
    if (status_ != WZQStatusTie && !(pos == lastMove_))
        return false;

    hash_ ^= hashGrid_[pos.x][pos.y][GetPiece(pos)];
    status_ = WZQStatusTie;

    SetPiece_(pos, WZQPieceBlank);
    for (int i = pos.x - neibOffset_; i <= pos.x + neibOffset_; i++) {
        if (!IsValidPos_(i)) continue;
        for (int j = pos.y - neibOffset_; j <= pos.y + neibOffset_; j++) {
            if (!IsValidPos_(j)) continue;
            numNeighbours_[i][j] -= 1;
        }
    }

    hash_ ^= hashGrid_[pos.x][pos.y][WZQPieceBlank];
    return true;
}

void WZQBoard::StdPrint() {
    char strGrid[GRID_MAXSIZE*2+5][GRID_MAXSIZE*4+5];
    
    for (int i = 0; i <= 2*size_+2; i++)
        for (int j = 0; j <= 4*size_+3; j++)
            strGrid[i][j] = ' ';

    for (int i = 0; i < size_; i++) {
        strGrid[0][i*4+4] = (i/10 ? '0'+i/10 : ' ');
        strGrid[0][i*4+5] = '0'+i%10;
        strGrid[i*2+2][0] = (i/10 ? '0'+i/10 : ' ');
        strGrid[i*2+2][1] = '0'+i%10;
    }
    
    for (int i = 0; i <= size_; i++)
        for (int j = 0; j <= size_; j++)
            strGrid[i*2+1][j*4+3] = '+';

    for (int i = 0; i < size_; i++)
        for (int j = 0; j <= size_; j++)
            strGrid[i*2+2][j*4+3] = '|';

    for (int i = 0; i <= size_; i++)
        for (int j = 0; j < size_; j++) {
            strGrid[i*2+1][j*4+4] = '-';
            strGrid[i*2+1][j*4+5] = '-';
            strGrid[i*2+1][j*4+6] = '-';
        }

    for (int i = 0; i < size_; i++)
        for (int j = 0; j < size_; j++)
            if (grid_[i][j] == WZQPieceBlack)
                strGrid[i*2+2][j*4+5] = 'X';
            else if (grid_[i][j] == WZQPieceWhite)
                strGrid[i*2+2][j*4+5] = 'O';
            // else strGrid[i*2+2][j*4+5] = numNeighbours_[i][j]+'0';

    for (WZQPoint &pos : *GetHeuristicChoices(WZQPlayerBlack)) {
        strGrid[pos.x*2+2][pos.y*4+6] = '*';
    }
    
    
    for (int i = 0; i <= 2*size_+1; i++) {
        strGrid[i][4*size_+4] = '\0';
        printf("%s\n", strGrid[i]);
    }
}

std::vector<WZQPoint> *WZQBoard::GetHeuristicChoices(const WZQPlayer &who) {
    std::vector<WZQPoint> *ret = new std::vector<WZQPoint>;

    for (int i = 0; i < size_; i++)
        for (int j = 0; j < size_; j++)
            if (numNeighbours_[i][j] > 0 && grid_[i][j] == WZQPieceBlank) {
                WZQPoint temp = WZQPoint(i, j);
                int HScore = 0;
                WZQPlayer enemy = (WZQPlayer) (1 - (int) who);
                HScore += GetLocalScore_(who, temp);
                HScore += GetLocalScore_(enemy, temp) / 8;
                temp.score = HScore;
                ret->push_back(temp);
            }

    sort(ret->begin(), ret->end(), WZQPoint::DescendingOrder);
    if (ret->size() > (size_t) maxChoice_) ret->resize(maxChoice_);
    if (ret->size() == 0) ret->push_back(WZQPoint(size_/2, size_/2));
    return ret;
}

int WZQBoard::GetTotalScore(const WZQPlayer &who) {
    assert(status_ == WZQStatusTie);
    int totalScores = 0;
    WZQPoint tempPos;
    for (int i = 0; i < size_; i++) {
        tempPos = WZQPoint(0, i);
        totalScores += GetLineScore_(who, tempPos, 2);
        totalScores += GetLineScore_(who, tempPos, 4);
        totalScores += GetLineScore_(who, tempPos, 6);
        tempPos = WZQPoint(i, 0);
        totalScores += GetLineScore_(who, tempPos, 0);
    }
    for (int i = 1; i < size_; i++) {
        tempPos = WZQPoint(i, 0);
        totalScores += GetLineScore_(who, tempPos, 4);
        tempPos = WZQPoint(i, size_-1);
        totalScores += GetLineScore_(who, tempPos, 6);
    }
    return totalScores;
}

int WZQBoard::GetLineScore_(const WZQPlayer &who,
                            WZQPoint pos,
                            const int &direction)
{
    int scores[3] = {};
    int blocks = 1;
    int counts = 1;
    WZQPiece cur = GetPiece(pos);

    while (IsValidPos_(pos.Move_(direction))) {
        WZQPiece next = GetPiece(pos);
        if (next == cur) counts++;
        else {
            if (next != WZQPieceBlank) blocks++;
            if (counts > 5) scores[cur] += patternScores_[blocks][5];
            else            scores[cur] += patternScores_[blocks][counts];

            blocks = cur == WZQPieceBlank ? 0 : 1; 
            counts = 1;
            cur = next;
        }
    }

    blocks++;
    if (counts > 5) scores[cur] += patternScores_[blocks][5];
    else            scores[cur] += patternScores_[blocks][counts];

    if (who == WZQPlayerBlack) return scores[WZQPieceBlack] - scores[WZQPieceWhite];
    else                       return scores[WZQPieceWhite] - scores[WZQPieceBlack];
}

int WZQBoard::GetLocalScore_(const WZQPlayer &who, const WZQPoint &pos) {
    WZQPoint tempPos;
    WZQPiece cur = who == WZQPlayerBlack ? WZQPieceBlack : WZQPieceWhite;
    if (cur == WZQPieceBlank) return false;

    int score = 0;
    int count[4] = {1, 1, 1, 1};
    int block[4] = {};

    for (int i = 0; i < 8; i++) {
        tempPos = pos;
        while (IsValidPos_(tempPos.Move_(i)) && GetPiece(tempPos) == cur)
            count[i/2]++;
        if (!IsValidPos_(tempPos) || GetPiece(tempPos) != WZQPieceBlank)
            block[i/2]++;
    }

    for (int i = 0; i < 4; i++) {
        if (count[i] > 5) score += patternScores_[0][5];
        else score += patternScores_[block[i]][count[i]];
    }

    return score;
}
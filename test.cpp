#include <iostream>
#include "WZQMinimax.h"

int main() {
    WZQBoard *board = new WZQBoard(8);
    board->ChoiceSettings(20, 2);
    WZQMinimax *minimax = new WZQMinimax(*board, WZQPlayerWhite);
    minimax->SearchSettings(6, 8000);
    int x, y, m;
    char op;
    WZQPoint pos;
    while (~scanf("%c", &op)) {
        switch(op) {

        case 'M':
            scanf("%d%d%d", &x, &y, &m);
            pos = WZQPoint(x, y);
            if (!board->MakeMove((WZQPlayer) m, pos))
                printf("bad move!!!\n");
            else {
                printf("move at ");
                pos.stdPrint();
                printf("\n");
            }
            break;

        case 'C':
            switch(board->GetStatus()) {
            case WZQStatusTie:
                printf("In tie ...\n");
                break;
            case WZQStatusBlack:
                printf("Black win!!!\n");
                break;
            case WZQStatusWhite:
                printf("White win!!!\n");
                break;
            }

        case 'U':
            scanf("%d%d", &x, &y);
            pos = WZQPoint(x, y);
            board->UndoMove(pos);
            break;

        case 'P':
            board->StdPrint();
            break;

        case 'H':
            printf("Hash : %llx\n", board->GetHash());
            break;

        case 'S':
            std::cout << "Scores : " << board->GetTotalScore(WZQPlayerBlack)
                      << std::endl;
            break;
        
        case 'E':
            printf("White evaluates : %d\n", minimax->GetEvaluation());
            break;

        case 'B':
            printf("White would make :");
            minimax->GetBestMove().stdPrint();
            printf("\n");
            break;
        
        case 'A':
            printf("White evaluates : %d\n", minimax->GetEvaluation());
            printf("White makes :");
            pos = minimax->GetBestMove();
            pos.stdPrint();
            printf("\n");
            board->MakeMove(WZQPlayerWhite, pos);
            board->StdPrint();
            break;

        case '\n': break;
        case '\r': break;

        default:
            printf("Unknown Operation ( %c )!!!\n", op);
        }

        fflush(stdout);
    }
    return 0;
}
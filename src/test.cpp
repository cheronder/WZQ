#include <iostream>
#include "WZQMinimax.h"

WZQBoard *board;
WZQMinimax *minimax;

void run(char op, int x = 0, int y = 0, WZQPlayer m = WZQPlayerBlack) {
    WZQPoint pos;

    switch(op) {
    case 'M':
        pos = WZQPoint(x, y);
        if (!board->MakeMove(m, pos))
            printf("Warining : bad move!\n");
        else {
            printf("moved at");
            pos.stdPrint();
            printf(".\n");
        }
        break;

    case 'C':
        switch(board->GetStatus()) {
        case WZQStatusTie:
            printf("In tie...\n");
            break;
        case WZQStatusBlack:
            printf("***Black win***\n");
            break;
        case WZQStatusWhite:
            printf("***White win***\n");
            break;
        }
        break;

    case 'U':
        pos = WZQPoint(x, y);
        if (!board->UndoMove(pos)) {
            printf("Warning : bad undo!\n");
        } else {
            printf("undid.\n");
        }

        break;

    case 'P':
        board->StdPrint();
        break;

    case 'H':
        printf("Board Hash : %llX\n", board->GetHash());
        break;

    case 'S':
        printf("scores : %d\n", board->GetTotalScore(m));
        break;
    
    case 'B':
        printf("would make : ");
        minimax->GetBestMove().stdPrint();
        printf(".\n");
        break;
    
    case 'A':
        printf("evaluates : %d\n", minimax->GetEvaluation());
        printf("makes : ");
        pos = minimax->GetBestMove();
        pos.stdPrint();
        printf("\n");
        board->MakeMove(m, pos);
        break;
    
    case 'L':
        pos = WZQPoint(x, y);
        printf("'s local scores : %d at ", board->GetLocalScore_(m, pos));
        pos.stdPrint();
        printf(".\n");

    case '\n': break;
    case ' ': break;
    case '\r': break;

    default:
        printf("Unknown Operation ( %c )!!!\n", op);
    }

    fflush(stdout);
}
int main() {
    printf("####################\n");
    printf("#      WZQ AI      #\n");
    printf("####################\n");
    printf("\n");

    printf("Please input board size (5 ~ 20) :\n");
    fflush(stdout);
    int size;
    scanf("%d", &size);
    board = new WZQBoard(size);
    minimax = new WZQMinimax(*board, WZQPlayerWhite);
    board->ChoiceSettings(12, 2);
    minimax->SearchSettings(8, 10001);
    printf("Please pick your color [B|W]:\n");
    printf("## <B: Black> | <W: White>\n");
    printf("## <D: Debug Mode>\n");
    fflush(stdout);
    WZQPlayer human = WZQPlayerBlack;
    WZQPlayer ai    = WZQPlayerWhite;
    char input;
    getchar();
    scanf("%c", &input);
    // std::cin >> input;
    if (islower(input)) input += 'A' - 'a';
    switch (input) {
    case 'D':
        char op;
        int x, y, m;
        while (~scanf("%c%d%d%d", &op, &x, &y, &m)) {
            printf("%s >> ", m == 0 ? "Black" : "White");
            if (islower(op)) op += 'A' - 'a';
            run(op, x, y, (WZQPlayer) m);
        }
        return 0;
    case 'B':
        human = WZQPlayerBlack;
        ai    = WZQPlayerWhite;
        break;
    case 'W':
        human = WZQPlayerWhite;
        ai    = WZQPlayerBlack;
        break;

    }

    WZQPlayer cur = WZQPlayerBlack;
    while (true) {
        if (cur == ai) {
            printf("AI ");
            run('A', 0, 0, ai);
        } else {
            int x, y;
            printf("Now it's your turn (x y):\n");
            fflush(stdout);
            scanf("%d%d", &x, &y);
            printf("Human ");
            run('M', x, y, human);
        }
        run('P');
        if (board->GetStatus() != WZQStatusTie) {
            int st = (int) board->GetStatus();
            if ((int) human == st) {
                printf("***Congratulations, you win***\n");
            } else {
                printf("***AI wins***\n");
            }
            break;
        }
        printf("\n");
        if (cur == WZQPlayerBlack) 
            cur = WZQPlayerWhite;
        else 
            cur = WZQPlayerBlack;
        fflush(stdout);
    }
    return 0;
}
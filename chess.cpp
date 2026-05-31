#include<iostream>
#include<raylib.h>
#include<vector>

using namespace std;

enum type{NONE=-1, PAWN=5, ROOK=4, KNIGHT=3, BISHOP=2, QUEEN=1, KING=0};
enum color{White=0,Black=1};
struct piece{
    type t;
    color c;
};
void setupboard(piece b[8][8])
{
    for(int i=0;i<=7;i++)
    {
        for(int j=0;j<=7;j++)
        {
            if(i==1||i==6)
            {
                b[i][j].t=PAWN;
            }
            if(i<=1)
            {
                b[i][j].c=Black;
            }
            if(i>=2&&i<=5)
            {
                b[i][j].t=NONE;
            }
            if(i>=6)
            {
                b[i][j].c=White;
            }
            if(i==0||i==7)
            {
                b[i][0].t=ROOK;
                b[i][1].t=KNIGHT;
                b[i][2].t=BISHOP;
                b[i][3].t=QUEEN;
                b[i][4].t=KING;
                b[i][5].t=BISHOP;
                b[i][6].t=KNIGHT;
                b[i][7].t=ROOK;
            }
        }
    }
}

Rectangle getpiece(type ty,color co)
{
    float cellw=float(768/6);
    float cellh=float(256/2);
    float c=float(ty);
    float r=float(co);
    return {c*cellw,r*cellh,cellw,cellh};
}

void drawpieces(piece bo[8][8],Texture2D p)
{
    for(int i=0;i<=7;i++)
    {
        for(int j=0;j<=7;j++)
        {
            if(bo[i][j].t==NONE)
            {
                continue;
            }
            Rectangle sr=getpiece(bo[i][j].t,bo[i][j].c);
            Rectangle dr={float(j*100),float(i*100),100,100};
            DrawTexturePro(p,sr,dr,{0,0},0,WHITE);
        }
    }
}
bool boundary(int r, int c) {
    return (r>=0 && r<8 && c>=0 && c<8);
}

vector<pair<int,int>> validmoves(piece Boa[8][8],int r,int c)
{
    vector<pair<int,int>> moves;
    if(Boa[r][c].t==NONE)
    {
        return moves;
    }
    color co=Boa[r][c].c;
    type t=Boa[r][c].t;
    if(t==PAWN)
    {
        int dir=(co == White) ? -1:1;
        int startrow=(co == White) ? 6:1;
        if(boundary(r+dir,c)&&Boa[r+dir][c].t==NONE)
        {
            moves.push_back({r+dir,c});
            if(r==startrow&&Boa[r+(2*dir)][c].t==NONE)
            {
                moves.push_back({r+(2*dir),c});
            }
        }
        int dcol[]={c-1,c+1};
        for(int i:dcol)
        {
            if(boundary(r+dir,i))
            {
                piece opp=Boa[r+dir][i];
                if(opp.t!=NONE&&opp.c!=co)
                {
                    moves.push_back({r+dir,i});
                }
            }
        }
    }
    else if(t==KNIGHT)
    {
        int knightmoves[8][2] = {{-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1}};
        for (int i = 0; i <= 7; i++) 
        {
            int ro = r + knightmoves[i][0];
            int col = c + knightmoves[i][1];
            if(boundary(ro,col))
            {
                if(Boa[ro][col].t==NONE||Boa[ro][col].c!=co)
                {
                    moves.push_back({ro,col});
                }
            }
        }
    }
    else if(t==KING)
    {
        int kingmoves[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
        for (int i = 0; i <= 7; i++) 
        {
            int ro = r + kingmoves[i][0];
            int col = c + kingmoves[i][1];
            if(boundary(ro,col))
            {
                if(Boa[ro][col].t==NONE||Boa[ro][col].c!=co)
                {
                    moves.push_back({ro,col});
                }
            }
        }
    }
    else
    {
        int directions[8][2] = {{-1, 0},{1, 0},{0, -1},{0, 1},{-1,-1},{-1,1},{1, -1},{1, 1}};
        int startd = 0;
        int endd = 8;
        if(t==ROOK)
        {
            startd=0;
            endd=4;
        }
        if(t==BISHOP)
        {
            startd=4;
            endd=8;
        }
        for(int i=startd;i<endd;i++)
        {
            int stepr = directions[i][0];
            int stepc = directions[i][1];
            int ro=r+stepr;
            int col=c+stepc;
            while(boundary(ro,col))
            {
                if(Boa[ro][col].t==NONE)
                {
                    moves.push_back({ro,col});
                }
                else if(Boa[ro][col].c!=co)
                {
                    moves.push_back({ro,col});
                    break;
                }
                else
                {
                    break;
                }
                ro+=stepr;
                col+=stepc;
            }
        }
    }
    return moves;
}

bool check(piece b[8][8],color cl)
{
    int kingr,kingc;
    for(int i=0;i<=7;i++)
    {
        for(int j=0;j<=7;j++)
        {
            if(b[i][j].c==cl&&b[i][j].t==KING)
            {
                kingr=i;
                kingc=j;
                break;
            }
        }
    }
    vector<pair<int,int>> vm;
    for(int i=0;i<=7;i++)
    {
        for(int j=0;j<=7;j++)
        {
            if(b[i][j].t!=NONE&&b[i][j].c!=cl)
            {
                vm=validmoves(b,i,j);
                for(unsigned int k=0;k<vm.size();k++)
                {
                    if(vm[k].first==kingr && vm[k].second==kingc)
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

vector<pair<int,int>> filter(piece bo[8][8],int r,int c,vector<pair<int,int>> mo,color co)
{
    vector<pair<int,int>> safe;
    for(unsigned int i=0;i<mo.size();i++)
    {
        piece tempb[8][8];
        for(int j=0;j<=7;j++)
        {
            for(int k=0;k<=7;k++)
            {
                tempb[j][k]=bo[j][k];
            }
        }
        int row=mo[i].first;
        int col=mo[i].second;
        tempb[row][col]=tempb[r][c];
        tempb[r][c].t=NONE;
        if(check(tempb,co)==false)
        {
            safe.push_back(mo[i]);
        }
    }
    return safe;
}

bool end(piece b[8][8],color cl)
{
    if(check(b,cl)==false)
    {
        return false;
    }
    for(int i=0;i<=7;i++)
    {
        for(int j=0;j<=7;j++)
        {
            if(b[i][j].t!=NONE&&b[i][j].c==cl)
            {
                vector<pair<int,int>> temp=validmoves(b,i,j);
                vector<pair<int,int>> safe=filter(b,i,j,temp,cl);
                if(safe.size()>0)
                {
                    return false;
                }
            }
        }
    }
    return true;
}

int main()
{
    bool start=true;
    bool game=false;
    color winner;
    bool kingcheck=false;
    Texture2D board;
    Texture2D pieces;
    Texture2D chess;
    piece Board[8][8];
    bool pieceselect=false;
    int rowselect,colselect;
    color current=White;
    vector<pair<int,int>> m;
    InitWindow(800,800,"Chess");
    SetTargetFPS(60);
    board=LoadTexture("assets/Board.png");
    pieces=LoadTexture("assets/Pieces.png");
    chess=LoadTexture("assets/Chess.png");
    setupboard(Board);
    while(!WindowShouldClose())
    {
      if(start)
      {
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Vector2 mouse = GetMousePosition();
            if(mouse.x>=250 && mouse.x<=550 && mouse.y>=470 && mouse.y<=540)
            {
                start = false;
            }
            if (mouse.x>=250 && mouse.x<=550 && mouse.y>=570 && mouse.y<=640)
            {
                CloseWindow();
                return 0;
            }
        }
      }
      else if(game==false)
      {  
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Vector2 mousexy=GetMousePosition();
            int row=mousexy.y/100;
            int col=mousexy.x/100;
            if(boundary(row,col))
            {
                if(pieceselect==false)
                {
                    if(Board[row][col].t!=NONE&&Board[row][col].c==current)
                    {
                        pieceselect=true;
                        rowselect=row;
                        colselect=col;
                        vector<pair<int,int>> temp=validmoves(Board,rowselect,colselect);
                        m=filter(Board,rowselect,colselect,temp,current);
                    }
                }
                else
                {
                    if(row==rowselect&&col==colselect)
                    {
                        pieceselect=false;
                        m.clear();
                    }
                    else if(Board[row][col].t!=NONE&&Board[row][col].c==current)
                    {
                        rowselect=row;
                        colselect=col;
                        vector<pair<int,int>> temp=validmoves(Board,rowselect,colselect);
                        m=filter(Board,rowselect,colselect,temp,current);
                    }
                    else
                    {   
                        bool valid = false;
                        for(unsigned int i=0;i<m.size();i++) 
                        {
                            if(m[i].first==row && m[i].second==col)
                            {
                                valid = true;
                                break;
                            }
                        }
                        if(valid)
                        {
                            Board[row][col]=Board[rowselect][colselect];
                            Board[rowselect][colselect].t=NONE;
                            pieceselect=false;
                            m.clear();
                            current=(current==White)? Black:White;
                            if(end(Board,current))
                            {
                                game=true;
                                winner=(current==White)? Black:White;
                                kingcheck=true;
                            }
                            else
                            {
                                kingcheck=check(Board,current);
                            }
                        }
                        else
                        {
                            pieceselect=false;
                            m.clear();
                        }
                    }
                }
            }
        }
      }
        BeginDrawing();
        ClearBackground(BLACK);
        if(start)
        {
            DrawTexture(chess,0,-130,WHITE);
            Vector2 mouse=GetMousePosition();
            if(mouse.x>=250 && mouse.x<=550 && mouse.y>=470 && mouse.y<=540)
            {
                DrawRectangle(250,470,300,70,Color{40,180,40,255});
            }
            else
            {
                DrawRectangle(250,470,300,70,LIME);
            }
            DrawRectangleLines(250,470,300,70,GREEN);
            DrawText("START GAME",295,490,30,BLACK);
            if(mouse.x>=250 && mouse.x<=550 && mouse.y>=570 && mouse.y<=640)
            {
                DrawRectangle(250,570,300,70,Color{180,40,40,255});
            }
            else
            {
                DrawRectangle(250,570,300,70,RED);
            }
            DrawRectangleLines(250,570,300,70,MAROON);
            DrawText("EXIT",365,590,30,WHITE);
        }
        else
        {
            DrawTexture(board,0,0,WHITE);
            if(kingcheck&&game==false)
            {
                int checkr=-1;
                int checkc=-1;
                for(int i=0;i<=7;i++)
                {
                    for(int j=0;j<=7;j++)
                    {
                        if(Board[i][j].t==KING&&Board[i][j].c==current)
                        {
                            checkr=i;
                            checkc=j;
                            break;
                        }
                    }
                }
                if(checkr!=-1&&checkc!=-1)
                {
                    DrawRectangle(checkc*100,checkr*100,100,100,Color{230,50,50,150});
                }
            }
            if(pieceselect)
            {
                DrawRectangle(colselect*100,rowselect*100,100,100,Color{ 247, 247, 105, 100 });
            }
            drawpieces(Board,pieces);
            if(pieceselect)
            {
                for(unsigned int j=0;j<m.size();j++)
                {   
                    int tr=m[j].first;
                    int tc=m[j].second;
                    if(Board[tr][tc].t!=NONE)
                    {
                        DrawCircleLines(tc*100+50,tr*100+50,45,Color{ 0, 0, 0, 35 });
                        DrawCircleLines(tc*100+50,tr*100+50,43,Color{ 130, 130, 130, 100 });
                    }
                    else
                    {
                        DrawCircle(tc*100+50,tr*100+50,15,Color{ 50, 50, 50, 60 });
                    }
                }
            }
            if(game)
            {
                BeginBlendMode(BLEND_ALPHA);
                DrawRectangle(100,300,600,200,Color{0,0,0,180});
                EndBlendMode();
                DrawRectangleLines(100,300,600,200,LIGHTGRAY);
                if(winner==White)
                {
                    DrawText("White won by checkmate!",140,360,40,GOLD);
                }
                else
                {
                    DrawText("Black won by checkmate!",140,360,40,GOLD);
                }
                DrawText("Press ESC to exit",300,450,30,LIGHTGRAY);
            }
        }
        EndDrawing();
    }
    UnloadTexture(board);
    UnloadTexture(pieces);
    CloseWindow();
}